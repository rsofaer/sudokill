#include "sudokill_core.h"
namespace hps
{
namespace sudokill
{

bool Board::isValidRow(const Point<PointType>& p, int value)
{
  typename Position::iterator pos = positions.begin();
  for(; pos != positions.end();++pos)
  {
    if((*pos).location.y == p.y && (*pos).value == value)
    {
      return false;
    }
  }
  return true;
}

bool Board::isValidColumn(const Point<PointType>& p, int value)
{
  typename Position::iterator pos = positions.begin();
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
  typename Position::iterator pos = std::find(positions.begin(), positions.end(), p);
  assert(pos == positions.end());
  return true;
}

bool Board::isValidGrid(const Point<PointType>& p, int value)
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

  typename Position::iterator pos = positions.begin();
  for(; pos != positions.end(); ++pos)
  {
    if((*pos).location >= pMin && (*pos).location <= pMax && (*pos).value == value)
    {
      return false;
    }
  }

  return true;

}

int Board::GridNumber(const Point<PointType>& p)
{
  int x = (p.x)/3;
  int y = (p.y)/3;
  return (y*3 + x + 1);
}

}
}
