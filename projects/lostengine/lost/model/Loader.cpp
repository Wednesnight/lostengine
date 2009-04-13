#include <boost/shared_ptr.hpp>
#include "Loader.h"
#include "lost/math/Vec3.h"
#include "lost/common/Logger.h"

//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/core.hpp>
#include <boost/spirit/actor/increment_actor.hpp>
#include <boost/spirit/actor/decrement_actor.hpp>
#include <boost/spirit/phoenix/binders.hpp>
//#include <boost/spirit/attribute.hpp>
//#include <boost/spirit/utility/lists.hpp>

using namespace boost;
using namespace boost::spirit;
using namespace std;
using namespace lost::math;
using namespace lost::mesh;
using namespace lost::resource;

namespace lost
{
  namespace model
  {

    /**
     * spirit actions for OBJ parsing
     */
    struct OBJActions
    {
      template <typename Type>
      struct SetArray
      {
        Type* array;
        Type& value;
        SetArray(Type* inArray, Type& inValue)
        : array(inArray),
          value(inValue)
        {
        }

        template<typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
          static unsigned int idx = 0;
          array[idx++] = value;
        }
      };
    };

    /**
     * load obj model from fileName through loader, returns lost::mesh::MeshPtr
     */
    Mesh3DPtr Loader::obj(shared_ptr<resource::Loader>& loader, const string& fileName)
    {
      /**
       * init resulting mesh
       */
      Mesh3DPtr mesh(new Mesh3D);

      /**
       *  load obj model
       */
      shared_ptr<File> objFile = loader->load(fileName);
      if (objFile)
      {
        /**
         * init obj data
         */
        std::string objData = objFile->str();

        /**
         * general spirit rules
         */
        rule<> unknown_p = *(anychar_p - eol_p) >> eol_p;

        /**
         * count vertices
         */
        unsigned int vertexCount = 0;
        rule<> vertexCount_p = (ch_p('v') >> +space_p >>
                                  real_p >> +space_p >>
                                  real_p >> +space_p >>
                                  real_p >> !(+space_p >> real_p) >> *(space_p - eol_p) >> 
                                eol_p)[increment_a(vertexCount)];
        BOOST_SPIRIT_DEBUG_NODE(vertexCount_p);

        /**
         * count normals
         */
        unsigned int normalCount = 0;
        rule<> normalCount_p = (str_p("vn") >> +space_p >> *(anychar_p - eol_p) >> eol_p)[increment_a(normalCount)];
        BOOST_SPIRIT_DEBUG_NODE(normalCount_p);
        
        /**
         * count indices
         */
        unsigned int indexCount = 0;
        rule<> indexCount_p = 
          ch_p('f') >> +space_p >> 
            int_p[increment_a(indexCount)] >> !('/' >> !(int_p) >> !('/' >> !(int_p))) >> +space_p >>
            int_p[increment_a(indexCount)] >> !('/' >> !(int_p) >> !('/' >> !(int_p))) >> +space_p >>
            int_p[increment_a(indexCount)] >> !('/' >> !(int_p) >> !('/' >> !(int_p))) >> *(space_p-eol_p) >>
          eol_p;
        BOOST_SPIRIT_DEBUG_NODE(indexCount_p);
        
        /**
         * count vertices, normals, indices
         */
        rule<> count_p = *(vertexCount_p | normalCount_p | indexCount_p | unknown_p);
        BOOST_SPIRIT_DEBUG_NODE(count_p);

        if (parse(objData.c_str(), count_p).full)
        {
#ifdef BOOST_SPIRIT_DEBUG
          /**
           * debug output
           */
          DOUT("vertexCount      : " << vertexCount);
          DOUT("normalCount      : " << normalCount);
          DOUT("indexCount       : " << indexCount);
#endif

          /**
           * init data arrays
           */
          Mesh3D::VertexType* vertices = new Mesh3D::VertexType[vertexCount];
          Mesh3D::NormalType* normals  = new Mesh3D::NormalType[normalCount];
          Mesh3D::IndexType*  indices  = new Mesh3D::IndexType[indexCount];

          /**
           * load vec3 type
           */
          Vec3 vec3;
          rule<> vec3_p = 
            real_p[assign_a(vec3.x)] >> +space_p >>
            real_p[assign_a(vec3.y)] >> +space_p >>
            real_p[assign_a(vec3.z)] >> !(+space_p >> real_p) >> *(space_p-eol_p);

          /**
           * load vertices
           */
          rule<> vertex_p = 
            ch_p('v') >> +space_p >> 
              vec3_p[OBJActions::SetArray<Mesh3D::VertexType>(vertices, vec3)] >>
            eol_p;
          BOOST_SPIRIT_DEBUG_NODE(vertex_p);

          /**
           * load normals
           */
          rule<> normal_p = 
            str_p("vn") >> +space_p >> 
              vec3_p[OBJActions::SetArray<Mesh3D::NormalType>(normals, vec3)] >>
            eol_p;
          BOOST_SPIRIT_DEBUG_NODE(normal_p);

          /**
           * load indices
           */
          Mesh3D::IndexType index;
          rule<> index_p = int_p[assign_a(index)][decrement_a(index)] >> !('/' >> !(int_p) >> !('/' >> !(int_p)));
          BOOST_SPIRIT_DEBUG_NODE(index_p);
          rule<> indexAssign_p = index_p[OBJActions::SetArray<Mesh3D::IndexType>(indices, index)];
          BOOST_SPIRIT_DEBUG_NODE(indexAssign_p);
          rule<> face_p =
            ch_p('f') >> +space_p >>
              indexAssign_p >> +space_p >>
              indexAssign_p >> +space_p >>
              indexAssign_p >> *(space_p-eol_p) >>
            eol_p;
          BOOST_SPIRIT_DEBUG_NODE(face_p);
          
          /**
           * load vec3 type, vertices, normals, indices
           */
          rule<> assign_p = *(vertex_p | normal_p | face_p | unknown_p);
          BOOST_SPIRIT_DEBUG_NODE(assign_p);

          if (parse(objData.c_str(), assign_p).full)
          {
#ifdef BOOST_SPIRIT_DEBUG
            /**
             * debug output
             */
            DOUT("vertices:");
            for (unsigned int idx = 0; idx < vertexCount; idx++)
              DOUT(vertices[idx]);
            DOUT("normals:");
            for (unsigned int idx = 0; idx < normalCount; idx++)
              DOUT(normals[idx]);
            DOUT("indices:");
            for (unsigned int idx = 0; idx < indexCount; idx++)
              DOUT(indices[idx]);
#endif

            /**
             * init drawing mode
             */
            mesh->drawMode = GL_TRIANGLES;

            /**
             * set mesh vertices
             */
            if (vertexCount)
            {
              mesh->vertices(true);
              mesh->vertexBuffer->bindBufferData(vertices, vertexCount);
            }

            /**
             * set mesh normals
             */
            if (normalCount)
            {
              mesh->normals(true);
              mesh->normalBuffer->bindBufferData(normals, normalCount);
            }

            /**
             * set mesh indices
             */
            if (indexCount)
            {
              mesh->indices(true);
              mesh->indexBuffer->bindBufferData(indices, indexCount);
            }
          }
          delete indices;
          delete normals;
          delete vertices;
        }
      }

      return mesh;
    }

  }
}
