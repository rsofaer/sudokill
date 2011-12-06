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
    Board::MoveList moves;
    board.ValidMoves(&moves);
    if(moves.size() == 0) // We have lost!
    {
      // Pick any empty cell.
      board.RandomEmptyCell(move);
    }
    else
    {
      *move = moves[math::RandBound(moves.size())];
    }
  }
};

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
  /// <summary> Return the next move for the player. </summary>
  void NextMove(const Board& board, Cell* move)
  {
    // Pick a random spot if it's early in the game.
    Board::MoveList sudokuMoves;
    board.SudokuValidMoves(&sudokuMoves);
    std::cout << "There are " << sudokuMoves.size() << " sudoku valid moves remaining." << std::endl;
    if(sudokuMoves.size() > 30)
    {
      RandomPlayer rand;
      rand.NextMove(board, move);
    }else
    {
      params.maxDepth = 10;
      ShrinkPossibleMovesEvaluationFunc f;
      AlphaBetaPruning::Run(&params, &const_cast<Board&>(board), &f, move);
    }
  }

private:
  AlphaBetaPruning::Params params;
};

}
using namespace sudokill;
}
#endif // _SUDOKILL_PLAYER_H
