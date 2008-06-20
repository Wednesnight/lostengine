#ifndef EFFECT_H
#define EFFECT_H

#include "RenderEnvironment.h"
#include <stdint.h>
#include <Gl/Gl.h>

struct Effect
{
    Effect(RenderEnvironment& renv) : renderenv(renv) {}
    
    virtual void draw(uint32_t deltaTick) = 0;

    void set2DProjection();
    void set3DProjection(GLdouble eyeX,     GLdouble eyeY,      GLdouble eyeZ, 
                             GLdouble centerX,  GLdouble centerY,   GLdouble centerZ,
                             GLdouble upX,      GLdouble upY,       GLdouble upZ);

    RenderEnvironment& renderenv;
};

#endif
