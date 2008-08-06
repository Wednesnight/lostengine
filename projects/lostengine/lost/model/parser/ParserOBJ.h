#ifndef LOST_MODEL_PARSER_PARSEROBJ_H
#define LOST_MODEL_PARSER_PARSEROBJ_H

#include <map>

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
#include "lost/model/MaterialOBJ.h"
#include "lost/resource/Loader.h"

namespace lost
{
  namespace model
  {
    namespace parser
    {
    
      struct MaterialAttributes
      {
        boost::shared_ptr<math::Vec3> ambient;
        boost::shared_ptr<math::Vec3> diffuse;
        boost::shared_ptr<math::Vec3> specular;
        
        void clear()
        {
          ambient.reset();
          diffuse.reset();
          specular.reset();
        }
      };
      
      struct MaterialAttributesReset
      {
        boost::shared_ptr<MaterialAttributes>& attributes;
        
        MaterialAttributesReset(boost::shared_ptr<MaterialAttributes>& inAttributes)
        : attributes(inAttributes)
        {
        }
        
        template<typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
          attributes.reset(new MaterialAttributes());
        }
      };
      
      struct Vec3Reset
      {
        boost::shared_ptr<math::Vec3>& vec;
        
        Vec3Reset(boost::shared_ptr<math::Vec3>& inVec)
        : vec(inVec)
        {
        }
        
        template<typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
          vec.reset(new math::Vec3());
        }
      };
      
      struct MaterialParser
      {
        std::string&                               filename;
        boost::shared_ptr<resource::Loader>&       loader;
        std::map<std::string, MaterialAttributes>& materials;
        
        MaterialParser(std::string& inFilename, boost::shared_ptr<resource::Loader>& inLoader, std::map<std::string, MaterialAttributes>& inMaterials)
        : filename(inFilename),
          loader(inLoader),
          materials(inMaterials)
        {
        }
        
        template<typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
          boost::shared_ptr<resource::File> file = loader->load(filename);
          if (file)
          {
            std::string currentName;

            boost::shared_ptr<MaterialAttributes> currentAttributes;
            
            boost::spirit::rule<> material_p = boost::spirit::str_p("newmtl")[MaterialAttributesReset(currentAttributes)]
                                               >> +boost::spirit::space_p >> +(boost::spirit::anychar_p-boost::spirit::space_p)[boost::spirit::assign_a(currentName)]
                                               >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> ambient_p = boost::spirit::str_p("Ka")[Vec3Reset(currentAttributes->ambient)]
                                              >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(currentAttributes->ambient->x)] 
                                              >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(currentAttributes->ambient->y)]
                                              >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(currentAttributes->ambient->z)]
                                              >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> diffuse_p = boost::spirit::str_p("Kd")[Vec3Reset(currentAttributes->diffuse)]
                                              >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(currentAttributes->diffuse->x)] 
                                              >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(currentAttributes->diffuse->y)]
                                              >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(currentAttributes->diffuse->z)]
                                              >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> specular_p = boost::spirit::str_p("Ks")[Vec3Reset(currentAttributes->specular)]
                                               >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(currentAttributes->specular->x)] 
                                               >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(currentAttributes->specular->y)]
                                               >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(currentAttributes->specular->z)]
                                               >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> unknown_p = *(boost::spirit::anychar_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> mtlfile_p = *(material_p |
                                                ambient_p |
                                                diffuse_p |
                                                specular_p |
                                                unknown_p);
            
            BOOST_SPIRIT_DEBUG_NODE(material_p);
            BOOST_SPIRIT_DEBUG_NODE(ambient_p);
            BOOST_SPIRIT_DEBUG_NODE(diffuse_p);
            BOOST_SPIRIT_DEBUG_NODE(specular_p);
            BOOST_SPIRIT_DEBUG_NODE(unknown_p);
            BOOST_SPIRIT_DEBUG_NODE(mtlfile_p);
            
            if (boost::spirit::parse(file->str().c_str(), mtlfile_p).full)
            {
            }
          }
        }
      };
      
      struct ParserOBJ
      {
      private:
        boost::shared_ptr<resource::Loader>       loader;
        std::map<std::string, MaterialAttributes> materials;

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
        ParserOBJ(boost::shared_ptr<resource::Loader> inLoader)
        : loader(inLoader)
        {
        }
        
        virtual std::string getExtension()
        {
          return "OBJ";
        }

        bool parseMesh(const std::string& inFilename, boost::shared_ptr<Mesh>& outMesh, boost::shared_ptr<MaterialOBJ>& outMaterial)
        {
          bool result = false;
          boost::shared_ptr<resource::File> file = loader->load(inFilename);
          if (file)
          {
            std::string data = file->str();

            outMesh = initMesh(data);
            if (outMesh)
            {
              materials.clear();
              
              unsigned int vertexIdx;
              unsigned int faceIdx;

              Vertex       vertex;
              unsigned int facePoint;
              
              std::string materialFile;
              
              boost::spirit::rule<> mtllib_p = boost::spirit::str_p("mtllib")
                                               >> +boost::spirit::space_p >> +(boost::spirit::anychar_p-boost::spirit::space_p)[boost::spirit::assign_a(materialFile)][MaterialParser(materialFile, loader, materials)]
                                               >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
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
                                             >> boost::spirit::list_p(facevert_p[MeshAssignFacePoint(*outMesh, facePoint, faceIdx)][boost::spirit::assign_a(facePoint, 0)], +boost::spirit::space_p)
                                             >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;                                                       
              boost::spirit::rule<> unknown_p = *(boost::spirit::anychar_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
              boost::spirit::rule<> objfile_p = *(mtllib_p |
                                                  vertex_p[MeshAssignVertex(*outMesh, vertex, vertexIdx)]
                                                          [phoenix::bind(&Vertex::zero)(phoenix::var(vertex))] |
                                                  face_p |
                                                  unknown_p);
              
              BOOST_SPIRIT_DEBUG_NODE(mtllib_p);
              BOOST_SPIRIT_DEBUG_NODE(vertex_p);
              BOOST_SPIRIT_DEBUG_NODE(facevert_p);
              BOOST_SPIRIT_DEBUG_NODE(face_p);
              BOOST_SPIRIT_DEBUG_NODE(unknown_p);
              BOOST_SPIRIT_DEBUG_NODE(objfile_p);
              
              result = boost::spirit::parse(data.c_str(), objfile_p).full;
              if (!result)
              {
                outMesh.reset();
                outMaterial.reset();
              }
            }
          }

          return result;
        }
      };
    
    }
  }
}

#endif
