#ifndef LIGHT_H
#define LIGHT_H

#include "lost/math/Vec4.h"
#include "lost/common/Color.h"

struct Light
{
    lost::math::Vec4    pos;
    lost::common::Color col;
};

#endif