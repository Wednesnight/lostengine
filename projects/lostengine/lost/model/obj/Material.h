#ifndef LOST_MODEL_OBJ_MATERIAL_H
#define LOST_MODEL_OBJ_MATERIAL_H

#include <vector>
#include "lost/math/Vec4.h"

namespace lost
{
  namespace model
  {
    namespace obj
    {

      struct MaterialGroup
      {
        unsigned int faceOffset;
        unsigned int faceLength;
        
        boost::shared_ptr<math::Vec4> ambient;
        boost::shared_ptr<math::Vec4> diffuse;
        boost::shared_ptr<math::Vec4> specular;
        
        MaterialGroup(unsigned int inFaceOffset, unsigned int inFaceLength, 
                      boost::shared_ptr<math::Vec4> inAmbient, boost::shared_ptr<math::Vec4> inDiffuse, boost::shared_ptr<math::Vec4> inSpecular)
        : faceOffset(inFaceOffset),
        faceLength(inFaceLength)
        {
          if (inAmbient) ambient = inAmbient;
          if (inDiffuse) diffuse = inDiffuse;
          if (inSpecular) specular = inSpecular;
        }
      };
      typedef std::vector<boost::shared_ptr<MaterialGroup> > MaterialGroups;

      struct Material
      {
        MaterialGroups groups;
        
        Material() {}
      };

    }
  }
}

#endif
