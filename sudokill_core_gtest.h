#ifndef _HPS_SUDOKILL_CORE_GTEST_H_
#define _HPS_SUDOKILL_CORE_GTEST_H_

#include "sudokill_core.h"
#include "gtest/gtest.h"


namespace _hps_sudokill_core_gtest_h
{
using namespace hps;

typedef sudokill::GenericBoard<9, 9, int> Board;

TEST(GenericBoard, initialization)
{

  Board board;
  ASSERT_EQ(Board::MaxX, 9);
  ASSERT_EQ(Board::MaxY, 9);
  
  for(int i = 0; i <= Board::MaxX; i++)
  {
    for(int j = 0; j <= Board::MaxY; j++)
    {
      sudokill::Point<int> point(i, j);
      ASSERT_EQ(board.ValueAt(point), Board::Empty);
    }
  }
}
}

#endif //_HPS_SUDOKILL_CORE_GTEST_H_
