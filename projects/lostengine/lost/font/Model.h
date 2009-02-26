#ifndef LOST_FONT_MODEL
#define LOST_FONT_MODEL

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include "lost/math/Vec2.h"
#include "lost/application/gl/Canvas.h"
#include "lost/application/gl/Context.h"
#include "lost/application/gl/State.h"
#include "lost/gl/Texture.h"

namespace lost
{

  namespace font
  {

    struct Model
    {
      Model();
      virtual ~Model();

      void render(const boost::shared_ptr<application::gl::Canvas>& canvas);

      boost::uint32_t vertexCount;
      boost::uint32_t indexCount;
      boost::shared_array<float>    vertices;
      boost::shared_array<float>    texcoords;
      boost::shared_array<boost::uint8_t>  indices;
      application::gl::SharedState renderState;
      boost::shared_ptr<gl::Texture>  texture;
      lost::math::Vec2              size; // width and height in pixel of the rendered string. baseline is at y=0
    };

  }
}

#endif
