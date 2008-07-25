#include <iostream>

//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/core.hpp>
#include <boost/spirit/utility/lists.hpp>
#include <boost/spirit/actor/increment_actor.hpp>
#include <boost/spirit/phoenix/binders.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/shared_ptr.hpp>

#include "lost/model/Parser.h"
#include "lost/math/Vec3.h"
#include "lost/model/Mesh.h"
#include "lost/math/Vec4.h"
#include "lost/common/SpiritHelpers.h"
#include "lost/common/Logger.h"

using namespace std;
using namespace boost;
using namespace boost::spirit;
using namespace lost::math;
using namespace lost::common::spirithelpers;

using phoenix::bind;
using phoenix::var;

namespace lost
{
  namespace model
  {
    namespace parser
    {

      struct MeshAssignVec3
      {
        typedef enum
        {
          typeVertex,
          typeNormal
        } MeshAssignVec3Type;
        
        Mesh&         mesh;
        unsigned int& index;
        Vec3&         vec;

        MeshAssignVec3Type assignType;
        
        MeshAssignVec3(MeshAssignVec3Type inType, Mesh& inMesh, unsigned int& inIndex, Vec3& inVec)
        : mesh(inMesh),
          index(inIndex),
          vec(inVec),
          assignType(inType)
        {
          index = 0;
        }

        template<typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
          switch (assignType)
          {
            case typeVertex:
              mesh.setVertex(index++, vec);
              break;
            case typeNormal:
              mesh.setNormal(index++, vec);
              break;
          }
        }
      };
      
      shared_ptr<Mesh> parse(const std::string& data)
      {
        shared_ptr<Mesh> result;

        unsigned int vertexCount = 0;
        unsigned int normalCount = 0;
        
        rule<> vertex_p_count  = ch_p('v')
                                  >> +space_p >> real_p
                                  >> +space_p >> real_p
                                  >> +space_p >> real_p
                                  >> !(+space_p >> real_p)
                                  >> *(space_p-eol_p) >> eol_p;
        rule<> normal_p_count  = str_p("vn")
                                  >> +space_p >> real_p
                                  >> +space_p >> real_p
                                  >> +space_p >> real_p
                                  >> *(space_p-eol_p) >> eol_p;
        rule<> unknown_p_count = *(anychar_p-eol_p) >> eol_p;
        rule<> objfile_p_count = *(vertex_p_count[increment_a(vertexCount)] | 
                                   normal_p_count[increment_a(normalCount)] | 
                                   unknown_p_count);

        BOOST_SPIRIT_DEBUG_NODE(vertex_p_count);
        BOOST_SPIRIT_DEBUG_NODE(normal_p_count);
        BOOST_SPIRIT_DEBUG_NODE(unknown_p_count);
        BOOST_SPIRIT_DEBUG_NODE(objfile_p_count);

        if (boost::spirit::parse(data.c_str(), objfile_p_count).full)
        {
          result.reset(new Mesh(vertexCount, normalCount));

          unsigned int index;
          Vec3         vertex;
          Vec3         normal;
          
          rule<> vertex_p  = ch_p('v')
                              >> +space_p >> real_p[assign_a(vertex.x)] 
                              >> +space_p >> real_p[assign_a(vertex.y)]
                              >> +space_p >> real_p[assign_a(vertex.z)]
                              >> !(+space_p >> real_p)
//                              >> !(+space_p >> real_p[assign_a(vertex.w)])
                              >> *(space_p-eol_p) >> eol_p;
          rule<> normal_p  = str_p("vn")
                              >> +space_p >> real_p[assign_a(normal.x)] 
                              >> +space_p >> real_p[assign_a(normal.y)]
                              >> +space_p >> real_p[assign_a(normal.z)]
                              >> *(space_p-eol_p) >> eol_p;
          rule<> unknown_p = *(anychar_p-eol_p) >> eol_p;
          rule<> objfile_p = *(vertex_p[MeshAssignVec3(MeshAssignVec3::typeVertex, *result, index, vertex)][bind(&Vec3::zero)(var(vertex))] |
                               normal_p[MeshAssignVec3(MeshAssignVec3::typeNormal, *result, index, normal)][bind(&Vec3::zero)(var(normal))] |
                               unknown_p);

          BOOST_SPIRIT_DEBUG_NODE(vertex_p);
          BOOST_SPIRIT_DEBUG_NODE(normal_p);
          BOOST_SPIRIT_DEBUG_NODE(unknown_p);
          BOOST_SPIRIT_DEBUG_NODE(objfile_p);
          
          if (!boost::spirit::parse(data.c_str(), objfile_p).full)
          {
            result.reset();
          }
          DOUT("index: " << index);
        }
        
        return result;
      }

    } // parser
  } // obj
}
