#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/gl/Texture.h"

#include "lost/lua/bindings/LostGLTexture.h"

using namespace boost;
using namespace luabind;
using namespace lost::gl;
using namespace lost::resource;
using namespace lost::bitmap;

namespace lost
{
  namespace gl
  {
    void TextureInit(Texture* texture, GLint p1, GLenum p2, GLsizei p3, GLsizei p4, GLint p5, GLenum p6, GLenum p7)
    {
      texture->init(p1, p2, p3, p4, p5, p6, p7, 0);
    }
  }
}

namespace lost
{
  namespace lua
  {
    void LostGLTexture(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<Texture, shared_ptr<Texture> >("Texture")
            .def(constructor<>())
            .def(constructor<shared_ptr<lost::bitmap::Bitmap> >())
            .def("bind", &Texture::bind)
            .def("init", (void(Texture::*)(shared_ptr<File>, const Texture::Params&))&Texture::init)
            .def("init", (void(Texture::*)(shared_ptr<Bitmap>, const Texture::Params&))&Texture::init)
            .def("init", (void(*)(Texture*, GLint, GLenum, GLsizei, GLsizei, GLint, GLenum, GLenum))&TextureInit)
            .def("filter", &Texture::filter)
            .def("wrap", &Texture::wrap)
          .scope
          [
            class_<Texture::Params, shared_ptr<Texture::Params> >("Params")
              .def(constructor<>())
              .def_readwrite("level", &Texture::Params::level)
              .def_readwrite("internalFormat", &Texture::Params::internalFormat)
              .def_readwrite("border", &Texture::Params::border)
              .def_readwrite("format", &Texture::Params::format)
              .def_readwrite("type", &Texture::Params::type)
              .def_readwrite("wrapS", &Texture::Params::wrapS)
              .def_readwrite("wrapT", &Texture::Params::wrapT)
              .def_readwrite("minFilter", &Texture::Params::minFilter)
              .def_readwrite("magFilter", &Texture::Params::magFilter)
              .def_readwrite("sizeHint", &Texture::Params::sizeHint)
          ]
        ]
      ];
    }
  }
}
