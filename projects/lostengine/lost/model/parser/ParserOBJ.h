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
#include "lost/math/Vec4.h"

namespace lost
{
  namespace model
  {
    namespace parser
    {
    
      struct MaterialAttributes
      {
        boost::shared_ptr<math::Vec4> ambient;
        boost::shared_ptr<math::Vec4> diffuse;
        boost::shared_ptr<math::Vec4> specular;
      };
      
      struct MaterialAttributesNext
      {
        std::map<std::string, boost::shared_ptr<MaterialAttributes> >& materials;
        std::string&                                                   name;
        boost::shared_ptr<MaterialAttributes>&                         attributes;
        
        MaterialAttributesNext(std::map<std::string, boost::shared_ptr<MaterialAttributes> >& inMaterials, std::string& inName, boost::shared_ptr<MaterialAttributes>& inAttributes)
        : materials(inMaterials),
          name(inName),
          attributes(inAttributes)
        {
        }
        
        template<typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
          DOUT("adding material: " << name);
          attributes.reset(new MaterialAttributes());
          materials[name] = attributes;
        }
      };
      
      struct MaterialAttributesAssign
      {
        typedef enum
        {
          typeAmbient,
          typeDiffuse,
          typeSpecular
        } AssignType;
        
        AssignType                             type;
        boost::shared_ptr<MaterialAttributes>& attributes;
        math::Vec4&                            vec;
        
        MaterialAttributesAssign(AssignType inType, boost::shared_ptr<MaterialAttributes>& inAttributes, math::Vec4& inVec)
        : type(inType),
          attributes(inAttributes),
          vec(inVec)
        {
        }
        
        template<typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
          switch (type)
          {
            case typeAmbient:
              attributes->ambient.reset(new math::Vec4(vec));
              break;
            case typeDiffuse:
              attributes->diffuse.reset(new math::Vec4(vec));
              break;
            case typeSpecular:
              attributes->specular.reset(new math::Vec4(vec));
              break;
          }
        }
      };
      
      struct MaterialParser
      {
        std::string&                                                   filename;
        boost::shared_ptr<resource::Loader>&                           loader;
        std::map<std::string, boost::shared_ptr<MaterialAttributes> >& materials;
        
        MaterialParser(std::string& inFilename, boost::shared_ptr<resource::Loader>& inLoader, std::map<std::string, boost::shared_ptr<MaterialAttributes> >& inMaterials)
        : filename(inFilename),
          loader(inLoader),
          materials(inMaterials)
        {
        }
        
        template<typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
          DOUT("loading");
          boost::shared_ptr<resource::File> file = loader->load(filename);
          if (file)
          {
            DOUT("file is valid");
            std::string currentName;

            boost::shared_ptr<MaterialAttributes> currentAttributes;

            math::Vec4 ambient;
            math::Vec4 diffuse;
            math::Vec4 specular;

            DOUT("rules");
            boost::spirit::rule<> material_p = boost::spirit::str_p("newmtl")
                                               >> +boost::spirit::space_p >> (+(boost::spirit::anychar_p-boost::spirit::space_p))[boost::spirit::assign_a(currentName)]
                                               >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> ambient_p = boost::spirit::str_p("Ka")
                                              >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(ambient.x)] 
                                              >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(ambient.y)]
                                              >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(ambient.z)][boost::spirit::assign_a(ambient.w, 1.0f)]
                                              >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> diffuse_p = boost::spirit::str_p("Kd")
                                              >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(diffuse.x)] 
                                              >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(diffuse.y)]
                                              >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(diffuse.z)][boost::spirit::assign_a(diffuse.w, 1.0f)]
                                              >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> specular_p = boost::spirit::str_p("Ks")
                                               >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(specular.x)] 
                                               >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(specular.y)]
                                               >> +boost::spirit::space_p >> boost::spirit::real_p[boost::spirit::assign_a(specular.z)][boost::spirit::assign_a(specular.w, 1.0f)]
                                               >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> unknown_p = *(boost::spirit::anychar_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
            boost::spirit::rule<> mtlfile_p = *(material_p[MaterialAttributesNext(materials, currentName, currentAttributes)] |
                                                ambient_p[MaterialAttributesAssign(MaterialAttributesAssign::typeAmbient, currentAttributes, ambient)] |
                                                diffuse_p[MaterialAttributesAssign(MaterialAttributesAssign::typeDiffuse, currentAttributes, diffuse)] |
                                                specular_p[MaterialAttributesAssign(MaterialAttributesAssign::typeSpecular, currentAttributes, specular)] |
                                                unknown_p);
            
            DOUT("debug");
            BOOST_SPIRIT_DEBUG_NODE(material_p);
            BOOST_SPIRIT_DEBUG_NODE(ambient_p);
            BOOST_SPIRIT_DEBUG_NODE(diffuse_p);
            BOOST_SPIRIT_DEBUG_NODE(specular_p);
            BOOST_SPIRIT_DEBUG_NODE(unknown_p);
            BOOST_SPIRIT_DEBUG_NODE(mtlfile_p);
            
            DOUT("parse");
            if (boost::spirit::parse(file->str().c_str(), mtlfile_p).full)
            {
            }
          }
        }
      };
      
      struct MaterialInit
      {
        boost::shared_ptr<MaterialOBJ>& material;
        
        MaterialInit(boost::shared_ptr<MaterialOBJ>& inMaterial)
        : material(inMaterial)
        {
        }
        
        template<typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
          material.reset(new MaterialOBJ());
        }
      };
      
      struct MaterialAssign
      {
        boost::shared_ptr<MaterialOBJ>& material;
        std::map<std::string, boost::shared_ptr<MaterialAttributes> >& materials;
        std::string& materialName;
        unsigned int& materialFaceOffset; 
        boost::shared_ptr<MaterialGroup>& group;

        MaterialAssign(boost::shared_ptr<MaterialOBJ>& inMaterial, 
                       std::map<std::string, boost::shared_ptr<MaterialAttributes> >& inMaterials, 
                       std::string& inMaterialName, 
                       unsigned int& inMaterialFaceOffset, 
                       boost::shared_ptr<MaterialGroup>& inGroup)
        : material(inMaterial),
          materials(inMaterials),
          materialName(inMaterialName),
          materialFaceOffset(inMaterialFaceOffset),
          group(inGroup)
        {
        }

        template<typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
          DOUT("trying to add material group: " << materialName);
          DOUT("materials.find(materialName): " << (materials.find(materialName) != materials.end()));
          if (materialName.size() && materials.find(materialName) != materials.end())
          {
            DOUT("adding material group: " << materialName);
            boost::shared_ptr<MaterialAttributes> attributes = materials[materialName];
            DOUT("resetting group");
            group.reset(new MaterialGroup(materialFaceOffset, 0, attributes->ambient, attributes->diffuse, attributes->specular));
            DOUT("adding group to material");
            material->groups.push_back(group);
          }
        }
      };
      
      struct MaterialGroupIncrement
      {
        boost::shared_ptr<MaterialGroup>& group;
        
        MaterialGroupIncrement(boost::shared_ptr<MaterialGroup>& inGroup)
        : group(inGroup)
        {
        }
        
        template<typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
          if (group) group->faceLength+= 3;
        }
      };
      
      struct ParserOBJ
      {
      private:
        boost::shared_ptr<resource::Loader> loader;

        std::map<std::string, boost::shared_ptr<MaterialAttributes> > materials;

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
              
              std::string  materialFile;
              std::string  materialName;
              unsigned int materialFaceOffset;

              boost::shared_ptr<MaterialGroup> currentGroup;
              
              boost::spirit::rule<> mtllib_p = boost::spirit::str_p("mtllib")
                                               >> +boost::spirit::space_p >> (+(boost::spirit::anychar_p-boost::spirit::space_p))[boost::spirit::assign_a(materialFile)][MaterialParser(materialFile, loader, materials)]
                                               >> *(boost::spirit::space_p-boost::spirit::eol_p) >> boost::spirit::eol_p;
              boost::spirit::rule<> usemtl_p = boost::spirit::str_p("usemtl")
                                               >> +boost::spirit::space_p >> (+(boost::spirit::anychar_p-boost::spirit::space_p))[boost::spirit::assign_a(materialName)][boost::spirit::assign_a(materialFaceOffset, faceIdx)]
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
              boost::spirit::rule<> objfile_p = *(mtllib_p[MaterialInit(outMaterial)] |
                                                  usemtl_p[MaterialAssign(outMaterial, materials, materialName, materialFaceOffset, currentGroup)] |
                                                  vertex_p[MeshAssignVertex(*outMesh, vertex, vertexIdx)]
                                                          [phoenix::bind(&Vertex::zero)(phoenix::var(vertex))] |
                                                  face_p[MaterialGroupIncrement(currentGroup)] |
                                                  unknown_p);
              
              BOOST_SPIRIT_DEBUG_NODE(mtllib_p);
              BOOST_SPIRIT_DEBUG_NODE(usemtl_p);
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
