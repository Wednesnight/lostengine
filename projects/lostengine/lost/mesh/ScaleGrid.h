#ifndef LOST_MESH_SCALEGRID_H
#define LOST_MESH_SCALEGRID_H

#include "lost/mesh/Mesh.h"

namespace lost
{
namespace mesh
{

struct ScaleGrid;
typedef lost::shared_ptr<ScaleGrid> ScaleGridPtr;

/** A ScaleGrid is basically a 2D Quad comprised of a connected 3x3 quad matrix.
 * The corners defined by the left/right/top/bottom parameters are not scaled. Everything else is.
 * The outline of the 3x3 quads will be blown up to match the size of the provided rect.
 * The l/r/t/b measurements should be within the range of the raw pixel data of the texture to
 * achieve meaningful results. Everything else is undefined and will probably look messed up.
 * The specified rect is treated as pixel coordinates in a 2d coordinate system.
 * l,r,t,b are cap sizes in pixels.
 * 0,0 is the bottom left corner.
 */
struct ScaleGrid : public Mesh
{
  uint32_t numVertices;
  uint32_t numIndices;
  uint32_t numTexCoords;

  ScaleGrid(gl::TexturePtr tex,
            const math::Rect& rect,
            float left,
            float right,
            float top,
            float bottom,
            bool flip); // flips texture coordinates vertically if true
  
  void createVertices(const math::Vec2& inSize,
                      float left,
                      float right,
                      float top,
                      float bottom);

  void updateSize(const math::Vec2& inSize,
                      float left,
                      float right,
                      float top,
                      float bottom);

  static ScaleGridPtr create(gl::TexturePtr tex,
                             const math::Rect& rect,
                             float left,
                             float right,
                             float top,
                             float bottom,
                             bool flip)
  {
    return ScaleGridPtr(new ScaleGrid(tex, rect, left, right, top, bottom, flip));
  }
};

}
}

#endif