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
#include <slub/globals.h>
#include <stdexcept>
#pragma warning(disable:4244) // warning C4244: 'argument' : conversion from 'const double' to 'float', possible loss of data

#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"

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
      
      slub::table meta = bundle->require(fs::Path("meta.lua"), state);
      slub::table vblayout = meta["vblayout"];
      slub::table iblayout = meta["iblayout"];
      slub::reference vbname = meta["vbname"];
      slub::reference ibname = meta["ibname"];
      
      DOUT("vblayout "<<vblayout.isNil());
      DOUT("iblayout "<<iblayout.isNil());
      DOUT("vbname "<<vbname.cast<string>());
      DOUT("ibname "<<ibname.cast<string>());

      common::DataPtr ibdata = bundle->load(fs::Path(ibname.cast<string>()));
      common::DataPtr vbdata = bundle->load(fs::Path(vbname.cast<string>()));
      
      DOUT("vbsize "<<vbdata->size);
      DOUT("ibsize "<<ibdata->size);
      
      BufferLayout vbl;
      for(uint32_t i=0; i<vblayout.maxn(); ++i)
      {
        slub::table entry = vblayout[i+1];
        ElementType et = (slub::globals(state)["gl"][entry[1]]).cast<ElementType>();
        UsageType ut = slub::globals(state)["gl"][entry[2]].cast<UsageType>();
//        bool normalise = entry[3].cast<bool>();
        vbl.add(et, ut);
      }
      
      ElementType indexType = slub::globals(state)["gl"][iblayout[1][1]].cast<ElementType>();
      
      MeshPtr result(new Mesh(vbl, indexType));
      result->vertexBuffer->reset(vbdata);
      result->indexBuffer->reset(ibdata);

      return result;
    }
  }
}
