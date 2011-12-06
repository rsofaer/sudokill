#ifndef _SUDOKILL_SUDOKILL_CORE_H
#define _SUDOKILL_SUDOKILL_CORE_H

#include <vector>
#include <algorithm>
#include <functional>
#include <assert.h>
#include <iostream>
#include "rand_bound.h"

namespace hps 
{
namespace sudokill
{

/// <summary> A point is a 2d location. </summary>
struct Point
{
  Point() : x(), y() {}
  Point(const int x_, const int y_) : x(x_), y(y_) {}
  
  int x;
  int y;
  
  inline const Point operator+(const Point& rhs) const
  {
    return Point(x+rhs.x, y+rhs.y);
  }

  inline const Point operator-(const Point& rhs) const
  {
    return Point(x-rhs.x, y-rhs.y);
  }
  
  inline bool operator==(const Point& rhs) const
  {
    return (x == rhs.x && y == rhs.y);
  }
};

/// <summary> A cell is a board location that contains a value. </summary>
struct Cell
{
  Cell() : location(), value() {}
  Cell(const Point& location_, int value_)
  : location(location_),
    value(value_)
  {}

  inline bool operator==(const Cell& rhs) const
  {
    return (location == rhs.location) && (value == rhs.value);
  }
  inline bool operator!=(const Cell& rhs) const
  {
    return !(*this == rhs);
  }

  Point location;
  int value;
};
  
/// <summary> A board is a grid of cells. </summary>
template<int MaxX_, int MaxY_>
class GenericBoard
{
public:
  // max x value.
  enum {MaxX = MaxX_,};
  // max y value.
  enum {MaxY = MaxY_,};

  enum { Empty = 0, };
  enum { MinValue = 1, };
  enum { MaxValue = 9, };

  typedef std::vector<Cell> MoveList;

  GenericBoard() : positions(), playerMoveCount(0) {}
  /// <summary> Initialize with a list of preset cells. </summary>
  explicit GenericBoard(const MoveList& preset)
  : positions(preset),
    playerMoveCount(0)
  {}
  
  /// <summary> Predicate to test that a move is at a given point. </summary>
  struct MoveMatchesPoint
  {
    MoveMatchesPoint(const Point* p_) : p(p_) {}
    inline bool operator()(const Cell& move) { return move.location == *p; }
    const Point* p;
  };

  /// <summary> Test if the given board location is occupied. </summary>
  inline bool Occupied(const Point& p) const
  {
    return positions.end() != std::find_if(positions.begin(),
                                           positions.end(),
                                           MoveMatchesPoint(&p));
  }
  
  /// <summary> This function puts the value at a point</summary>
  inline void PlayMove(const Point& p, int value)
  {
    assert(p.x >= 0 && p.x < MaxX);
    assert(p.y >= 0 && p.y < MaxY);
    assert(IsValidMove(p, value));
    assert(playerMoveCount >= 0);
    
    //assert if position occupied.
    assert(!Occupied(p));
    
    //position is set by creating an object of type Cell.
    positions.push_back(Cell(p,value));
    ++playerMoveCount;
  }

  inline void PlayMove(const Cell& c)
  {
    //PrintBoard();
    //std::cout << "Playing value: " << c.value << " at: (" << c.location.x << "," << c.location.y << ")" << std::endl;
    PlayMove(c.location, c.value);
  }
  
  /// <summary> This function is used to Undo the last move.</summary>
  inline void Undo()
  {
    assert(!positions.empty());
    // the last value played is put at the back.
    positions.pop_back();
    --playerMoveCount;
  }
  
  /// <summary> This function returns the value at a point</summary>
  /// <parameters> A generic point Point</parameters>
  /// <returns> Value at point p or -1 if the location is empty.</returns>
  inline int ValueAt(const Point& p) const
  {
    assert(p.x >= 0 && p.x < MaxX);
    assert(p.y >= 0 && p.y < MaxY);
    
    typedef typename MoveList::const_iterator ConstPosition;
    const ConstPosition positionsEnd = positions.end();
    ConstPosition pos = std::find_if(positions.begin(),
                                     positionsEnd,
                                     MoveMatchesPoint(&p));
    if(pos != positionsEnd)
    {
      return (*pos).value;
    }
    else
    {
      return Empty;
    }
  }
  
  /// <summary> Check if the move is valid by the Sudokill rules. </summary>
  bool IsValidMove(const Point& p, int value) const
  {
    if(!IsSudokuValidMove(p, value))
    {
      return false;
    }

    if(playerMoveCount > 0)
    {
      const Point& lastPlay = positions.back().location;
      if(lastPlay.x == p.x || lastPlay.y == p.y)
      {
        return true;
      }

      Point testPoint = lastPlay;

      for(int x = 0; x < MaxX; x++)
      {
        testPoint.x = x;
        if(!Occupied(testPoint))
        {
          return false;
        }
      }

      testPoint = lastPlay;
      for(int y = 0; y < MaxY; y++)
      {
        testPoint.y = y;
        if(!Occupied(testPoint))
        {
          return false;
        }
      }
    }
    return true;
  }

  /// <summary> Check if the move is valid by the Sudokill rules. </summary>
  bool IsValidMove(const Cell& cell) const
  {
    return IsValidMove(cell.location, cell.value);
  }

  /// <summary> Check if the move is valid by Sudoku rules. </summary>
  bool IsSudokuValidMove(const Point& p, int value) const
  {
    return ((p.x >=0 && p.x < MaxX) &&
            (p.y >= 0 && p.y < MaxY) &&
            !Occupied(p) &&
            IsValidValue(value) &&
            IsValidRow(p, value) &&
            IsValidColumn(p, value) &&
            IsValidBox(p, value));
  }

  void ValidMoves(MoveList* moveBuffer) const
  {
    assert(moveBuffer);
    moveBuffer->clear();

    if(playerMoveCount > 0)
    {
      int unoccupiedFound = 0;
      const Point& lastPlay = positions.back().location;

      Point testPoint = lastPlay;

      for(int x = 0; x < MaxX; x++)
      {
        testPoint.x = x;
        if(!Occupied(testPoint))
        {
          ++unoccupiedFound;
          for(int value = 1; value <= MaxValue; value++)
          {
            if(IsSudokuValidMove(testPoint, value))
            {
              moveBuffer->push_back(Cell(testPoint,value));
            }
          }
        }
      }

      testPoint = lastPlay;
      for(int y = 0; y < MaxY; y++)
      {
        testPoint.y = y;
        if(!Occupied(testPoint))
        {
          ++unoccupiedFound;
          for(int value = 1; value <= MaxValue; value++)
          {
            if(IsSudokuValidMove(testPoint, value))
            {
              moveBuffer->push_back(Cell(testPoint,value));
            }
          }
        }
      }
      // When there are unoccupied spaces that are not Sudoku-valid, then we
      // have to roll with it.
      if (0 != unoccupiedFound)
      {
        return;
      }
    }
    if(moveBuffer->empty())
    {
      SudokuValidMoves(moveBuffer);
    }
  }

  /// <summary> Verify that the value is in bounds. </summary>
  bool IsValidValue(int value) const
  {
    return value >= MinValue && value <= MaxValue;
  }

  /// <summary> Verify that the row conforms to Sudoku rules. </sumary>
  bool IsValidRow(const Point& p, int value) const
  {
    typename MoveList::const_iterator pos = positions.begin();
    const MoveList::const_iterator positionsEnd = positions.end();
    for(; pos != positionsEnd; ++pos)
    {
      if((*pos).location.y == p.y && (*pos).value == value)
      {
        //std::cout << "isValidRow returned false" << std::endl;
        return false;
      }
    }
    //std::cout << "isValidRow returned true" <<std::endl;
    return true;
  }
  
  /// <summary> Verify that the column conforms to Sudoku rules. </sumary>
  bool IsValidColumn(const Point& p, int value) const
  {
    typename MoveList::const_iterator pos = positions.begin();
    const MoveList::const_iterator positionsEnd = positions.end();
    for(; pos != positionsEnd; ++pos)
    {
      if((*pos).location.x == p.x && (*pos).value == value)
      {
        //std::cout << "isValidColumn returned false" <<std::endl;
        return false;
      }
    }
    //std::cout << "isValidColumn returned true" <<std::endl;
    return true;
  }

  /// <summary> Verify that the point is within the bounding box. </summary>
  inline static bool IsWithinBox(const Point& NW, const Point& SE, const Point& p)
  {
    return (p.x >= NW.x) && (p.y >= NW.y) && (p.x <= SE.x) && (p.y <= SE.y); 
  }

#pragma warning(push)
#pragma warning(disable: 4701) // Uninitialized local variable.
  void GetBoundingBox(int boxNumber, Point* pNW, Point* pSE) const
  {
    assert(pNW && pSE);

    int& boxMinX = pNW->x;
    int& boxMaxX = pSE->x;
    int& boxMinY = pNW->y;
    int& boxMaxY = pSE->y;
    
    switch(boxNumber)
    {
    case 1:
      boxMinX = 0; boxMaxX = 2; boxMinY = 0; boxMaxY = 2; 
      break;
    case 2:
      boxMinX = 3; boxMaxX = 5; boxMinY = 0; boxMaxY = 2;
      break;
    case 3:
      boxMinX = 6; boxMaxX = 8; boxMinY = 0; boxMaxY = 2; 
      break;
    case 4:
      boxMinX = 0; boxMaxX = 2; boxMinY = 3; boxMaxY = 5;
      break;
    case 5:
      boxMinX = 3; boxMaxX = 5; boxMinY = 3; boxMaxY = 5; 
      break;
    case 6:
      boxMinX = 6; boxMaxX = 8; boxMinY = 3; boxMaxY = 5;
      break;
    case 7:
      boxMinX = 0; boxMaxX = 2; boxMinY = 6; boxMaxY = 8; 
      break;
    case 8:
      boxMinX = 3; boxMaxX = 5; boxMinY = 6; boxMaxY = 8;
      break;
    case 9:
      boxMinX = 6; boxMaxX = 8; boxMinY = 6; boxMaxY = 8; 
      break;
    default:
      assert(false && "Should not reach here.");
    }
    //std::cout << "boxminx: " << pNW->x << std::endl;
    //std::cout << "boxminy: " << pNW->y << std::endl;
    //std::cout << "boxmaxx: " << pSE->x << std::endl;
    //std::cout << "boxmaxy: " << pSE->y << std::endl;
  }
#pragma warning(pop)

  /// <summary> Verify 3x3 box containing p is valid by Sudoku rules. </summary>
  bool IsValidBox(const Point& p, int value) const
  {
    assert(p.x >=0 && p.x < MaxX);
    assert(p.y >=0 && p.y < MaxY);
    //std::cout << "value: " << value <<std::endl;
    const int boxNumber = BoxNumber(p);
    //std::cout << "box Number: " << boxNumber <<std::endl;
    Point pNW;
    Point pSE;
    GetBoundingBox(boxNumber,&pNW,&pSE);

    typename MoveList::const_iterator pos = positions.begin();
    const MoveList::const_iterator positionsEnd = positions.end();
    for(; pos != positionsEnd; ++pos)
    {
      if(IsWithinBox(pNW, pSE, pos->location) && pos->value == value )
      {
        return false;
      }
    }
    return true;
  }

  /// <summary> Get Sudoku 3x3 box index. </summary>
  int BoxNumber(const Point& p) const
  {
    int x = (p.x)/3;
    int y = (p.y)/3;
    return (y*3 + x + 1);
  }

  /// <summary> Get the list of valid Sudoku moves from the
  ///   current state.
  /// </summary>
  void SudokuValidMoves(MoveList* moveBuffer) const
  {
    assert(moveBuffer);
    // This is the dumbest code I've ever written.
    // Memoizing it would bring a pretty big performance benefit
    // Cachebust on PlayMove and Undo
    // RJS 5/12
    for(int i = 0; i < MaxX; i++)
    {
      for(int j = 0; j < MaxY; j++)
      {
        Point p(i,j);
        for(int v = MinValue; v <= MaxValue; v++)
        {
          if(IsSudokuValidMove(p, v))
          {
            moveBuffer->push_back(Cell(p,v));
          }
        }
      }
    }
  }

  /// <summary> Find any unoccupied cell and make a move for it. </summary>
  void RandomEmptyCell(Cell* c) const
  {
    Point p(0,0);
    for(int x = 0; x < MaxX; x++)
    {
      for(int y = 0; y < MaxY; y++)
      {
        p.x = x;
        p.y = y;
        if(!Occupied(p))
        {
          c->location = p;
          c->value = math::RandBound(MaxValue) + 1;
          return;
        }
      }
    }
  }

  void PrintBoard() const
  {
    std::cout << "  012 345 678  " << std::endl;
    std::cout << "  ____________ " << std::endl;
    std::cout << "0|" << ValueAt(Point(0,0)) << ValueAt(Point(1,0)) << ValueAt(Point(2,0))<<"|" << ValueAt(Point(3,0)) << ValueAt(Point(4,0)) << ValueAt(Point(5,0)) <<"|"<< ValueAt(Point(6,0)) << ValueAt(Point(7,0)) << ValueAt(Point(8,0)) << "|" << std::endl;
    std::cout << "1|" << ValueAt(Point(0,1)) << ValueAt(Point(1,1)) << ValueAt(Point(2,1))<<"|" << ValueAt(Point(3,1)) << ValueAt(Point(4,1)) << ValueAt(Point(5,1)) <<"|"<< ValueAt(Point(6,1)) << ValueAt(Point(7,1)) << ValueAt(Point(8,1)) << "|" << std::endl;
    std::cout << "2|" << ValueAt(Point(0,2)) << ValueAt(Point(1,2)) << ValueAt(Point(2,2))<<"|" << ValueAt(Point(3,2)) << ValueAt(Point(4,2)) << ValueAt(Point(5,2)) <<"|"<< ValueAt(Point(6,2)) << ValueAt(Point(7,2)) << ValueAt(Point(8,2)) << "|" << std::endl;
    std::cout << " |---|---|---| " << std::endl;
    std::cout << "3|" << ValueAt(Point(0,3)) << ValueAt(Point(1,3)) << ValueAt(Point(2,3))<<"|" << ValueAt(Point(3,3)) << ValueAt(Point(4,3)) << ValueAt(Point(5,3)) <<"|"<< ValueAt(Point(6,3)) << ValueAt(Point(7,3)) << ValueAt(Point(8,3)) << "|" << std::endl;
    std::cout << "4|" << ValueAt(Point(0,4)) << ValueAt(Point(1,4)) << ValueAt(Point(2,4))<<"|" << ValueAt(Point(3,4)) << ValueAt(Point(4,4)) << ValueAt(Point(5,4)) <<"|"<< ValueAt(Point(6,4)) << ValueAt(Point(7,4)) << ValueAt(Point(8,4)) << "|" << std::endl;
    std::cout << "5|" << ValueAt(Point(0,5)) << ValueAt(Point(1,5)) << ValueAt(Point(2,5))<<"|" << ValueAt(Point(3,5)) << ValueAt(Point(4,5)) << ValueAt(Point(5,5)) <<"|"<< ValueAt(Point(6,5)) << ValueAt(Point(7,5)) << ValueAt(Point(8,5)) << "|" << std::endl;
    std::cout << " |---|---|---| " << std::endl;
    std::cout << "6|" << ValueAt(Point(0,6)) << ValueAt(Point(1,6)) << ValueAt(Point(2,6))<<"|" << ValueAt(Point(3,6)) << ValueAt(Point(4,6)) << ValueAt(Point(5,6)) <<"|"<< ValueAt(Point(6,6)) << ValueAt(Point(7,6)) << ValueAt(Point(8,6)) << "|" << std::endl;
    std::cout << "7|" << ValueAt(Point(0,7)) << ValueAt(Point(1,7)) << ValueAt(Point(2,7))<<"|" << ValueAt(Point(3,7)) << ValueAt(Point(4,7)) << ValueAt(Point(5,7)) <<"|"<< ValueAt(Point(6,7)) << ValueAt(Point(7,7)) << ValueAt(Point(8,7)) << "|" << std::endl;
    std::cout << "8|" << ValueAt(Point(0,8)) << ValueAt(Point(1,8)) << ValueAt(Point(2,8))<<"|" << ValueAt(Point(3,8)) << ValueAt(Point(4,8)) << ValueAt(Point(5,8)) <<"|"<< ValueAt(Point(6,8)) << ValueAt(Point(7,8)) << ValueAt(Point(8,8)) << "|" << std::endl;
    std::cout << " |---|---|---  " << std::endl;
  }

  /// <summary> Query number of moves made by players. </summary>
  inline int NumberPlayerMoves() const
  {
    return playerMoveCount;
  }

  /// <summary> Query the cells occupied. </summary>
  inline const MoveList& GetOccupied() const
  {
    return positions;
  }

  /// <summary> Query the last move. </summary>
  inline const Cell& LastMove() const
  {
    assert(playerMoveCount > 0);
    return positions.back();
  }

private:
  /// <summary> List of occupied board cells. </summary>
  MoveList positions;
  /// <summary> Number of moves made by players. </summary>
  int playerMoveCount;
};

typedef sudokill::GenericBoard<9, 9> Board;

inline void AnyPlyWillDo(const Board* board, Cell* cell)
{
  board->RandomEmptyCell(cell);
}

} // end ns sudokill
using namespace sudokill;
} // end ns hps


#endif // _SUDOKILL_SUDOKILL_CORE_H
