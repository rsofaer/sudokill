#ifndef _HPS_PLAYER_GTEST_H_
#define _HPS_PLAYER_GTEST_H_

#include "player.h"
#include "timer.h"
#include "gtest/gtest.h"

namespace _hps_player_gtest_h
{
using namespace hps;

TEST(AlphaBetaVsRandom, PlayerBattle)
{
  AlphaBetaPlayer abPlayer;
  RandomPlayer randomPlayer;

  double total_time = 0;

  Board board;
  std::string winner;
  std::string abv = "AlphaBeta";
  for (;;)
  {
    // Player 0.
    {
      Timer abTimer;
      Cell move;
      abPlayer.NextMove(board, &move);
      std::cout << "ALPHABETA player moved: " << move.value << ", at: (" << move.location.x << "," << move.location.y << ")" << std::endl;
      if (!board.IsValidMove(move.location, move.value))
      {
        winner = "Random";
        break;
      }
      board.PlayMove(move);
      total_time += abTimer.GetTime();
    }
    // Player 1.
    {
      Cell move;
      randomPlayer.NextMove(board, &move);
      std::cout << "RANDOM player moved: " << move.value << ", at: (" << move.location.x << "," << move.location.y << ")" << std::endl;
      if (!board.IsValidMove(move.location, move.value))
      {
        winner = abv;
        break;
      }
      board.PlayMove(move);
    }
    std::cout << "Played a round." << std::endl;
  }
  EXPECT_EQ(winner, abv);
  std::cout << total_time;
  EXPECT_TRUE(total_time < 115);
  std::cout << "The winner is " << winner << "." << std::endl;
}

}

#endif //_HPS_PLAYER_GTEST_H_
