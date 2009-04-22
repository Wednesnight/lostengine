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
        
        lost::shared_ptr<math::Vec4> ambient;
        lost::shared_ptr<math::Vec4> diffuse;
        lost::shared_ptr<math::Vec4> specular;
        
        MaterialGroup(unsigned int inFaceOffset, unsigned int inFaceLength, 
                      lost::shared_ptr<math::Vec4> inAmbient, lost::shared_ptr<math::Vec4> inDiffuse, lost::shared_ptr<math::Vec4> inSpecular)
        : faceOffset(inFaceOffset),
        faceLength(inFaceLength)
        {
          if (inAmbient) ambient = inAmbient;
          if (inDiffuse) diffuse = inDiffuse;
          if (inSpecular) specular = inSpecular;
        }
      };
      typedef std::vector<lost::shared_ptr<MaterialGroup> > MaterialGroups;

      struct Material
      {
        MaterialGroups groups;
        
        Material() {}
      };

    }
  }
}

#endif
