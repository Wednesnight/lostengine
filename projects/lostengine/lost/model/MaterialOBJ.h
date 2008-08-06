#ifndef LOST_MODEL_MATERIALOBJ_H
#define LOST_MODEL_MATERIALOBJ_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include "lost/math/Vec3.h"

namespace lost
{
  namespace model
  {

    struct MaterialOBJ
    {
      struct MaterialGroup
      {
        unsigned int faceOffset;
        unsigned int faceLength;

        boost::shared_ptr<math::Vec3> ambient;
        boost::shared_ptr<math::Vec3> diffuse;
        boost::shared_ptr<math::Vec3> specular;
        
        MaterialGroup(unsigned int inFaceOffset, unsigned int inFaceLength, 
                      boost::shared_ptr<math::Vec3> inAmbient, boost::shared_ptr<math::Vec3> inDiffuse, boost::shared_ptr<math::Vec3> inSpecular)
        : faceOffset(inFaceOffset),
          faceLength(inFaceLength),
          ambient(inAmbient),
          diffuse(inDiffuse),
          specular(inSpecular)
        {
        }
      };
      
      std::vector<boost::shared_ptr<MaterialGroup> > groups;
      
      MaterialOBJ() {}
    };

  }
}

#endif
