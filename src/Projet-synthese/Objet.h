#ifndef _OBJET_H
#define _OBJET_H


#include "mesh.h"
#include "Cube.h"
#include <vector>

class Objet
{
private:
   
public:
    Mesh mesh; 
    std::vector<Cube> cube;
    Objet();
    ~Objet();
    void setNumberCube(int n);
};




#endif