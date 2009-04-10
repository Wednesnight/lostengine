#ifndef LOST_MODEL_LOADER_H
#define LOST_MODEL_LOADER_H

#include <string>
#include "lost/mesh/Mesh.h"
#include "lost/resource/Loader.h"

namespace lost
{
  namespace model
  {

    struct Loader
    {

      static lost::mesh::Mesh3DPtr obj(boost::shared_ptr<lost::resource::Loader>& loader, const std::string& fileName);

    };

  }
}

#endif
