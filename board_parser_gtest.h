#ifndef _HPS_BOARD_PARSER_GTEST_H_
#define _HPS_BOARD_PARSER_GTEST_H_

#include "board_parser.h"
#include "gtest/gtest.h"

namespace _hps_board_parser_gtest_h_
{
using namespace hps;

TEST(Parser, Parse)
{
  const char* stateString =
    "\n"
    "MOVE START\n"
    "0 0 5\n"
    "0 3 4\n"
    "-1 -1 -1\n"
    "4 3 8\n"
    "MOVE END\n";
  Board board;
  ASSERT_TRUE(Parser::Parse(stateString, &board));
  EXPECT_EQ(board.GetOccupied()[0], Cell(Point(0, 0), 5));
  EXPECT_EQ(board.GetOccupied()[1], Cell(Point(0, 3), 4));
  EXPECT_EQ(board.GetOccupied()[2], Cell(Point(4, 3), 8));
  EXPECT_EQ(1, board.GetPlayerMovesCount());
  EXPECT_EQ(board.GetLastMove(), Cell(Point(4, 3), 8));
}

}

#endif //_HPS_BOARD_PARSER_GTEST_H_
