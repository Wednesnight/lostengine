
#include "Effect.h"
#include <GL/GL.h>
#include <GLUT/glut.h>
#include "GlUtils.h"

void Effect::set2DProjection()
{
    glutils::set2DProjection(renderenv);
}

void Effect::set3DProjection(GLdouble eyeX,     GLdouble eyeY,      GLdouble eyeZ, 
                             GLdouble centerX,  GLdouble centerY,   GLdouble centerZ,
                             GLdouble upX,      GLdouble upY,       GLdouble upZ)
{
    glutils::set3DProjection(renderenv,
                    eyeX,     eyeY,      eyeZ, 
                    centerX,  centerY,   centerZ,
                    upX,      upY,       upZ);
}
