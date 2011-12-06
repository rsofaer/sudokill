#ifndef _SUDOKILL_PLAYER_H
#define _SUDOKILL_PLAYER_H
#include "rand_bound.h"

namespace hps 
{
namespace sudokill
{

  struct RandomPlayer
  {
    void NextMove(const Board& board, Cell* move)
    {
      Board::MoveList moves = board.ValidMoves();
      if(moves.size() == 0) // We have lost!
      {
        // Pick any empty cell.
      }else
      {
        move = &(moves[math::RandBound(moves.size())]);
      }
    }
  }

}
}
#endif // _SUDOKILL_PLAYER_H
