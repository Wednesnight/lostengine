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
#include "lost/gl/HybridVertexBuffer.h"
#include "lost/gl/HybridIndexBuffer.h"
#include <stdexcept>

using namespace std;
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

    void LostGLBuffer(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<BufferLayout>("BufferLayout")
            .def(constructor<>())
            .def("add", &BufferLayout::add)
        ]
      ];
      globals(state)["gl"]["ET_u8"] =   ET_u8;
      globals(state)["gl"]["ET_u16"] =   ET_u16;
      globals(state)["gl"]["ET_u32"] =   ET_u32;
      globals(state)["gl"]["ET_f32"] =   ET_f32;  
      globals(state)["gl"]["ET_vec2_u8"] =   ET_vec2_u8;
      globals(state)["gl"]["ET_vec2_u16"] =   ET_vec2_u16;
      globals(state)["gl"]["ET_vec2_u32"] =   ET_vec2_u32;
      globals(state)["gl"]["ET_vec2_f32"] =   ET_vec2_f32; 
      globals(state)["gl"]["ET_vec3_u8"] =   ET_vec3_u8;
      globals(state)["gl"]["ET_vec3_u16"] =   ET_vec3_u16;
      globals(state)["gl"]["ET_vec3_u32"] =   ET_vec3_u32;
      globals(state)["gl"]["ET_vec3_f32"] =   ET_vec3_f32; 
      globals(state)["gl"]["ET_vec4_u8"] =   ET_vec4_u8;
      globals(state)["gl"]["ET_vec4_u16"] =   ET_vec4_u16;
      globals(state)["gl"]["ET_vec4_u32"] =   ET_vec4_u32;
      globals(state)["gl"]["ET_vec4_f32"] =   ET_vec4_f32;
      globals(state)["gl"]["ET_mat4x4_f32"] =   ET_mat4x4_f32;

      globals(state)["gl"]["UT_unused"] = UT_unused;
      globals(state)["gl"]["UT_index"] = UT_index;
      globals(state)["gl"]["UT_vertex"] = UT_vertex;
      globals(state)["gl"]["UT_texcoord0"] = UT_texcoord0;
      globals(state)["gl"]["UT_texcoord1"] = UT_texcoord1;
      globals(state)["gl"]["UT_texcoord2"] = UT_texcoord2;
      globals(state)["gl"]["UT_texcoord3"] = UT_texcoord3;
      globals(state)["gl"]["UT_normal"] = UT_normal;
      globals(state)["gl"]["UT_color"] = UT_color;
      globals(state)["gl"]["UT_vertexAttrib0"] = UT_vertexAttrib0;
      globals(state)["gl"]["UT_vertexAttrib1"] = UT_vertexAttrib1;
      globals(state)["gl"]["UT_vertexAttrib2"] = UT_vertexAttrib2;
      globals(state)["gl"]["UT_vertexAttrib3"] = UT_vertexAttrib3;
      globals(state)["gl"]["UT_vertexAttrib4"] = UT_vertexAttrib4;
      globals(state)["gl"]["UT_vertexAttrib5"] = UT_vertexAttrib5;
      globals(state)["gl"]["UT_vertexAttrib6"] = UT_vertexAttrib6;
      globals(state)["gl"]["UT_vertexAttrib7"] = UT_vertexAttrib7;

      
    }

    TexturePtr FrameBuffer_colorTexture(FrameBufferPtr framebuffer, boost::uint8_t index)
    {
      if (framebuffer->colorBuffers.find(index) != framebuffer->colorBuffers.end())
      {
        return framebuffer->colorBuffers[index]->texture;
      }
      else
      {
        throw runtime_error("index out of range");
      }
    }

    void LostGLFrameBuffer(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<FrameBuffer>("FrameBuffer")
            .scope
            [
              def("create", &FrameBuffer::create)
            ]
            .def("colorTexture", &FrameBuffer_colorTexture)
            .def("resize", &FrameBuffer::resize)
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
      globals(state)["gl"]["GL_FRONT"] = GL_FRONT;
      globals(state)["gl"]["GL_BACK"] = GL_BACK;
      globals(state)["gl"]["GL_FRONT_AND_BACK"] = GL_FRONT_AND_BACK;

      // types
      globals(state)["gl"]["GL_INT"]                = GL_BOOL;
      globals(state)["gl"]["GL_BOOL_VEC2"]          = GL_BOOL_VEC2;
      globals(state)["gl"]["GL_BOOL_VEC3"]          = GL_BOOL_VEC3;
      globals(state)["gl"]["GL_BOOL_VEC4"]          = GL_BOOL_VEC4;
      globals(state)["gl"]["GL_INT"]                = GL_INT;
      globals(state)["gl"]["GL_INT_VEC2"]           = GL_INT_VEC2;
      globals(state)["gl"]["GL_INT_VEC3"]           = GL_INT_VEC3;
      globals(state)["gl"]["GL_INT_VEC4"]           = GL_INT_VEC4;
      globals(state)["gl"]["GL_FLOAT"]              = GL_FLOAT;
      globals(state)["gl"]["GL_FLOAT_VEC2"]         = GL_FLOAT_VEC2;
      globals(state)["gl"]["GL_FLOAT_VEC3"]         = GL_FLOAT_VEC3;
      globals(state)["gl"]["GL_FLOAT_VEC4"]         = GL_FLOAT_VEC4;
      globals(state)["gl"]["GL_FLOAT_MAT2"]         = GL_FLOAT_MAT2;
      globals(state)["gl"]["GL_FLOAT_MAT2x3"]       = GL_FLOAT_MAT2x3;
      globals(state)["gl"]["GL_FLOAT_MAT2x4"]       = GL_FLOAT_MAT2x4;
      globals(state)["gl"]["GL_FLOAT_MAT3"]         = GL_FLOAT_MAT3;
      globals(state)["gl"]["GL_FLOAT_MAT3x2"]       = GL_FLOAT_MAT3x2;
      globals(state)["gl"]["GL_FLOAT_MAT3x4"]       = GL_FLOAT_MAT3x4;
      globals(state)["gl"]["GL_FLOAT_MAT4"]         = GL_FLOAT_MAT4;
      globals(state)["gl"]["GL_FLOAT_MAT4x2"]       = GL_FLOAT_MAT4x2;
      globals(state)["gl"]["GL_FLOAT_MAT4x3"]       = GL_FLOAT_MAT4x3;
      globals(state)["gl"]["GL_SAMPLER_1D"]         = GL_SAMPLER_1D;
      globals(state)["gl"]["GL_SAMPLER_2D"]         = GL_SAMPLER_2D;
      globals(state)["gl"]["GL_SAMPLER_3D"]         = GL_SAMPLER_3D;
      globals(state)["gl"]["GL_SAMPLER_CUBE"]       = GL_SAMPLER_CUBE;
      globals(state)["gl"]["GL_SAMPLER_1D_SHADOW"]  = GL_SAMPLER_1D_SHADOW;
      globals(state)["gl"]["GL_SAMPLER_2D_SHADOW"]  = GL_SAMPLER_2D_SHADOW;
    }

    void LostGLRenderBuffer(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<RenderBuffer>("RenderBuffer")
            .def(constructor<>())
            .def("enable", &RenderBuffer::enable)
            .def("disable", &RenderBuffer::disable)
            .def("storage", &RenderBuffer::storage)
        ]
      ];
    }

    object ShaderProgram_parameterMap(object shaderProgramObj)
    {
      ShaderProgramPtr shaderProgram = object_cast<ShaderProgramPtr>(shaderProgramObj);
      object result = newtable(shaderProgramObj.interpreter());
      ShaderProgram::ParameterMap params = shaderProgram->parameterMap();
      if (params.size() > 0)
      {
        for (ShaderProgram::ParameterMap::iterator idx = params.begin(); idx != params.end(); ++idx)
        {
          result[idx->first] = idx->second;
        }
      }
      return result;
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
            .def("parameterMap", &ShaderProgram_parameterMap)
            .scope
            [
              class_<ShaderProgram::Parameter>("Parameter")
                .def_readonly("name", &ShaderProgram::Parameter::name)
                .def_readonly("index", &ShaderProgram::Parameter::index)
                .def_readonly("glType", &ShaderProgram::Parameter::glType)
                .def_readonly("size", &ShaderProgram::Parameter::size)
                .def_readonly("paramType", &ShaderProgram::Parameter::paramType)
            ]
        ]
      ];
      globals(state)["lost"]["gl"]["ShaderProgram"]["Parameter"]["UNDEFINED"] = (int)ShaderProgram::Parameter::UNDEFINED;
      globals(state)["lost"]["gl"]["ShaderProgram"]["Parameter"]["ATTRIBUTE"] = (int)ShaderProgram::Parameter::ATTRIBUTE;
      globals(state)["lost"]["gl"]["ShaderProgram"]["Parameter"]["UNIFORM"]   = (int)ShaderProgram::Parameter::UNIFORM;
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
      LostGLGL(state); // needs to come first since it creates the global gl table
      LostGLBuffer(state);
      LostGLContext(state);
      LostGLFrameBuffer(state);
      LostGLRenderBuffer(state);
      LostGLShaderProgram(state);
      LostGLTexture(state);
    }

  }
}
