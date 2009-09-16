#ifndef LIGHT_H
#define LIGHT_H

#include "lost/math/Vec3.h"
#include "lost/common/Color.h"

struct Light
{
    lost::math::Vec3    pos;
    lost::common::Color col;
    float               intensity;
    
    Light()
    :pos(3,3,3), col(1,1,1), intensity(1)
    {
    }
};

#endif