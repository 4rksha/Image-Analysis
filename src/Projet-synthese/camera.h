#ifndef _CAMERA_H
#define _CAMERA_H

#include "orbiter.h"
#include "vec.h"
#include "mat.h"

class Camera : public Orbiter
{
    public:
        void lookAt(const Point & center,const float size);
    private:
        Transform camT;

};
#endif