#include <Box2D/Box2D.h>
#include "lost/mesh/forward.h"
#include "lost/rg/forward.h"
#include "lost/gl/forward.h"
#include "lost/resource/forward.h"

namespace lost
{
namespace box2d
{

struct DebugDraw : public b2DebugDraw
{
  DebugDraw(resource::LoaderPtr loader);
  virtual ~DebugDraw();
  
  // call these before/after calling world.DrawDebugData()
  void beginDraw();
  void endDraw();
  
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	virtual void DrawTransform(const b2Transform& xf);

  rg::NodePtr rootNode;
private: 
  resource::LoaderPtr loader;
  gl::ShaderProgramPtr colorShader;
  gl::ShaderProgramPtr shader();
  mesh::MeshPtr createQuad();
  void enqueueAllQuads();
  mesh::MeshPtr dequeueQuad();
  void updateRenderGraph();

  typedef vector<mesh::MeshPtr> MeshVector;
  MeshVector unusedQuads;
  MeshVector quads;
  
  typedef vector<rg::NodePtr> NodeVector;
  typedef map<uint32_t, NodeVector> NodeMap;
  NodeMap numverts2nodevector; // number of vertices -> node vector. each node is a draw node that contains a mesh with the desired number of vertices preallocated
  NodeVector activeNodes; // will be filled by debugdraw calls. These nodes should be added to the rootNode in endDraw()
  rg::NodePtr createNode(uint32_t numVertices);
  rg::NodePtr findInactiveNode(uint32_t numVertices);
  void deactivateAllNodes();
};

}
}