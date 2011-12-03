#ifndef _SUDOKILL_SUDOKILL_CORE_H
#define _SUDOKILL_SUDOKILL_CORE_H

#include <vector>
#include <algorithm>
#include <functional>

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
  
  Point operator+(const Point& lhs, const Point& rhs) const
  {
    return Point(lhs.x+rhs.x, lhs.y+rhs.y);
  }
  
  bool operator==(const Point& lhs, const Point& rhs) const
  {
    return (x == rhs.x && y == rhs.y);
  }
  
  bool operator<=(const Point& lhs, const Point& rhs)
  {
    return (lhs.x <= rhs.x && lhs.y <= rhs.y);
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
  
  typedef std::vector<Cell<PointType> > Positions;
  Positions positions;
  
  /*
   <summary> This function puts the value at a point</summary>
   <parameters> A generic point Point and the value to be played.</parameters>
   */
  inline void PlayMove(Point<PointType> p, int value)
  {
    assert(p.x >= 0 && p.x <= MaxX);
    assert(p.y >= 0 && p.y <= MaxY);
    
    typename Positions::iterator pos = std::find(positions.begin(),positions.end(),p);
    
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
    int retVal = -1;
    
    typename Positions::iterator pos = std::find(positions.begin(),positions.end(),p);
    
    if(positions != positions.end())
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
	    isValidGrid(p, value);
  }

  bool isEmpty(const Point<PointType>& p);
  bool isValidRow(const Point<PointType>& p, int value);
  bool isValidColumn(const Point<PointType>& p, int value);
  bool isValidGrid(const Point<PointType>& p, int value);
  //{
    
    //TODO:
    // check if row is valid
    // check if column is valid.
    // check if grid is valid.
  //}
};

template <typename PointType>
typedef GenericBoard<int,int,PointType> Board;

}
}


#endif // _SUDOKILL_SUDOKILL_CORE_H
