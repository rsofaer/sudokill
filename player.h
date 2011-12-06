#ifndef _SUDOKILL_PLAYER_H
#define _SUDOKILL_PLAYER_H
#include "rand_bound.h"
#include "alphabetapruning.h"

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

/// <summary> Player using alpha-beta pruning. </summary>
  class AlphaBetaPlayer
  {
    /// <summary> Try to estimate the number of reachable cells left. </summary>
    struct ShrinkPossibleMovesEvaluationFunc
    {
      inline int operator()(const Board& board) const
      {
        -board.SudokuValidMoves();
      }
    };
  public:
    AlphaBetaPlayer();

    /// <summary> Return the next move for the player. </summary>
    void NextMove(const Board& board, Cell* move)
    {
      AlphaBetaPruning::Run(&params, &board,
                            ShrinkPossibleMovesEvaluationFunc(),
                            &move);
    }

  private:
    AlphaBetaPruning::Params params;
  };

}
}
#endif // _SUDOKILL_PLAYER_H
