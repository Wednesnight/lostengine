#include "lost/box2d/DebugDraw.h"
#include "lost/common/Logger.h"

namespace lost
{
namespace box2d
{

DebugDraw::DebugDraw()
{
  AppendFlags(e_shapeBit);
  AppendFlags(e_jointBit);
  AppendFlags(e_aabbBit);		
  AppendFlags(e_pairBit);			
  AppendFlags(e_centerOfMassBit);
}

DebugDraw::~DebugDraw()
{
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
  DOUT("vcount: "<<vertexCount);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
  DOUT("vcount: "<<vertexCount);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
  DOUT("");
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
  DOUT("");
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
  DOUT("");
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
  DOUT("");
}

}
}