#ifndef BOX_HPP
#define BOX_HPP

#include "vec.h"
#include "mat.h"
#include "mesh.h"

class Box {
  public :
    Box() ;
    Box(const Point& pmin, const Point& pmax) ;

    bool collides3d(const Box& rhs) ;
    Mesh GetMesh();
    Point pmin, pmax ;
    Transform T ;
    Mesh meshcollider;
} ;

#endif
