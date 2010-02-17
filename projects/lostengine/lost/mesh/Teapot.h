#ifndef LOST_MESH_TEAPOT_H
#define LOST_MESH_TEAPOT_H

#include "lost/mesh/Mesh.h"

namespace lost
{
namespace mesh
{

struct Teapot;
typedef lost::shared_ptr<Teapot> TeapotPtr;

struct Teapot
{
    Teapot()
    virtual ~Teapot();
    
    static TeapotPtr create();
};

}
}

#endif