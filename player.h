#ifndef _SUDOKILL_PLAYER_H
#define _SUDOKILL_PLAYER_H
#include "alphabetapruning.h"

namespace hps 
{
namespace sudokill
{

/// <summary> Player using alpha-beta pruning. </summary>
class AlphaBetaPlayer
{
  /// <summary> Try to estimate the number of reachable cells left. </summary>
  struct ShrinkPossibleMovesEvaluationFunc
  {
    inline int operator()(const Board& board) const
    {
      Board::MoveList moves;
      board.SudokuValidMoves(&moves);
      return -static_cast<int>(moves.size());
    }
  };
public:
  AlphaBetaPlayer();

  /// <summary> Return the next move for the player. </summary>
  void NextMove(const Board& board, Cell* move)
  {
    ShrinkPossibleMovesEvaluationFunc f;
    AlphaBetaPruning::Run(&params, &const_cast<Board&>(board), &f, move);
  }

private:
  AlphaBetaPruning::Params params;
};

}
}
#endif // _SUDOKILL_PLAYER_H
