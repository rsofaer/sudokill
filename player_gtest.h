#ifndef _HPS_PLAYER_GTEST_H_
#define _HPS_PLAYER_GTEST_H_

#include "player.h"
#include "gtest/gtest.h"

namespace _hps_player_gtest_h
{
using namespace hps;

TEST(AlphaBetaVsRandom, PlayerBattle)
{
  AlphaBetaPlayer abPlayer;
  RandomPlayer randomPlayer;

  Board board;
  std::string winner;
  for (;;)
  {
    // Player 0.
    {
      Cell move;
      abPlayer.NextMove(board, &move);
      if (!board.IsValidMove(move.location, move.value))
      {
        winner = "Random";
        break;
      }
      board.PlayMove(move);
    }
    // Player 1.
    {
      Cell move;
      randomPlayer.NextMove(board, &move);
      if (!board.IsValidMove(move.location, move.value))
      {
        winner = "AlphaBeta";
        break;
      }
      board.PlayMove(move);
    }
  }
  std::cout << "The winner is " << winner << "." << std::endl;
}

}

#endif //_HPS_PLAYER_GTEST_H_
