#ifndef LOST_MODEL_OBJ_PARSER_H
#define LOST_MODEL_OBJ_PARSER_H

// spirit::real_p (double) assignment to float
#pragma warning(disable:4244)

#include <map>

#include "lost/model/Vertex.h"
#include "lost/model/Mesh.h"
#include "lost/common/SpiritHelpers.h"
#include "lost/common/Logger.h"
#include "lost/model/obj/Material.h"
#include "lost/resource/Loader.h"
#include "lost/math/Vec4.h"

namespace lost
{
  namespace model
  {
    namespace obj
    {

      struct MaterialAttributes
      {
        boost::shared_ptr<math::Vec4> ambient;
        boost::shared_ptr<math::Vec4> diffuse;
        boost::shared_ptr<math::Vec4> specular;
      };

      struct Parser
      {
      private:
        boost::shared_ptr<resource::Loader> loader;

        std::map<std::string, boost::shared_ptr<MaterialAttributes> > materials;

        boost::shared_ptr<lost::model::Mesh> initMesh(const std::string& inData);

      public:
        Parser(boost::shared_ptr<resource::Loader> inLoader);
        virtual ~Parser();

        virtual std::string getExtension();

        bool parseMesh(const std::string& inFilename, boost::shared_ptr<Mesh>& outMesh, boost::shared_ptr<Material>& outMaterial);
      };
    }
  }
}

#endif
