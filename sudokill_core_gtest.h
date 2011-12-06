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
  
  for(int i = 0; i <= Board::MaxX; i++)
  {
    for(int j = 0; j <= Board::MaxY; j++)
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
}

#endif //_HPS_SUDOKILL_CORE_GTEST_H_
