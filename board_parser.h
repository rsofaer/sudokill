#ifndef _HPS_SUDOKILL_BOARD_PARSER_H_
#define _HPS_SUDOKILL_BOARD_PARSER_H_
#include "sudokill_core.h"
#include <string>
#include <sstream>
#include <iostream>

namespace hps
{
namespace sudokill
{

/// <summary> Setup a Sudokill game from a state string. </summmary>
struct Parser
{
  /// <summary> Beginning of game board data. </summary>
  inline static const char* StateStringBegin()
  {
    return "MOVE START";
  }
  /// <summary> End of game board data. </summary>
  inline static const char* StateStringEnd()
  {
    return "MOVE END";
  }

  /// <summary> Extract cell data from space-delimited input. </summary>
  inline static bool ExtractCell(const std::string& line, Cell* cell)
  {
    assert(!line.empty());
    std::stringstream ssCell(line);
    if (!ssCell.good()) { return false; }
    ssCell >> cell->location.x;
    if (!ssCell.good()) { return false; }
    ssCell >> cell->location.y;
    if (!ssCell.good()) { return false; }
    ssCell >> cell->value;
    return true;
  }

  /// <summary> Read next non-empty line from stream. </summary>
  inline static bool ReadNextLineNonEmpty(std::istream& stream, std::string* line)
  {
    while (stream.good())
    {
      std::getline(stream, *line);
      if (!line->empty())
      {
        return true;
      }
    }
    return false;
  }

  /// <summary> Construct game from state string. </summary>
  static bool Parse(const std::string& stateString, Board* board)
  {
    std::stringstream ssState(stateString);
    std::string line;
    *board = Board();

    // Extract game info.
    if (!ReadNextLineNonEmpty(ssState, &line)) { return false; }
    assert(StateStringBegin() == line);
    const Cell premadeMoveSentinel(Point(-1, -1), -1);
    Cell cell;
    for (;;)
    {
      if (!ReadNextLineNonEmpty(ssState, &line)) { return false; }
      if (StateStringEnd() == line) { break; }
      if (!ExtractCell(line, &cell)) { return false; }
      if (premadeMoveSentinel != cell)
      {
        board->PlayMove(cell);
      }
    }
    return true;
  }
};

}
using namespace sudokill;
}

#endif //_HPS_SUDOKILL_BOARD_PARSER_H_
