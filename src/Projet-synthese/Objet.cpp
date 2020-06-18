#include "Objet.h"

void Objet::AddBox(Point pmin, Point pmax)
{   
    boxes.push_back(Box(pmin, pmax));
}
Objet::Objet()
{
}

Objet::~Objet()
{
}