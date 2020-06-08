#ifndef _OBJET_H
#define _OBJET_H


#include "mesh.h"
#include "Cube.h"

class Objet
{
private:
   
public:
    Mesh mesh; 
    Cube cube;
    Objet();
    ~Objet();
};

Objet::Objet()
{
}

Objet::~Objet()
{
}


#endif