#ifndef CAMERA_H
#define CAMERA_H

#include "lost/math/Vec4.h"
#include "lost/math/Vec2.h"
#include "Ray.h"

// the projection plane is always situated at z=0
// the fov is determined by the dsitance between viewer and plane
struct Camera
{
    float distance; // distance between viewer and plane, determines fov
    lost::math::Vec2 screenSize; // width/height  
    
    inline void calculateRayForPixel(Ray& r, uint32_t x, uint32_t y)
    {
      lost::math::Vec3 px(-(screenSize.width/2) + x, -(screenSize.height/2) + y, 0);
      lost::math::Vec3 pos(0,0,distance);
      r.pos = pos;
      r.dir = px - pos;
      normalise(r.dir);
    }
};

#endif