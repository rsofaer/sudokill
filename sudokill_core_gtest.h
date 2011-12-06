#ifndef _HPS_SUDOKILL_CORE_GTEST_H_
#define _HPS_SUDOKILL_CORE_GTEST_H_

#include "sudokill_core.h"
#include "gtest/gtest.h"


namespace _hps_sudokill_core_gtest_h
{
using namespace hps;

TEST(GenericBoard, initialization)
{

  Board board;
  ASSERT_EQ(Board::MaxX, 9);
  ASSERT_EQ(Board::MaxY, 9);
  
  for(int i = 0; i < Board::MaxX; i++)
  {
    for(int j = 0; j < Board::MaxY; j++)
    {
      Point point(i, j);
      ASSERT_EQ(board.ValueAt(point), static_cast<int>(Board::Empty));
    }
  }

  Board::MoveList moves;
  board.ValidMoves(&moves);
  EXPECT_EQ(9 * 9 * 9, moves.size());
}

TEST(GenericBoard, PlayMove)
{
  Board board;
  Point p(0,0);
  ASSERT_EQ(board.ValueAt(p), static_cast<int>(Board::Empty));
  board.PlayMove(p, 1);
  ASSERT_EQ(board.ValueAt(p), 1);
}

TEST(GenericBoard, ValidMoves)
{
  Board board;
  Board::MoveList moves;

  int maxMoves =  Board::MaxX * Board::MaxY * (Board::MaxValue - Board::MinValue + 1);
  
  ASSERT_TRUE(board.IsSudokuValidMove(Point(0,0),1));
  board.SudokuValidMoves(&moves);
  EXPECT_EQ(maxMoves, moves.size());

  moves.clear();
  
  board.ValidMoves(&moves);
  EXPECT_EQ(maxMoves, moves.size());
}

TEST(GenericBoard, IsValidBox)
{
  Board board;
  EXPECT_TRUE(board.IsValidBox(Point(1,0), 2));
  EXPECT_TRUE(board.IsValidBox(Point(0,8), 2));
  EXPECT_TRUE(board.IsValidBox(Point(5,5), 6));
  EXPECT_TRUE(board.IsValidBox(Point(2,2), 1));
  board.PlayMove(Point(0,0), 1);
  EXPECT_TRUE(board.IsValidBox(Point(1,0), 2));
  EXPECT_TRUE(board.IsValidBox(Point(0,8), 2));
  EXPECT_TRUE(board.IsValidBox(Point(5,5), 6));
  EXPECT_FALSE(board.IsValidBox(Point(2,2), 1));
  board.PlayMove(Point(0,3), 9);
  board.PlayMove(Point(4,3), 4);
  EXPECT_TRUE(board.IsValidBox(Point(1,0), 2));
  EXPECT_TRUE(board.IsValidBox(Point(0,8), 2));
  EXPECT_TRUE(board.IsValidBox(Point(5,5), 6));


}

TEST(GenericBoard, BoxNumber)
{
  Board board;
  //point(2,2), grid:1;
  //point(3,1), grid:2;
  //point(4,5), grid:5; 
  //point(6,3), grid:6;
  //point(5,4), grid:5
  /*
     0 1 2 3 4 5 6 7 8
     _________________
   0|    |     |     |
   1|  1 |  2  |  3  |
   2|----|-----|-----|
   3|    |     |     |
   4|  4 |  5  |  6  |
   5|----|-----|-----|
   6|    |     |     |
   7|  7 |  8  |  9  |
   8|----|-----|-----|
   */
  EXPECT_EQ(board.BoxNumber(Point(2,2)),1);

  EXPECT_EQ(board.BoxNumber(Point(3,1)),2);

  EXPECT_EQ(board.BoxNumber(Point(6,1)),3);

  EXPECT_EQ(board.BoxNumber(Point(2,5)),4);

  EXPECT_EQ(board.BoxNumber(Point(4,5)),5);
  EXPECT_EQ(board.BoxNumber(Point(5,4)),5);
  EXPECT_EQ(board.BoxNumber(Point(3,3)),5);

  EXPECT_EQ(board.BoxNumber(Point(6,3)),6);

  EXPECT_EQ(board.BoxNumber(Point(1,6)),7);

  EXPECT_EQ(board.BoxNumber(Point(4,7)),8);
  EXPECT_EQ(board.BoxNumber(Point(3,6)),8);

  EXPECT_EQ(board.BoxNumber(Point(6,6)),9);
  EXPECT_EQ(board.BoxNumber(Point(7,6)),9); 
}

TEST(GenericBoard, GetBoundingBox)
{
  Board board;
  Point pNW(-1,-1);
  Point pSE(9,9);
  board.GetBoundingBox(board.BoxNumber(Point(4,3)),&pNW,&pSE);
  EXPECT_EQ(pNW.x,3);
  EXPECT_EQ(pNW.y,3);
  EXPECT_EQ(pSE.x,5);
  EXPECT_EQ(pSE.y,5);
  
}

TEST(GenericBoard, IsValidMove)
{
  Board board;
  // Invalid Points:
  EXPECT_FALSE(board.IsValidMove(Point(-1, -1), 1));
  EXPECT_FALSE(board.IsValidMove(Point(10,10), 3));
  EXPECT_FALSE(board.IsValidMove(Point(Board::MaxX,Board::MaxY), 9));
  // Invalid Values:
  EXPECT_FALSE(board.IsValidMove(Point(3, 3), 0));
  EXPECT_FALSE(board.IsValidMove(Point(3, 3), -1));
  EXPECT_FALSE(board.IsValidMove(Point(3, 3), 10));
  // Valid Moves:
  EXPECT_TRUE(board.IsValidMove(Point(0,0), 1));
  EXPECT_TRUE(board.IsValidMove(Point(0,0), 9));
  EXPECT_TRUE(board.IsValidMove(Point(Board::MaxX-1,Board::MaxY-1), 9));
  
  board.PlayMove(Point(0,0), 1);

  // Sudoku invalid move:
  EXPECT_FALSE(board.IsValidMove(Point(0,0),2));
  EXPECT_FALSE(board.IsValidMove(Point(6,0),1));
  EXPECT_FALSE(board.IsValidMove(Point(0,6),1));
  EXPECT_FALSE(board.IsValidMove(Point(2,2),1));
  // Sudokill invalid move:
  EXPECT_FALSE(board.IsValidMove(Point(4,4),2)); // Same row or column required.
  EXPECT_FALSE(board.IsValidMove(Point(2,2),2)); // Same box doesn't count.
  
  EXPECT_TRUE(board.IsValidMove(Point(1,0), 2));
  EXPECT_TRUE(board.IsValidMove(Point(0,1), 2));
  EXPECT_TRUE(board.IsValidMove(Point(0,8), 2));
  
  // Now play out all the moves in the relevant row and column:
  board.PlayMove(Point(0,1), 2);
  board.PlayMove(Point(0,2), 3);
  board.PlayMove(Point(0,3), 4);
  board.PlayMove(Point(0,4), 5);
  board.PlayMove(Point(0,5), 6);
  board.PlayMove(Point(0,6), 7);
  board.PlayMove(Point(0,7), 8);
  // The Column is full, play across the board to get to the row:
  board.PlayMove(Point(8,7), 4);
  board.PlayMove(Point(8,8), 8);
  board.PlayMove(Point(7,8), 7);
  board.PlayMove(Point(6,8), 6);
  board.PlayMove(Point(5,8), 5);
  board.PlayMove(Point(4,8), 4);
  board.PlayMove(Point(3,8), 3);
  board.PlayMove(Point(2,8), 2);
  board.PlayMove(Point(1,8), 1);
  board.PlayMove(Point(0,8),9); // No moves left in column 0 and row 8.
  EXPECT_TRUE(board.IsValidMove(Point(2,2),9));
}
}

#endif //_HPS_SUDOKILL_CORE_GTEST_H_
