/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/mesh/Loader.h"
#include "lost/math/Vec3.h"
#include "lost/common/Logger.h"
#include "lost/resource/Bundle.h"
#include <slub/slub.h>
#include <slub/table.h>
#include <stdexcept>
#pragma warning(disable:4244) // warning C4244: 'argument' : conversion from 'const double' to 'float', possible loss of data

//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_increment_actor.hpp>
#include <boost/spirit/include/classic_decrement_actor.hpp>
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"

using namespace boost;
using namespace boost::spirit;
using namespace boost::spirit::classic;
using namespace lost::math;
using namespace lost::mesh;
using namespace lost::gl;

namespace lost
{
  namespace mesh
  {    

    void write(uint8_t* data, uint32_t numBytes, const lost::fs::Path& inPath)  
    {
      FILE* file = fopen(inPath.string().c_str(), "wb");
      if (file != NULL) {
        if (fwrite(data, 1, numBytes, file) != numBytes) {
          EOUT("write failed to "<<inPath.string());
        }
        fclose(file);
      }
      else {
        EOUT("failed to open file for write: "<<inPath.string().c_str());
      }      
    }
    
    void dumpBufferLayout(HybridBufferPtr buffer)
    {
      BufferLayout bl = buffer->hostBuffer->layout;
      for(vector<BufferLayoutAttribute>::iterator pos = bl.attributes.begin(); pos != bl.attributes.end(); ++pos)
      {
        BufferLayoutAttribute attr = *pos;
        DOUT("{ \"" << gl::nameForElementType(attr.elementType) << "\", \"" << gl::nameForUsageType(attr.usageType) << "\", " << (attr.normalise ? "true" : "false") << "},");
      }      
    }
    
    void writeVertexBuffer(const MeshPtr& inMesh, const lost::fs::Path& inPath)
    {
      HybridVertexBufferPtr vb = inMesh->vertexBuffer;
      HostBufferPtr hostBuffer = vb->hostBuffer;
      DOUT("MESH write vb size: "<<hostBuffer->bufferSize());
      write(hostBuffer->buffer, hostBuffer->bufferSize(), inPath);
      
      dumpBufferLayout(vb);
    }
    
    void writeIndexBuffer(const MeshPtr& inMesh, const lost::fs::Path& inPath)
    {
      HybridIndexBufferPtr ib = inMesh->indexBuffer;
      HostBufferPtr hostBuffer = ib->hostBuffer;
      DOUT("MESH write ib size: "<<hostBuffer->bufferSize());      
      write(hostBuffer->buffer, hostBuffer->bufferSize(), inPath);

      dumpBufferLayout(ib);
    }
    
    MeshPtr loadMeshBundle(const resource::BundlePtr& bundle, lua_State* state)
    {
      MeshPtr result;
      
/*      slub::table meta = bundle->require(fs::Path("meta.lua"), state);
      slub::table vblayout = meta["vblayout"];
      slub::table iblayout = meta["iblayout"];
//      string vbname = meta["vbname"].cast<string>();
//      string ibname = meta["ibname"].cast<string>();
      slub::reference vbname = meta["vbname"];
      slub::reference ibname = meta["ibname"];
      
//      DOUT("vblayout "<<vblayout.isNil());
//      DOUT("iblayout "<<iblayout.isNil());
//      DOUT("vbname "<<vbname.isNil());
//      DOUT("ibname "<<ibname.isNil());
  */    
      return result;
    }

    
    template <typename Type>
    struct MeshSet
    {
      MeshPtr mesh;
      Type& value;
      uint16_t& idx;
      UsageType usageType;
      MeshSet(uint16_t& inIdx, MeshPtr m, Type& inValue, UsageType ut)
      : idx(inIdx),
        mesh(m),
        value(inValue),
        usageType(ut)
      {
      }

      template<typename IteratorT>
      void operator()(IteratorT first, IteratorT last) const
      {
        switch(usageType)
        {
          case UT_position:
          case UT_normal:
            mesh->vertexBuffer->set(idx, usageType, value);
            break;
          case UT_index:
            mesh->indexBuffer->set(idx, usageType, value);
            break;
          default:
            throw std::runtime_error("don't know what to do with usageType");
            break;
        }
      }
    };
    

    MeshPtr Loader::obj(common::DataPtr objFile)
    {
      BufferLayout layout;
      layout.add(ET_vec3_f32, UT_position);
      layout.add(ET_vec3_f32, UT_normal);
      MeshPtr mesh(new Mesh(layout, ET_u16));
      uint16_t vtxCount = 0;
      uint16_t nrmCount = 0;
      uint16_t idxCount = 0;

      string objData = objFile->str();

      rule<> unknown_p = *(anychar_p - eol_p) >> eol_p;

      unsigned int vertexCount = 0;
      rule<> realTriple =  real_p >> +space_p >> real_p >> +space_p >> real_p;
      rule<> optionalReal = !(+space_p >> real_p);
      rule<> zeroOrMoreSpacesToEol = *(space_p - eol_p);
      rule<> vertexCount_p = (ch_p('v') >> +space_p >> realTriple >> optionalReal >> zeroOrMoreSpacesToEol >> 
                              eol_p)[increment_a(vertexCount)];
      BOOST_SPIRIT_DEBUG_NODE(vertexCount_p);

      uint16_t normalCount = 0;
      rule<> normalCount_p = (str_p("vn") >> +space_p >> *(anychar_p - eol_p) >> eol_p)[increment_a(normalCount)];
      BOOST_SPIRIT_DEBUG_NODE(normalCount_p);
      
      uint16_t indexCount = 0;
      rule<> indexCount_p = 
        ch_p('f') >> +space_p >> 
          int_p[increment_a(indexCount)] >> !('/' >> !(int_p) >> !('/' >> !(int_p))) >> +space_p >>
          int_p[increment_a(indexCount)] >> !('/' >> !(int_p) >> !('/' >> !(int_p))) >> +space_p >>
          int_p[increment_a(indexCount)] >> !('/' >> !(int_p) >> !('/' >> !(int_p))) >> *(space_p-eol_p) >>
        eol_p;
      BOOST_SPIRIT_DEBUG_NODE(indexCount_p);
      
      rule<> count_p = *(vertexCount_p | normalCount_p | indexCount_p | unknown_p);
      BOOST_SPIRIT_DEBUG_NODE(count_p);

      if (parse(objData.c_str(), count_p).full)
      {
        if(normalCount && (vertexCount != normalCount)) {throw std::runtime_error("number of normals doesn't match number of vertices");}
        mesh->resetSize(vertexCount, indexCount);
        DOUT("vertexCount      : " << vertexCount);
        DOUT("normalCount      : " << normalCount);
        DOUT("indexCount       : " << indexCount);


        Vec3 vec3;
        rule<> vec3_p = 
          real_p[assign_a(vec3.x)] >> +space_p >>
          real_p[assign_a(vec3.y)] >> +space_p >>
          real_p[assign_a(vec3.z)] >> !(+space_p >> real_p) >> *(space_p-eol_p);

        rule<> vertex_p = 
          ch_p('v') >> +space_p >> 
            vec3_p[MeshSet<Vec3>(vtxCount, mesh, vec3, UT_position)][increment_a(vtxCount)] >>
          eol_p;
        BOOST_SPIRIT_DEBUG_NODE(vertex_p);

        rule<> normal_p = 
          str_p("vn") >> +space_p >> 
            vec3_p[MeshSet<Vec3>(nrmCount, mesh, vec3, UT_normal)][increment_a(nrmCount)] >>
          eol_p;
        BOOST_SPIRIT_DEBUG_NODE(normal_p);

        u16 index;
        rule<> index_p = int_p[assign_a(index)][decrement_a(index)] >> !('/' >> !(int_p) >> !('/' >> !(int_p)));
        BOOST_SPIRIT_DEBUG_NODE(index_p);
        rule<> indexAssign_p = index_p[MeshSet<uint16_t>(idxCount, mesh, index, UT_index)][increment_a(idxCount)];
        BOOST_SPIRIT_DEBUG_NODE(indexAssign_p);
        rule<> face_p =
          ch_p('f') >> +space_p >>
            indexAssign_p >> +space_p >>
            indexAssign_p >> +space_p >>
            indexAssign_p >> *(space_p-eol_p) >>
          eol_p;
        BOOST_SPIRIT_DEBUG_NODE(face_p);
        
        rule<> assign_p = *(vertex_p | normal_p | face_p | unknown_p);
        BOOST_SPIRIT_DEBUG_NODE(assign_p);

        if (parse(objData.c_str(), assign_p).full)
        {
          mesh->indexBuffer->drawMode = GL_TRIANGLES;

          if(!normalCount)
          {
            // assuming triangles
//            DOUT("no normals");
            Vec3* vertexNormals  = new Vec3[vertexCount];
            Vec3 p1, p2, p3;
            Vec3 v1, v2;
            Vec3 n;
            // caluclate tri normals, add to vertex normals
            for(u16 i=0; i<indexCount; i+=3)
            {
              u16 i1 = mesh->indexBuffer->getAsU32(i, UT_index);
              u16 i2 = mesh->indexBuffer->getAsU16(i+1, UT_index);
              u16 i3 = mesh->indexBuffer->getAsU16(i+2, UT_index);
              p1 = mesh->vertexBuffer->getAsVec3(i1, UT_position);
              p2 = mesh->vertexBuffer->getAsVec3(i2, UT_position);
              p3 = mesh->vertexBuffer->getAsVec3(i3, UT_position);
              v1 = p2-p1;
              v2 = p3-p2;
              n = cross(v1, v2);
              normalise(n);
              // vertices were nulled in constructor so we can just add
              vertexNormals[i1] += n;
              vertexNormals[i2] += n;
              vertexNormals[i3] += n;
            }
            // normalize all vertex normals to create final vnormal from sum of tri normals
            for(unsigned int i = 0; i < vertexCount; ++i)
            {
              normalise(vertexNormals[i]);
              mesh->vertexBuffer->set(i, UT_normal, vertexNormals[i]);
            }
            delete [] vertexNormals;
          }
        }
      }
      
      writeVertexBuffer(mesh, lost::fs::Path("/Users/tony/mesh.vb"));
      writeIndexBuffer(mesh, lost::fs::Path("/Users/tony/mesh.ib"));
      
      return mesh;
    }

  }
}
