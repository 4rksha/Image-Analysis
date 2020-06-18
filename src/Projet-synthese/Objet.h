#ifndef _OBJET_H
#define _OBJET_H


#include "mesh.h"
#include "box.h"
#include <vector>

class Objet
{   
public:
    Objet();
    ~Objet();
    void AddBox(Point pmin, Point pmax, Transform T);

    Mesh mesh; 
    std::vector<Box> boxes;
};




#endif