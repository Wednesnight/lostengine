#ifndef LOST_FONT_MODEL
#define LOST_FONT_MODEL

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include "lost/math/Vec2.h"
#include "lost/gl/Canvas.h"
#include "lost/gl/Context.h"
#include "lost/gl/State.h"
#include "lost/gl/Texture.h"

namespace lost
{

  namespace font
  {

    struct Model
    {
      Model();
      virtual ~Model();

      void render(const boost::shared_ptr<gl::Canvas>& canvas);

      boost::uint32_t vertexCount;
      boost::uint32_t indexCount;
      boost::shared_array<float>    vertices;
      boost::shared_array<float>    texcoords;
      boost::shared_array<boost::uint8_t>  indices;
      gl::SharedState renderState;
      boost::shared_ptr<gl::Texture>  texture;
      lost::math::Vec2 min; // min in pixel of the rendered string. baseline is at y=0
      lost::math::Vec2 max; // max in pixel of the rendered string. baseline is at y=0
      lost::math::Vec2 size; // min, max, width and height in pixel of the rendered string. baseline is at y=0
    };

  }
}

#endif
