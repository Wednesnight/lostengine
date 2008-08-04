#ifndef LOST_MODEL_LOADER_PARSER_PARSEROBJ_H
#define LOST_MODEL_LOADER_PARSER_PARSEROBJ_H

#include "lost/model/loader/parser/Parser.h"

//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/core.hpp>
#include <boost/spirit/utility/lists.hpp>
#include <boost/spirit/actor/increment_actor.hpp>
#include <boost/spirit/actor/decrement_actor.hpp>
#include <boost/spirit/phoenix/binders.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/shared_ptr.hpp>

#include "lost/model/Vertex.h"
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
          struct MeshAssignVertex
          {
            Mesh&         mesh;
            unsigned int& index;
            Vertex&       data;
            
            MeshAssignVertex(Mesh& inMesh, Vertex& inData, unsigned int& inIndex)
            : mesh(inMesh),
              index(inIndex),
              data(inData)
            {
              index = 0;
            }
            
            template<typename IteratorT>
            void operator()(IteratorT first, IteratorT last) const
            {
              mesh.setVertex(index++, data);
            }
          };
          
          struct MeshAssignFacePoint
          {
            Mesh&         mesh;
            unsigned int& index;
            unsigned int& data;
            
            MeshAssignFacePoint(Mesh& inMesh, unsigned int& inData, unsigned int& inIndex)
            : mesh(inMesh),
              index(inIndex),
              data(inData)
            {
              index = 0;
            }
            
            template<typename IteratorT>
            void operator()(IteratorT first, IteratorT last) const
            {
              mesh.setFacePoint(index++, data);
            }
          };
          
          boost::shared_ptr<lost::model::Mesh> initMesh(const std::string& inData)
          {
            boost::shared_ptr<lost::model::Mesh> result;
            
            unsigned int vertexCount = 0;
            unsigned int faceCount   = 0;
            
            boost::spirit::rule<> vertex_p_count  = boost::spirit::ch_p('v')
                                                    >> +boost::spirit::space_p >> boost::spirit::real_p
                                                    >> +boost::spirit::space_p >> boost::spirit::real_p
                                                    >> +boost::spirit::space_p >> boost::spirit::real_p
                                                    >> !(+boost::spirit::space_p >> boost::spirit::real_p)
                                                    >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> facevert_p_count = boost::spirit::int_p[boost::spirit::increment_a(faceCount)]
                                                     >> !(   '/'
                                                          >>  !(boost::spirit::int_p)
                                                          >>  !(   '/'
                                                                >> !(boost::spirit::int_p)
                                                                )
                                                          );                   
            boost::spirit::rule<> face_p_count = boost::spirit::ch_p('f') >> +boost::spirit::space_p
                                                 >> boost::spirit::list_p(facevert_p_count, +boost::spirit::space_p)
                                                 >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;                                                       
            boost::spirit::rule<> unknown_p_count = *(boost::spirit::anychar_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> objfile_p_count = *(vertex_p_count[boost::spirit::increment_a(vertexCount)] | 
                                                      face_p_count | 
                                                      unknown_p_count);
            
            BOOST_SPIRIT_DEBUG_NODE(vertex_p_count);
            BOOST_SPIRIT_DEBUG_NODE(facevert_p_count);
            BOOST_SPIRIT_DEBUG_NODE(face_p_count);
            BOOST_SPIRIT_DEBUG_NODE(unknown_p_count);
            BOOST_SPIRIT_DEBUG_NODE(objfile_p_count);
            
            if (boost::spirit::parse(inData.c_str(), objfile_p_count).full)
            {
              result.reset(new lost::model::Mesh(vertexCount, faceCount));
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
              unsigned int vertexIdx;
              unsigned int faceIdx;

              Vertex       vertex;
              unsigned int facePoint;
              
              boost::spirit::rule<> vertex_p  = boost::spirit::ch_p('v')
                                                >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(vertex.x)] 
                                                >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(vertex.y)]
                                                >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(vertex.z)]
                                                >> !(+boost::spirit::space_p >> boost::spirit::real_p)
                                                >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
              boost::spirit::rule<> facevert_p = boost::spirit::int_p[boost::spirit::assign_a(facePoint)][boost::spirit::decrement_a(facePoint)]
                                                 >> !(   '/'
                                                      >>  !(boost::spirit::int_p)
                                                      >>  !(   '/'
                                                            >> !(boost::spirit::int_p)
                                                            )
                                                      );                   
              boost::spirit::rule<> face_p = boost::spirit::ch_p('f') >> +boost::spirit::space_p
                                             >> boost::spirit::list_p(facevert_p[MeshAssignFacePoint(*result, facePoint, faceIdx)][boost::spirit::assign_a(facePoint, 0)], +boost::spirit::space_p)
                                             >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;                                                       
              boost::spirit::rule<> unknown_p = *(boost::spirit::anychar_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
              boost::spirit::rule<> objfile_p = *(vertex_p[MeshAssignVertex(*result, vertex, vertexIdx)]
                                                          [phoenix::bind(&Vertex::zero)(phoenix::var(vertex))] |
                                                  face_p |
                                                  unknown_p);
              
              BOOST_SPIRIT_DEBUG_NODE(vertex_p);
              BOOST_SPIRIT_DEBUG_NODE(facevert_p);
              BOOST_SPIRIT_DEBUG_NODE(face_p);
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
