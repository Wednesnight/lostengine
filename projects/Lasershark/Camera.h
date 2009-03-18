#ifndef CAMERA_H
#define CAMERA_H

#include "lost/math/Vec4.h"
#include "lost/math/Vec2.h"

struct Camera
{
    lost::math::Vec3 pos;
    lost::math::Vec3 viewDirection; // normalised
    lost::math::Vec2 screenSize; // width/height
    // distance of viewer and screen plane is defined by pos.z
    // this should also define the fov in both directions
        
    Camera()
    : pos(0,0,-5), viewDirection(0,0,1), screenSize(512, 512)
    {
    }
};

#endif