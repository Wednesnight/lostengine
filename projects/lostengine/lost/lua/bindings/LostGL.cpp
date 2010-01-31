#include "lost/lua/bindings/LostGL.h"
#include "lost/lua/lua.h"

#include "lost/gl/Context.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/gl/RenderBuffer.h"
#include "lost/gl/ShaderProgram.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/gl/Texture.h"

using namespace luabind;
using namespace lost::bitmap;
using namespace lost::camera;
using namespace lost::gl;
using namespace lost::math;
using namespace lost::resource;

namespace lost
{
  namespace lua
  {

    void LostGLContext(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<Context>("Context")
            .def("makeCurrent", &Context::makeCurrent)
            .def("swapBuffers", &Context::swapBuffers)
            .def("vsync", &Context::vsync)
            .def("multithreaded", &Context::multithreaded)
            .def("writeScreenshot", &Context::writeScreenshot)
        ]
      ];
    }

    gl::TexturePtr colorTexture(gl::FrameBufferPtr fb, int num)
    {
      return fb->colorTextures[num];
    }

    void LostGLFrameBuffer(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<FrameBuffer>("FrameBuffer")
            .def("attachDepth", (void(FrameBuffer::*)(TexturePtr))&FrameBuffer::attachDepth)
            .def("attachDepth", (void(FrameBuffer::*)(RenderBufferPtr))&FrameBuffer::attachDepth)
            .def("attachColor", (void(FrameBuffer::*)(int, TexturePtr))&FrameBuffer::attachColor)
            .def("status", &FrameBuffer::status)
            .def("colorTexture", &colorTexture)
            .scope
            [
              def("createFrameBuffer", &FrameBuffer::createFrameBuffer)
            ]
        ]
      ];
    }
    
    void LostGLGL(lua_State* state)
    {
      globals(state)["gl"] = luabind::newtable(state);
#if TARGET_OPENGL
      globals(state)["gl"]["GL_CLAMP"] = GL_CLAMP;
      globals(state)["gl"]["GL_CLAMP"] = GL_REPEAT;
      globals(state)["gl"]["GL_DEPTH_COMPONENT"] = GL_DEPTH_COMPONENT;
      globals(state)["gl"]["GL_DEPTH_COMPONENT16"] = GL_DEPTH_COMPONENT16;
      globals(state)["gl"]["GL_DEPTH_COMPONENT24"] = GL_DEPTH_COMPONENT24;
      globals(state)["gl"]["GL_DEPTH_COMPONENT32"] = GL_DEPTH_COMPONENT32;
      globals(state)["gl"]["GL_FRAMEBUFFER_COMPLETE_EXT"] = GL_FRAMEBUFFER_COMPLETE_EXT;
      globals(state)["gl"]["GL_RGBA8"] = GL_RGBA8;
      globals(state)["gl"]["GL_SCISSOR_BIT"] = GL_SCISSOR_BIT;
      globals(state)["gl"]["GL_VIEWPORT_BIT"] = GL_VIEWPORT_BIT;
#endif
      globals(state)["gl"]["GL_CLAMP_TO_EDGE"] = GL_CLAMP_TO_EDGE;
      globals(state)["gl"]["GL_COLOR_BUFFER_BIT"] = GL_COLOR_BUFFER_BIT;
      globals(state)["gl"]["GL_DEPTH_BUFFER_BIT"] = GL_DEPTH_BUFFER_BIT;
      globals(state)["gl"]["GL_DEPTH_TEST"] = GL_DEPTH_TEST;
      globals(state)["gl"]["GL_FLOAT"] = GL_FLOAT;
      globals(state)["gl"]["GL_LINE_SMOOTH"] = GL_LINE_SMOOTH;
      globals(state)["gl"]["GL_LINEAR"] = GL_LINEAR;
      globals(state)["gl"]["GL_MODELVIEW"] = GL_MODELVIEW;
      globals(state)["gl"]["GL_NEAREST"] = GL_NEAREST;
      globals(state)["gl"]["GL_ONE_MINUS_SRC_ALPHA"] = GL_ONE_MINUS_SRC_ALPHA;
      globals(state)["gl"]["GL_POINT_SMOOTH"] = GL_POINT_SMOOTH;
      globals(state)["gl"]["GL_RGB"] = GL_RGB;
      globals(state)["gl"]["GL_RGBA"] = GL_RGBA;
      globals(state)["gl"]["GL_SCISSOR_TEST"] = GL_SCISSOR_TEST;
      globals(state)["gl"]["GL_SRC_ALPHA"] = GL_SRC_ALPHA;
      globals(state)["gl"]["GL_TEXTURE_2D"] = GL_TEXTURE_2D;
      globals(state)["gl"]["GL_TRIANGLES"] = GL_TRIANGLES;
      globals(state)["gl"]["GL_UNSIGNED_BYTE"] = GL_UNSIGNED_BYTE;
      globals(state)["gl"]["GL_VERTEX_ARRAY"] = GL_VERTEX_ARRAY;
      globals(state)["gl"]["GL_MODELVIEW"] = GL_MODELVIEW;
      globals(state)["gl"]["GL_FLOAT"] = GL_FLOAT;
      globals(state)["gl"]["GL_INT"] = GL_INT;
    }

    void LostGLRenderBuffer(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<RenderBuffer>("RenderBuffer")
            .def("enable", &RenderBuffer::enable)
            .def("disable", &RenderBuffer::disable)
            .def("storage", &RenderBuffer::storage)
        ]
      ];
    }

    void LostGLShaderProgram(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          def("loadShader", loadShader),
          class_<ShaderProgram>("ShaderProgram")
            .def("enable", &ShaderProgram::enable)
            .def("disable", &ShaderProgram::disable)
            .def("log", &ShaderProgram::log)
            .def("validate", &ShaderProgram::validate)
            .def("validated", &ShaderProgram::validated)
            .def("numericalType", &ShaderProgram::numericalType)
            .def("setInt", (void(ShaderProgram::*)(const std::string& inName, GLint inVal)) &ShaderProgram::setInt)
            .def("setFloat", (void(ShaderProgram::*)(const std::string& inName, float inVal)) &ShaderProgram::setFloat)
            .def("set", (void(ShaderProgram::*)(const std::string& inName, const lost::common::Color& inVal)) &ShaderProgram::set)
            .def("set", (void(ShaderProgram::*)(const std::string& inName, const lost::math::Vec4& inVal))  &ShaderProgram::set)
            .def("set", (void(ShaderProgram::*)(const std::string& inName, const lost::math::Vec2& inVal)) &ShaderProgram::set)
            .def("set", (void(ShaderProgram::*)(const std::string& inName, const lost::math::Vec3& inVal)) &ShaderProgram::set)            
        ]
      ];
    }    

    void TextureInit(Texture* texture, GLint p1, GLenum p2, GLsizei p3, GLsizei p4, GLint p5, GLenum p6, GLenum p7)
    {
      texture->init(p1, p2, p3, p4, p5, p6, p7, 0);
    }

    void LostGLTexture(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<Texture>("Texture")
            .scope
            [
              def("create", (TexturePtr(*)(common::DataPtr, const Texture::Params&))&Texture::create),
              def("create", (TexturePtr(*)(const lost::math::Vec2&, const Texture::Params&))&Texture::create),
              def("create", (TexturePtr(*)(bitmap::BitmapPtr, const Texture::Params&))&Texture::create)
            ]
            .def("bind", &Texture::bind)
            .def("init", (void(Texture::*)(common::DataPtr, const Texture::Params&))&Texture::init)
            .def("init", (void(Texture::*)(shared_ptr<Bitmap>, const Texture::Params&))&Texture::init)
            .def("init", (void(Texture::*)(const lost::math::Vec2&, const Texture::Params&))&Texture::init)
            .def("init", (void(*)(Texture*, GLint, GLenum, GLsizei, GLsizei, GLint, GLenum, GLenum))&TextureInit)
            .def("filter", &Texture::filter)
            .def("wrap", &Texture::wrap)
            .def_readwrite("width", &Texture::width)
            .def_readwrite("height", &Texture::height)
            .scope
            [
              class_<Texture::Params>("Params")
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

    void LostGL(lua_State* state)
    {
      LostGLContext(state);
      LostGLFrameBuffer(state);
      LostGLGL(state);
      LostGLRenderBuffer(state);
      LostGLShaderProgram(state);
      LostGLTexture(state);
    }

  }
}
