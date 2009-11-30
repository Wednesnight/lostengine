#ifndef LOST_MESH_LOADER_H
#define LOST_MESH_LOADER_H

#include "lost/mesh/Mesh.h"
#include "lost/common/Data.h"

namespace lost
{

  namespace mesh
  {

    struct Loader;
    typedef lost::shared_ptr<Loader> LoaderPtr;

    struct Loader
    {
      static MeshPtr obj(common::DataPtr objFile);
    };

  }
}

#endif
