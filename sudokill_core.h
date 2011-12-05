#ifndef _SUDOKILL_SUDOKILL_CORE_H
#define _SUDOKILL_SUDOKILL_CORE_H

#include <vector>
#include <algorithm>
#include <functional>
#include <assert.h>

namespace hps 
{

namespace sudokill
{

template<typename PointType>
struct Point
{
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

  bool operator<=(const Point& rhs)
  {
    return (x <= rhs.x && y <= rhs.y);
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

  enum {Empty = -1,};
  
  std::vector<Cell<PointType> > positions;
  
  /*
   <summary> This function puts the value at a point</summary>
   <parameters> A generic point Point and the value to be played.</parameters>
   */
  inline void PlayMove(Point<PointType> p, int value)
  {
    assert(p.x >= 0 && p.x <= MaxX);
    assert(p.y >= 0 && p.y <= MaxY);
    
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
  inline int ValueAt(Point<PointType> p)
  {
    assert(p.x >= 0 && p.x <= MaxX);
    assert(p.y >= 0 && p.y <= MaxY);
    int retVal = Empty;
    
    typename std::vector<Cell<PointType> >::iterator pos = std::find(positions.begin(),positions.end(),p);
    
    if(pos != positions.end())
    {
      retVal = (*pos).value;
    }
      
    return retVal;
  }
  
  bool ValidMove(Point<PointType> p, int value)
  {
    assert(p.x >=0 && p.x <= MaxX);
    assert(p.y >= 0 && p.y <= MaxY);
    
    return (isEmpty(p) &&
	    isValidRow(p, value) &&
	    isValidColumn(p, value) &&
	    isValidGrid(p, value));
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
    assert(pos == positions.end());
    return true;
  }

  bool isValidGrid(const Point<PointType>& p, int value)
  {
    assert(p.x >=0 && p.x <= MaxX);
    assert(p.y >=0 && p.y <= MaxY);
    int gridMinX, gridMaxX;
    int gridMinY, gridMaxY;
    
    int gridNumber = GridNumber(p);
    switch(gridNumber)
    {
    case 1:
      gridMinX = 0; gridMaxX = 2; gridMinY = 0; gridMaxY = 2; 
      break;
    case 2:
      gridMinX = 3; gridMaxX = 5; gridMinY = 0; gridMaxY = 2;
      break;
    case 3:
      gridMinX = 6; gridMaxX = 8; gridMinY = 0; gridMaxY = 2; 
      break;
    case 4:
      gridMinX = 0; gridMaxX = 2; gridMinY = 3; gridMaxY = 5;
      break;
    case 5:
      gridMinX = 3; gridMaxX = 5; gridMinY = 3; gridMaxY = 5; 
      break;
    case 6:
      gridMinX = 6; gridMaxX = 8; gridMinY = 3; gridMaxY = 5;
      break;
    case 7:
      gridMinX = 0; gridMaxX = 2; gridMinY = 6; gridMaxY = 8; 
      break;
    case 8:
      gridMinX = 3; gridMaxX = 5; gridMinY = 6; gridMaxY = 8;
      break;
    case 9:
      gridMinX = 6; gridMaxX = 8; gridMinY = 6; gridMaxY = 8; 
      break;
    }
    
    Point<PointType> pMin(gridMinX,gridMinY);
    Point<PointType> pMax(gridMaxX,gridMaxY);
    
    typename std::vector<Cell<PointType> >::iterator pos = positions.begin();
    for(; pos != positions.end(); ++pos)
    {
      if((*pos).location >= pMin && (*pos).location <= pMax && (*pos).value == value)
      {
	return false;
      }
    }
    
    return true;
    
  }

  int GridNumber(const Point<PointType>& p)
  {
    int x = (p.x)/3;
    int y = (p.y)/3;
    return (y*3 + x + 1);
  }

};

}
}


#endif // _SUDOKILL_SUDOKILL_CORE_H
