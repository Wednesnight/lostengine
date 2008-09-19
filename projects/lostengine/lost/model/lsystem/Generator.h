#ifndef LOST_MODEL_LSYSTEM_GENERATOR_H
#define LOST_MODEL_LSYSTEM_GENERATOR_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "lost/lsystem/LSystem.h"
#include "lost/model/Mesh.h"
#include "lost/math/Matrix.h"
#include "lost/model/Vertex.h"

namespace lost
{
  namespace model
  {
    namespace lsystem
    {

      struct Generator
      {
        boost::shared_ptr<lost::lsystem::LSystem> lsystem;

        std::vector<lost::model::Vertex> lines;

        Generator()
        {
          lsystem.reset(new lost::lsystem::LSystem());
        }

        boost::shared_ptr<lost::model::Mesh> generate(const boost::shared_ptr<lost::lsystem::LSystemState>& state)
        {
          boost::shared_ptr<lost::model::Mesh> result(new lost::model::Mesh());

          // reset result coordinates
          lines.clear();
          // init offset matrix
          lost::math::Matrix matrix;
          matrix.initTranslation(lost::math::Vec3(0,0,0));
          // walk lsystem nodes and apply result coordinates
          lsystem->walk(state, matrix, boost::bind(&Generator::buildLSystemMesh, this, _1, _2));
          // apply result coordinates to Mesh
          result->setVertexCount(lines.size());
          unsigned int vertexIdx = 0;
          for (std::vector<lost::model::Vertex>::iterator idx = lines.begin(); idx != lines.end(); ++idx)
          {
            result->setVertex(vertexIdx++, *idx);
          }

          return result;
        }
        
        void buildLSystemMesh(lost::math::Matrix& matrix, lost::lsystem::LSystemResult& result)
        {
          if (result.drawable)
          {
            lines.push_back(result.matrix * Vertex(0,0,0));
            result.matrix = result.matrix * matrix;
            lines.push_back(result.matrix * Vertex(0,0,0));
          }
        }

      };
    }
  }
}

#endif
