#ifndef LOST_MODEL_LOADER_PARSER_PARSEROBJ_H
#define LOST_MODEL_LOADER_PARSER_PARSEROBJ_H

#include "lost/model/loader/parser/Parser.h"

//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/core.hpp>
#include <boost/spirit/utility/lists.hpp>
#include <boost/spirit/actor/increment_actor.hpp>
#include <boost/spirit/phoenix/binders.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/shared_ptr.hpp>

#include "lost/math/Vec3.h"
#include "lost/model/Mesh.h"
#include "lost/common/SpiritHelpers.h"
#include "lost/common/Logger.h"

namespace lost
{
  namespace model
  {
    namespace loader
    {
      namespace parser
      {
      
        struct ParserOBJ : public Parser
        {
        private:
          typedef enum
            {
              MeshAssignTypeVertex,
              MeshAssignTypeNormal
            } MeshAssignType;
          
          template <typename DataType>
          struct MeshAssign
          {
            Mesh&         mesh;
            unsigned int& index;
            DataType&     data;
            
            MeshAssignType assignType;
            
            MeshAssign(MeshAssignType inType, Mesh& inMesh, unsigned int& inIndex, DataType& inData)
            : mesh(inMesh),
              index(inIndex),
              data(inData),
              assignType(inType)
            {
              index = 0;
            }
            
            template<typename IteratorT>
            void operator()(IteratorT first, IteratorT last) const
            {
              switch (assignType)
              {
                case MeshAssignTypeVertex:
                  mesh.setVertex(index++, data);
                  break;
                case MeshAssignTypeNormal:
                  mesh.setNormal(index++, data);
                  break;
                default:
                  break;
              }
            }
          };
          
          boost::shared_ptr<lost::model::Mesh> initMesh(const std::string& inData)
          {
            boost::shared_ptr<lost::model::Mesh> result;
            
            unsigned int vertexCount = 0;
            unsigned int normalCount = 0;
            
            boost::spirit::rule<> vertex_p_count  = boost::spirit::ch_p('v')
                                                    >> +boost::spirit::space_p >> boost::spirit::real_p
                                                    >> +boost::spirit::space_p >> boost::spirit::real_p
                                                    >> +boost::spirit::space_p >> boost::spirit::real_p
                                                    >> !(+boost::spirit::space_p >> boost::spirit::real_p)
                                                    >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> normal_p_count  = boost::spirit::str_p("vn")
                                                    >> +boost::spirit::space_p >> boost::spirit::real_p
                                                    >> +boost::spirit::space_p >> boost::spirit::real_p
                                                    >> +boost::spirit::space_p >> boost::spirit::real_p
                                                    >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> unknown_p_count = *(boost::spirit::anychar_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> objfile_p_count = *(vertex_p_count[boost::spirit::increment_a(vertexCount)] | 
                                                      normal_p_count[boost::spirit::increment_a(normalCount)] | 
                                                      unknown_p_count);
            
            BOOST_SPIRIT_DEBUG_NODE(vertex_p_count);
            BOOST_SPIRIT_DEBUG_NODE(normal_p_count);
            BOOST_SPIRIT_DEBUG_NODE(unknown_p_count);
            BOOST_SPIRIT_DEBUG_NODE(objfile_p_count);
            
            if (boost::spirit::parse(inData.c_str(), objfile_p_count).full)
            {
              result.reset(new lost::model::Mesh(vertexCount, normalCount));
            }

            return result;
          }
          
        public:
          virtual std::string getExtension()
          {
            return "OBJ";
          }

          boost::shared_ptr<lost::model::Mesh> parseMesh(const std::string& inData)
          {
            boost::shared_ptr<lost::model::Mesh> result = initMesh(inData);
            if (result)
            {
              unsigned int     index;
              lost::math::Vec3 vertex;
              lost::math::Vec3 normal;
              
              boost::spirit::rule<> vertex_p  = boost::spirit::ch_p('v')
                                                >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(vertex.x)] 
                                                >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(vertex.y)]
                                                >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(vertex.z)]
                                                >> !(+boost::spirit::space_p >> boost::spirit::real_p)
                                                >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
              boost::spirit::rule<> normal_p  = boost::spirit::str_p("vn")
                                                >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(normal.x)] 
                                                >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(normal.y)]
                                                >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(normal.z)]
                                                >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
              boost::spirit::rule<> unknown_p = *(boost::spirit::anychar_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
              boost::spirit::rule<> objfile_p = *(vertex_p[MeshAssign<lost::math::Vec3>(MeshAssignTypeVertex, *result, index, vertex)]
                                                          [phoenix::bind(&lost::math::Vec3::zero)(phoenix::var(vertex))] |
                                                  normal_p[MeshAssign<lost::math::Vec3>(MeshAssignTypeNormal, *result, index, normal)]
                                                          [phoenix::bind(&lost::math::Vec3::zero)(phoenix::var(normal))] |
                                                  unknown_p);
              
              BOOST_SPIRIT_DEBUG_NODE(vertex_p);
              BOOST_SPIRIT_DEBUG_NODE(normal_p);
              BOOST_SPIRIT_DEBUG_NODE(unknown_p);
              BOOST_SPIRIT_DEBUG_NODE(objfile_p);
              
              if (!boost::spirit::parse(inData.c_str(), objfile_p).full)
              {
                result.reset();
              }
            }

            return result;
          }
        };
      
      }
    }
  }
}

#endif
