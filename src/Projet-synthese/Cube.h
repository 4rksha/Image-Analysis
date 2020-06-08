#ifndef _CUBE_H
#define _CUBE_H
#include <vec.h>

class Cube
{
private:
Point pmax;
Point pmin;

public:
    Cube();
    ~Cube();
    void set_position(Point p_max,Point p_min);
    Point get_pmin();
    Point get_pmax();
    int calculate_collision(Cube c);
};



#endif