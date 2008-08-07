#ifndef LOST_MODEL_MATERIALOBJ_H
#define LOST_MODEL_MATERIALOBJ_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include "lost/math/Vec4.h"

namespace lost
{
  namespace model
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

    struct MaterialOBJ
    {
      MaterialGroups groups;
      
      MaterialOBJ() {}
    };

  }
}

#endif
