#include "Objet.h"

void Objet::setNumberCube(int n){
    for(int i=0;i<n;i++)
    {
        Cube c;
        cube.push_back(c);
    }
}

Objet::Objet()
{
}

Objet::~Objet()
{
}