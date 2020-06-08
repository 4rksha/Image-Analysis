#include "Cube.h"

void Cube::set_position(Point p_max,Point p_min)
{
    pmax=p_max;
    pmin=p_min;
}

Point Cube::get_pmax()
{
    return pmax;
}

Point Cube::get_pmin()
{
    return pmin;
}

Cube::Cube()
{

}

Cube::~Cube()
{
}

int Cube::calculate_collision(Cube c)
{
    return 0;
}