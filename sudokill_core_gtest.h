#ifndef _HPS_SUDOKILL_CORE_GTEST_H_
#define _HPS_SUDOKILL_CORE_GTEST_H_

#include "sudokill_core.h"
#include "gtest/gtest.h"


namespace _hps_sudokill_core_gtest_h
{
using namespace hps;

typedef sudokill::GenericBoard<9, 9, int> Board;
typedef sudokill::Point<int> Point;

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
      ASSERT_EQ(board.ValueAt(point), Board::Empty);
    }
  }
}

TEST(GenericBoard, PlayMove)
{
  Board board;
  Point p(0,0);
  ASSERT_EQ(board.ValueAt(p), Board::Empty);
  board.PlayMove(p, 1);
  ASSERT_EQ(board.ValueAt(p), 1);
}

TEST(GenericBoard, isValidMove)
{
  Board board;
  // Invalid Points:
  EXPECT_FALSE(board.isValidMove(Point(-1, -1), 1));
  EXPECT_FALSE(board.isValidMove(Point(10,10), 3));
  EXPECT_FALSE(board.isValidMove(Point(Board::MaxX,Board::MaxY), 9));
  // Invalid Values:
  EXPECT_FALSE(board.isValidMove(Point(3, 3), 0));
  EXPECT_FALSE(board.isValidMove(Point(3, 3), -1));
  EXPECT_FALSE(board.isValidMove(Point(3, 3), 10));
  // Valid Moves:
  EXPECT_TRUE(board.isValidMove(Point(0,0), 1));
  EXPECT_TRUE(board.isValidMove(Point(0,0), 9));
  EXPECT_TRUE(board.isValidMove(Point(Board::MaxX-1,Board::MaxY-1), 9));
}

}

#endif //_HPS_SUDOKILL_CORE_GTEST_H_
