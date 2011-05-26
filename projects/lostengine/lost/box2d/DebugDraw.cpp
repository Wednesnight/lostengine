#include "lost/box2d/DebugDraw.h"
#include "lost/common/Logger.h"
#include "lost/rg/Draw.h"
#include "lost/gl/BufferLayout.h"
#include "lost/mesh/Mesh.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/math/io.h"
#include "lost/gl/gl.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/mesh/Quad.h"

namespace lost
{
namespace box2d
{

using namespace lost::math;

DebugDraw::DebugDraw(resource::LoaderPtr inLoader)
{
  AppendFlags(e_shapeBit);
  AppendFlags(e_jointBit);
  AppendFlags(e_aabbBit);		
  AppendFlags(e_pairBit);			
  AppendFlags(e_centerOfMassBit);
  rootNode = rg::Node::create();
  loader = inLoader;
}

DebugDraw::~DebugDraw()
{
}

void DebugDraw::beginDraw()
{
  enqueueAllQuads();
}

void DebugDraw::endDraw()
{
  updateRenderGraph();
}

void DebugDraw::updateRenderGraph()
{
  // rootNode must contain as many draw nodes as there are quads
  uint32_t numNodes = rootNode->children.size();
  uint32_t numMeshes = quads.size();
  if(numNodes != numMeshes)
  {
    if(numNodes < numMeshes)
    {
      rootNode->add(rg::Draw::create(mesh::MeshPtr()));
    }
    else {
      while(rootNode->children.size() > numMeshes)
      {
        rootNode->removeLast();
      }
    }
  }
  // set meshes of drawnodes to current meshes
  uint32_t idx=0;
  for(std::list<rg::NodePtr>::iterator i=rootNode->children.begin(); i!=rootNode->children.end(); ++i)
  {
    rg::DrawPtr node = static_pointer_cast<rg::Draw>(*i);
    node->mesh = quads[idx];
    ++idx;
  }
}

gl::ShaderProgramPtr DebugDraw::shader()
{
  if(!colorShader)
  {
    colorShader = lost::gl::loadShader(loader,"lost/Resources/glsl/basic_color");
  }
  return colorShader;
}

mesh::MeshPtr DebugDraw::createQuad()
{
  lost::gl::BufferLayout layout;
  layout.add(gl::ET_vec2_f32,gl::UT_position,0);
  
  mesh::MeshPtr result(new mesh::Mesh(layout, gl::ET_u16));
  uint32_t numVertices = 4;
  uint32_t numIndices = 4;
  result->indexBuffer->drawMode = GL_LINE_LOOP;
  result->resetSize(numVertices,numIndices);
  result->material->shader = shader();

  result->setIndex(0,0);
  result->setIndex(1,1);
  result->setIndex(2,2);
  result->setIndex(3,3);
  
  return result;
}

void DebugDraw::enqueueAllQuads()
{
  for(MeshVector::iterator i=quads.begin(); i!=quads.end(); ++i)
  {
    unusedQuads.push_back(*i);
  }
  quads.clear();
}

mesh::MeshPtr DebugDraw::dequeueQuad()
{
  mesh::MeshPtr result;
  
  if(unusedQuads.size() > 0)
  {
    result = unusedQuads.back();
    unusedQuads.pop_back();
  }
  else
  {
    result = createQuad();
  }
  
  return result;
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
//  DOUT(vertexCount);
  if(vertexCount = 4)
  {
    mesh::MeshPtr quad = dequeueQuad();
    for(uint32_t i=0; i<vertexCount; ++i)
    {
      math::Vec2 pos(vertices[i].x, vertices[i].y);
      quad->set(i,gl::UT_position,pos);
    }
    quads.push_back(quad);
  }
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
//  DOUT(vertexCount);
  if(vertexCount = 4)
  {
    mesh::MeshPtr quad = dequeueQuad();
    for(uint32_t i=0; i<vertexCount; ++i)
    {
      math::Vec2 pos(vertices[i].x, vertices[i].y);
      quad->set(i,gl::UT_position,pos);
    }
    quads.push_back(quad);
  }
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
//  DOUT("");
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
//  DOUT("");
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
//  DOUT("");
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
//  DOUT("");
}


}
}