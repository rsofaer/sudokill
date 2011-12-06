#include "sudokill_core.h"
#include "board_parser.h"
#ifdef WIN32
#include <winsock.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h> 
#endif
#include <string>
#include <sstream>
#include <iostream>

using namespace hps;

/// <summary> Buffer size used to read from server socket. </summery>
enum { SockBufferSize = 1024, };

/// <summary> Write a string message to the socket. </summary>
inline bool Write(const int sockfd, const std::string& data)
{
  assert(!data.empty());
  const size_t sent = send(sockfd, data.c_str(), data.size(), 0);
  return sent == data.size();
}

/// <summary> Read all available data on the socket. </summary>
int Read(const int sockfd, const int timeoutMs, std::string* data)
{
  assert(data);
  data->clear();

  // Wait for first chunk?
  fd_set read;
  if (timeoutMs >= 0)
  {
    timeval timeout;
    {
      timeout.tv_sec = timeoutMs / 1000;
      timeout.tv_usec = 1000 * (timeoutMs % 1000);
    }
    FD_ZERO(&read);
    FD_SET(sockfd, &read);
    const int ready = select(sockfd + 1, &read, NULL, NULL, &timeout);
    if (ready < 1)
    {
      return 0;
    }
  }

  // Read data until no more on the socket.
  char buffer[SockBufferSize];
  int numRead = 0;
  for (;;)
  {
    // Read first chunk of data.
    const int sizeRecv = recv(sockfd, buffer, sizeof(buffer), 0);
    numRead += sizeRecv;
    timeval timeout;
    memset(&timeout, 0, sizeof(timeout));
    if (sizeRecv <= 0)
    {
      break;
    }
    *data = *data + std::string(buffer, buffer + sizeRecv);
    // Any more data waiting?
    FD_ZERO(&read);
    FD_SET(sockfd, &read);
    const int ready = select(sockfd + 1, &read, NULL, NULL, &timeout);
    if (ready < 1)
    {
      break;
    }
  }

  return numRead;
}

/// <summary> Sudokill command line arguments. </summary>
struct CommandLineArgs
{
  enum
  {
    Argv_Application,
    Argv_Hostname,
    Argv_Port,
    Argv_PlayerName,
    Argv_Count,
  };
  CommandLineArgs() : application(), hostname(), port(), playerName() {}
  std::string application;
  std::string hostname;
  short port;
  std::string playerName;
};

inline bool ExtractArgs(const int argc, char** argv, CommandLineArgs* args)
{
  assert(args);
  if (argc != CommandLineArgs::Argv_Count) { return false; }
  args->application = argv[CommandLineArgs::Argv_Application];
  args->hostname = argv[CommandLineArgs::Argv_Hostname];
  args->playerName = argv[CommandLineArgs::Argv_PlayerName];
  std::stringstream ssPort(argv[CommandLineArgs::Argv_Port]);
  if (!ssPort.good()) { return false; }
  short port = 0;
  ssPort >> port;
  if (0 == port) { return false; }
  args->port = port;
  return true;
}

int main(int argc, char *argv[])
{
#ifdef WIN32
  WORD wsaRqdVersion = MAKEWORD(2, 0);
  WSADATA wsaData;
  WSAStartup(wsaRqdVersion, &wsaData);
#endif

  // Check args.
  CommandLineArgs args;
  if (!ExtractArgs(argc, argv, &args))
  {
    std::cerr << "Usage: " << argv[0] << " HOSTNAME PORT PLAYER NAME" << std::endl;
    return 1;
  }

  // Open port and start connection (server should be listening).
  const short portno = static_cast<short>(args.port);
  const int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
  {
    std::cerr << "ERROR: failed opening socket." << std::endl;
    return 1;
  }
  const struct hostent* server = gethostbyname(args.hostname.c_str());
  if (NULL == server)
  {
    std::cerr << "ERROR: no such host." << std::endl;
    return 1;
  }
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
  {
    std::cerr << "ERROR: failed connecting to server." << std::endl;
    return 1;
  }

  // Write hello string as per API documentation.
  {
    const std::string helloStr = std::string("SUDOKILL_PLAYER\n") +
                                 args.playerName + std::string("\n");
    std::cout << "Write():\n" << helloStr << std::endl;
    Write(sockfd, helloStr);
  }

  // Read the first state.
  std::string stateString;
  if (Read(sockfd, -1, &stateString) > 0)
  {
    std::cout << "stateString:\n" << stateString << std::endl;
    // Initialize the player and state.
    Board board;
    int roundsPlayed = 0;
    Parser::Parse(stateString, &board);
    //GreedyPlayer player(game, 30);
    // Play until the server disconnects.
    do
    {
      Parser::Parse(stateString, &board);
      //player.Play(game);
      std::stringstream ssMove;
      const Cell& cell = board.LastMove();
      ssMove << cell.location.x << " " << cell.location.y << " "
             << cell.value << "\n";
      Write(sockfd, ssMove.str());
      ++roundsPlayed;
    } while (Read(sockfd, -1, &stateString) > 0);
    std::cout << "Played " << roundsPlayed << " rounds." << std::endl;
  }

  // Wait for primmadonna server to end.
  Read(sockfd, -1, &stateString);

  // Disconnect.
#ifdef WIN32
  closesocket(sockfd);
  WSACleanup();
#else
  close(sockfd);
#endif

  return 0;
}
