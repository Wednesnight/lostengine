#ifndef LOST_MESH_LOADER_H
#define LOST_MESH_LOADER_H

#include "lost/mesh/Mesh.h"

namespace lost
{
  namespace resource
  {
    struct File;
    typedef lost::shared_ptr<File> FilePtr;
  }

  namespace mesh
  {

    struct Loader;
    typedef lost::shared_ptr<Loader> LoaderPtr;

    struct Loader
    {
      static Mesh3DPtr obj(resource::FilePtr objFile);
    };

  }
}

#endif
