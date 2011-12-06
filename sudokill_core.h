#ifndef _SUDOKILL_SUDOKILL_CORE_H
#define _SUDOKILL_SUDOKILL_CORE_H

#include <vector>
#include <algorithm>
#include <functional>
#include <assert.h>
#include <iostream>

namespace hps 
{

namespace sudokill
{

template<typename PointType>
struct Point
{
  Point()
  {}
  Point(const PointType x_,
        const PointType y_)
  : x(x_),
    y(y_)
  {}
  
  PointType x;
  PointType y;
  
  Point operator+(const Point& rhs) const
  {
    return Point(x+rhs.x, y+rhs.y);
  }
  
  bool operator==(const Point& rhs) const
  {
    return (x == rhs.x && y == rhs.y);
  }
};

template <typename PointType>
struct Cell
{
  Cell(Point<PointType> location_,
       int value_)
  : location(location_),
    value(value_)
  {}

  // RJS 5/12:  I feel like I shouldn't be doing this.  We use it in ValueAt.
  bool operator==(const Point<PointType>& rhs) const
  {
    return (location == rhs);
  }

  Point<PointType> location;
  int value;
};
  
template<int MaxX_, int MaxY_, typename PointType>
struct GenericBoard
{
  // max x value.
  enum {MaxX = MaxX_,};
  // max y value.
  enum {MaxY = MaxY_,};

  enum {Empty = -1,
        MinValue = 1,
        MaxValue = 9};

  typedef Point<PointType> point_type;
  typedef Cell<PointType> move_type;
  typedef std::vector<move_type> move_list_type;
  
  std::vector<Cell<PointType> > positions;
  
  /*
   <summary> This function puts the value at a point</summary>
   <parameters> A generic point Point and the value to be played.</parameters>
   */
  inline void PlayMove(Point<PointType> p, int value)
  {
    assert(p.x >= 0 && p.x < MaxX);
    assert(p.y >= 0 && p.y < MaxY);
    assert(isValidMove(p, value));
    
    typename std::vector<Cell<PointType> >::iterator pos = std::find(positions.begin(),positions.end(),p);
    
    //assert if position occupied.
    assert(pos == positions.end());
    
    //position is set by creating an object of type Cell.
    Cell<PointType> c(p,value);
    positions.push_back(c);
  }
  
  /*
   <summary> This function is used to Undo the last move.</summary>
   */
  inline void Undo()
  {
    assert(positions.size() > 0);
    // the last value played is put at the back.
    positions.pop_back();
  }
  
  /*
   <summary> This function returns the value at a point</summary>
   <parameters> A generic point Point</parameters>
   <returns> Value at point p or -1 if the location is empty.</returns>
   */

  inline int ValueAt(const Point<PointType>& p)
  {
    assert(p.x >= 0 && p.x < MaxX);
    assert(p.y >= 0 && p.y < MaxY);
    int retVal = Empty;
    
    typename std::vector<Cell<PointType> >::iterator pos = std::find(positions.begin(),positions.end(),p);
    
    if(pos != positions.end())
    {
      retVal = (*pos).value;
    }
      
    return retVal;
  }
  
  bool isValidMove(const Point<PointType>& p, int value)
  {
    return ((p.x >=0 && p.x < MaxX) &&
            (p.y >= 0 && p.y < MaxY) &&
            isEmpty(p) &&
            isValidValue(value) &&
	          isValidRow(p, value) &&
	          isValidColumn(p, value) &&
	          isValidBox(p, value)); //&&
            //isSameRowOrColumnIfPossible(p);
  }

  bool isSameRowOrColumnIfPossible(const Point<PointType>& p)
  {
    if(positions.size() == 0)
    {
      // Allow any move if this is the first move.
      return true;
    } else
    {
      Point<PointType> lastPlay = positions.back().location;
      //std::cout << "Last Play: (" << lastPlay.x << "," << lastPlay.y << ")\n";
      //std::cout << "This Play: (" << p.x << "," << p.y << ")\n";
      if( (lastPlay.x == p.x) || (lastPlay.y == p.y)){
        return true;
      } else
      {
        // This is wrong.  We must check that there are possible moves
        // within the row/column.
        return false;
      }
    }
  }

  bool isValidValue(int value)
  {
    return value >= MinValue && value <= MaxValue;
  }

  bool isValidRow(const Point<PointType>& p, int value)
  {
    typename std::vector<Cell<PointType> >::iterator pos = positions.begin();
    for(; pos != positions.end();++pos)
    {
      if((*pos).location.y == p.y && (*pos).value == value)
      {
        return false;
      }
    }
    return true;
  }
  
  bool isValidColumn(const Point<PointType>& p, int value)
  {
    typename std::vector<Cell<PointType> >::iterator pos = positions.begin();
    for(; pos != positions.end();++pos)
    {
      if((*pos).location.x == p.x && (*pos).value == value)
      {
        return false;
      }
    }
    return true;
  }

  bool isEmpty(const Point<PointType>& p)
  {
    typename std::vector<Cell<PointType> >::iterator pos = std::find(positions.begin(), positions.end(), p);
    if(pos != positions.end())
    {
      return false;
    }
    return true;
  }

  bool isWithinBox(const Point<PointType>& NW, const Point<PointType>& SE, const Point<PointType>& p)
  {
    return (p.x >= NW.x) && (p.x >= NW.y) && (p.x <= SE.x) && (p.y <= SE.y); 
  }

  void getBoundingBox(int boxNumber, 
		      Point<PointType>* pNW,
		      Point<PointType>* pSE)
  {
    int boxMinX, boxMaxX;
    int boxMinY, boxMaxY;
    
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
    }
    pNW->x = boxMinX; pNW->y = boxMinY;
    pSE->x = boxMaxX; pSE->y = boxMaxY;
    std::cout << "boxminx: " << boxMinX << std::endl;
    std::cout << "boxminy: " << boxMinY << std::endl;
    std::cout << "boxmaxx: " << boxMaxX << std::endl;
    std::cout << "boxmaxy: " << boxMaxY << std::endl;
  }
  bool isValidBox(const Point<PointType>& p, int value)
  {
    assert(p.x >=0 && p.x < MaxX);
    assert(p.y >=0 && p.y < MaxY);
    std::cout << "value: " << value <<std::endl;
    int boxNumber = BoxNumber(p);
    std::cout << "box Number: " << boxNumber <<std::endl;
    Point<PointType> pNW;
    Point<PointType> pSE;
    getBoundingBox(boxNumber,&pNW,&pSE);

    typename std::vector<Cell<PointType> >::iterator pos = positions.begin();
    for(; pos != positions.end(); ++pos)
    {
      if(!isWithinBox(pNW, pSE, pos->location) && pos->value == value )
      {
        return false;
      }
    }
    
    return true;
    
  }

  int BoxNumber(const Point<PointType>& p)
  {
    // point(3,1), grid:1
    // point(4,5), grid:5
    // point(6,3), grid:6
    int x = (p.x)/3;
    int y = (p.y)/3;
    return (y*3 + x + 1);
  }

  void ValidMoves(std::vector<Cell<PointType> >* moveBuffer)
  {
    // This is the dumbest code I've ever written.
    // RJS 5/12
    for(int i = 0; i < MaxX; i++)
    {
      for(int j = 0; j < MaxY; j++)
      {
        Point<PointType> p(i,j);
        for(int v = MinValue; v < MinValue; v++)
        {
          if(isValidMove(p, v))
          {
            Cell<PointType> c(p,v);
            moveBuffer->push_back(c);
          }
        }
      }
    }
  }

};

}
}


#endif // _SUDOKILL_SUDOKILL_CORE_H
