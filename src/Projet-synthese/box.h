#ifndef BOX_HPP
#define BOX_HPP

#include "vec.h"
#include "mat.h"

class Box {
  public :
    Box() ;
    Box(const Point& pmin, const Point& pmax) ;

    bool collides3d(const Box& rhs) ;

    Point pmin, pmax ;
    Transform T ;
} ;

#endif
