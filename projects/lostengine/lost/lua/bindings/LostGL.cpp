#include "lost/lua/lua.h"
#include "lost/lua/bindings/LostGL.h"

#include "lost/gl/Context.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/gl/RenderBuffer.h"
#include "lost/gl/ShaderProgram.h"
#include "lost/gl/Shader.h"
#include "lost/gl/VertexShader.h"
#include "lost/gl/FragmentShader.h"
#include "lost/gl/Uniform.h"
#include "lost/gl/UniformBlock.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/gl/Texture.h"
#include "lost/gl/HybridVertexBuffer.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/bitmap/Bitmap.h"

#include <stdexcept>

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
      slub::package(state, "lost").package("gl")
        .clazz<Context>("Context")
          .method("makeCurrent", &Context::makeCurrent)
          .method("swapBuffers", &Context::swapBuffers)
          .method("vsync", &Context::vsync)
          .method("multithreaded", &Context::multithreaded)
          .method("writeScreenshot", &Context::writeScreenshot)
          .method("shader",&Context::shader);
    }

    void LostGLBuffer(lua_State* state)
    {
      slub::package gl = slub::package(state, "lost").package("gl");

      gl.clazz<BufferLayout>("BufferLayout")
          .constructor()
          .method("add", &BufferLayout::add);

      gl.clazz<HybridVertexBuffer>("HybridVertexBuffer");

      gl.clazz<HybridIndexBuffer>("HybridIndexBuffer")
        .field("drawMode", &HybridIndexBuffer::drawMode);

      slub::package(state, "gl")
        .constant("ET_u8", ET_u8)
        .constant("ET_u16", ET_u16)
        .constant("ET_u32", ET_u32)
        .constant("ET_f32", ET_f32)  
        .constant("ET_vec2_u8", ET_vec2_u8)
        .constant("ET_vec2_u16", ET_vec2_u16)
        .constant("ET_vec2_u32", ET_vec2_u32)
        .constant("ET_vec2_f32", ET_vec2_f32) 
        .constant("ET_vec3_u8", ET_vec3_u8)
        .constant("ET_vec3_u16", ET_vec3_u16)
        .constant("ET_vec3_u32", ET_vec3_u32)
        .constant("ET_vec3_f32", ET_vec3_f32) 
        .constant("ET_vec4_u8", ET_vec4_u8)
        .constant("ET_vec4_u16", ET_vec4_u16)
        .constant("ET_vec4_u32", ET_vec4_u32)
        .constant("ET_vec4_f32", ET_vec4_f32)
        .constant("ET_mat4x4_f32", ET_mat4x4_f32)

        .constant("UT_unused", UT_unused)
        .constant("UT_index", UT_index)
        .constant("UT_position", UT_position)
        .constant("UT_texcoord0", UT_texcoord0)
        .constant("UT_texcoord1", UT_texcoord1)
        .constant("UT_texcoord2", UT_texcoord2)
        .constant("UT_texcoord3", UT_texcoord3)
        .constant("UT_normal", UT_normal)
        .constant("UT_color", UT_color)
        .constant("UT_vertexAttrib0", UT_vertexAttrib0)
        .constant("UT_vertexAttrib1", UT_vertexAttrib1)
        .constant("UT_vertexAttrib2", UT_vertexAttrib2)
        .constant("UT_vertexAttrib3", UT_vertexAttrib3)
        .constant("UT_vertexAttrib4", UT_vertexAttrib4)
        .constant("UT_vertexAttrib5", UT_vertexAttrib5)
        .constant("UT_vertexAttrib6", UT_vertexAttrib6)
        .constant("UT_vertexAttrib7", UT_vertexAttrib7);
    }

    TexturePtr FrameBuffer_colorTexture(FrameBuffer* framebuffer, uint8_t index)
    {
      if (framebuffer->colorBuffers.find(index) != framebuffer->colorBuffers.end())
      {
        return framebuffer->colorBuffers[index]->texture;
      }
      else
      {
        throw std::runtime_error("index out of range");
      }
    }

    TexturePtr FrameBuffer_depthTexture(FrameBuffer* framebuffer)
    {
      return framebuffer->depthBuffer->texture;
    }
    
    void LostGLFrameBuffer(lua_State* state)
    {
      slub::package(state, "lost").package("gl")
        .clazz<FrameBuffer>("FrameBuffer")
          .method("colorTexture", &FrameBuffer_colorTexture)
          .method("depthTexture", &FrameBuffer_depthTexture)
          .method("resize", &FrameBuffer::resize)
          .function("create", &FrameBuffer::create)
          .function("setup", &FrameBuffer::setup);
    }
    
    void LostGLGL(lua_State* state)
    {
      slub::package(state, "gl")
#if TARGET_OPENGL
      .constant("GL_REPEAT", GL_REPEAT)
      .constant("GL_DEPTH_COMPONENT", GL_DEPTH_COMPONENT)
      .constant("GL_DEPTH_COMPONENT16", GL_DEPTH_COMPONENT16)
      .constant("GL_DEPTH_COMPONENT24", GL_DEPTH_COMPONENT24)
      .constant("GL_DEPTH_COMPONENT32", GL_DEPTH_COMPONENT32)
      .constant("GL_FRAMEBUFFER_COMPLETE_EXT", GL_FRAMEBUFFER_COMPLETE_EXT)
      .constant("GL_RGBA8", GL_RGBA8)
      .constant("GL_SCISSOR_BIT", GL_SCISSOR_BIT)
      .constant("GL_VIEWPORT_BIT", GL_VIEWPORT_BIT)
#endif
      .constant("GL_CLAMP_TO_EDGE", GL_CLAMP_TO_EDGE)
      .constant("GL_COLOR_BUFFER_BIT", GL_COLOR_BUFFER_BIT)
      .constant("GL_DEPTH_BUFFER_BIT", GL_DEPTH_BUFFER_BIT)
      .constant("GL_DEPTH_TEST", GL_DEPTH_TEST)
      .constant("GL_FLOAT", GL_FLOAT)
//      .constant("GL_LINE_SMOOTH", GL_LINE_SMOOTH)
      .constant("GL_LINEAR", GL_LINEAR)
//      .constant("GL_MODELVIEW", GL_MODELVIEW)
      .constant("GL_NEAREST", GL_NEAREST)
      .constant("GL_ONE_MINUS_SRC_ALPHA", GL_ONE_MINUS_SRC_ALPHA)
//      .constant("GL_POINT_SMOOTH", GL_POINT_SMOOTH)
      .constant("GL_RGB", GL_RGB)
      .constant("GL_RGBA", GL_RGBA)
      .constant("GL_SCISSOR_TEST", GL_SCISSOR_TEST)
      .constant("GL_SRC_ALPHA", GL_SRC_ALPHA)
      .constant("GL_TEXTURE_2D", GL_TEXTURE_2D)
      .constant("GL_LINE_LOOP", GL_LINE_LOOP)
      .constant("GL_LINES", GL_LINES)
      .constant("GL_POINTS", GL_POINTS)
      .constant("GL_TRIANGLES", GL_TRIANGLES)
      .constant("GL_UNSIGNED_BYTE", GL_UNSIGNED_BYTE)
//      .constant("GL_VERTEX_ARRAY", GL_VERTEX_ARRAY)
//      .constant("GL_MODELVIEW", GL_MODELVIEW)
      .constant("GL_FRONT", GL_FRONT)
      .constant("GL_BACK", GL_BACK)
      .constant("GL_FRONT_AND_BACK", GL_FRONT_AND_BACK)

      // types
#if !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE && !defined ANDROID
      .constant("GL_BOOL", GL_BOOL)
      .constant("GL_BOOL_VEC2", GL_BOOL_VEC2)
      .constant("GL_BOOL_VEC3", GL_BOOL_VEC3)
      .constant("GL_BOOL_VEC4", GL_BOOL_VEC4)
      .constant("GL_INT", GL_INT)
      .constant("GL_INT_VEC2", GL_INT_VEC2)
      .constant("GL_INT_VEC3", GL_INT_VEC3)
      .constant("GL_INT_VEC4", GL_INT_VEC4)
      .constant("GL_FLOAT", GL_FLOAT)
      .constant("GL_FLOAT_VEC2", GL_FLOAT_VEC2)
      .constant("GL_FLOAT_VEC3", GL_FLOAT_VEC3)
      .constant("GL_FLOAT_VEC4", GL_FLOAT_VEC4)
      .constant("GL_FLOAT_MAT2", GL_FLOAT_MAT2)
      .constant("GL_FLOAT_MAT2x3", GL_FLOAT_MAT2x3)
      .constant("GL_FLOAT_MAT2x4", GL_FLOAT_MAT2x4)
      .constant("GL_FLOAT_MAT3", GL_FLOAT_MAT3)
      .constant("GL_FLOAT_MAT3x2", GL_FLOAT_MAT3x2)
      .constant("GL_FLOAT_MAT3x4", GL_FLOAT_MAT3x4)
      .constant("GL_FLOAT_MAT4", GL_FLOAT_MAT4)
      .constant("GL_FLOAT_MAT4x2", GL_FLOAT_MAT4x2)
      .constant("GL_FLOAT_MAT4x3", GL_FLOAT_MAT4x3)
      .constant("GL_SAMPLER_1D", GL_SAMPLER_1D)
      .constant("GL_SAMPLER_2D", GL_SAMPLER_2D)
      .constant("GL_SAMPLER_3D", GL_SAMPLER_3D)
      .constant("GL_SAMPLER_CUBE", GL_SAMPLER_CUBE)
      .constant("GL_SAMPLER_1D_SHADOW", GL_SAMPLER_1D_SHADOW)
      .constant("GL_SAMPLER_2D_SHADOW", GL_SAMPLER_2D_SHADOW);
#endif
    }

    void LostGLRenderBuffer(lua_State* state)
    {
      slub::package(state, "lost").package("gl")
        .clazz<RenderBuffer>("RenderBuffer")
          .constructor()
          .method("enable", &RenderBuffer::enable)
          .method("disable", &RenderBuffer::disable)
          .method("storage", &RenderBuffer::storage);
    }

    slub::reference ShaderProgram_uniformMap(ShaderProgram* shaderProgram, lua_State* state)
    {
      lua_newtable(state);
      ShaderProgram::UniformMap& uniformMap = shaderProgram->uniformMap();
      if (uniformMap.size() > 0)
      {
        for (ShaderProgram::UniformMap::iterator idx = uniformMap.begin(); idx != uniformMap.end(); ++idx)
        {
          slub::converter<string>::push(state, idx->first);
          slub::converter<Uniform>::push(state, idx->second);
          lua_settable(state, -2);
        }
      }
      return slub::reference(state);
    }

    void LostGLShaderProgram(lua_State* state)
    {
      slub::package(state, "lost").package("gl")
        .function("loadShader", loadShader)
        .function("buildShader", buildShader)

        .clazz<ShaderProgram>("ShaderProgram")
          .method("enable", &ShaderProgram::enable)
          .method("disable", &ShaderProgram::disable)
          .method("attach", &ShaderProgram::attach)
          .method("link", &ShaderProgram::link)
          .method("linked", &ShaderProgram::linked)
          .method("buildUniformMap", &ShaderProgram::buildUniformMap)
          .method("buildVertexAttributeMap", &ShaderProgram::buildVertexAttributeMap)
          .method("log", &ShaderProgram::log)
          .method("validate", &ShaderProgram::validate)
          .method("validated", &ShaderProgram::validated)
          .method("numericalType", &ShaderProgram::numericalType)
          .method("setInt", (void(ShaderProgram::*)(const string& inName, GLint inVal)) &ShaderProgram::setInt)
          .method("setFloat", (void(ShaderProgram::*)(const string& inName, float inVal)) &ShaderProgram::setFloat)
          .method("setBool", (void(ShaderProgram::*)(const string& inName, bool inVal)) &ShaderProgram::setBool)
          .method("set", (void(ShaderProgram::*)(const string& inName, const lost::common::Color& inVal)) &ShaderProgram::set)
          .method("set", (void(ShaderProgram::*)(const string& inName, const lost::math::Vec4& inVal))  &ShaderProgram::set)
          .method("set", (void(ShaderProgram::*)(const string& inName, const lost::math::Vec2& inVal)) &ShaderProgram::set)
          .method("set", (void(ShaderProgram::*)(const string& inName, const lost::math::Vec3& inVal)) &ShaderProgram::set)
          .method("set", (void(ShaderProgram::*)(const string& inName, const lost::math::Matrix& inVal)) &ShaderProgram::set)
          .method("uniformMap", &ShaderProgram_uniformMap)
          .method("hasUniform", &ShaderProgram::hasUniform)
          .function("create", &ShaderProgram::create);
    }

    void LostGLShader(lua_State* state)
    {
      slub::package(state, "lost").package("gl")
        .clazz<Shader>("Shader")
          .method("source", &Shader::source)
          .method("compile", &Shader::compile)
          .method("compiled", &Shader::compiled)
          .method("log", &Shader::log);
    }

    void LostGLVertexShader(lua_State* state)
    {
      slub::package(state, "lost").package("gl")
        .clazz<VertexShader>("VertexShader")
          .extends<Shader>()
          .function("create", &VertexShader::create);
    }
    
    void LostGLFragmentShader(lua_State* state)
    {
      slub::package(state, "lost").package("gl")
        .clazz<FragmentShader>("FragmentShader")
          .extends<Shader>()
          .function("create", &FragmentShader::create);
    }
    
    void LostGLUniform(lua_State* state)
    {
      slub::package(state, "lost").package("gl")
        .clazz<Uniform>("Uniform")
          .field("name", &Uniform::name)
          .field("index", &Uniform::index)
          .field("glType", &Uniform::glType)
          .field("size", &Uniform::size);
    }

    void LostGLUniformBlock(lua_State* state)
    {
      slub::package(state, "lost").package("gl")
        .clazz<UniformBlock>("UniformBlock")
          .method("setInt", &UniformBlock::setInt)
          .method("setFloat", &UniformBlock::setFloat)
          .method("setBool", &UniformBlock::setBool)
          .method("set", (void(UniformBlock::*)(const string&, const common::Color&))&UniformBlock::set)
          .method("set", (void(UniformBlock::*)(const string&, const math::Vec2&))&UniformBlock::set)
          .method("set", (void(UniformBlock::*)(const string&, const math::Vec3&))&UniformBlock::set)
          .method("set", (void(UniformBlock::*)(const string&, const math::Vec4&))&UniformBlock::set)
          .method("set", (void(UniformBlock::*)(const string&, const math::Matrix&))&UniformBlock::set)
          .function("create", &UniformBlock::create);
    }

    void TextureInit(Texture* texture, GLint p1, GLenum p2, GLsizei p3, GLsizei p4, GLint p5, GLenum p6, GLenum p7)
    {
      texture->init(p1, p2, p3, p4, p5, p6, p7, 0);
    }

    void LostGLTexture(lua_State* state)
    {
      slub::package(state, "lost").package("gl")
        .clazz<Texture>("Texture")
          .method("bind", &Texture::bind)
          .method("init", (void(Texture::*)(const common::DataPtr&, const Texture::Params&))&Texture::init)
          .method("init", (void(Texture::*)(const bitmap::BitmapPtr&, const Texture::Params&))&Texture::init)
          .method("init", (void(Texture::*)(const lost::math::Vec2&, const Texture::Params&))&Texture::init)
          .method("init", (void(*)(Texture*, GLint, GLenum, GLsizei, GLsizei, GLint, GLenum, GLenum))&TextureInit)
          .method("filter", &Texture::filter)
          .method("wrap", &Texture::wrap)
          .field("width", &Texture::width)
          .field("height", &Texture::height)
          .field("dataWidth", &Texture::dataWidth)
          .field("dataHeight", &Texture::dataHeight)
          .function("create", (TexturePtr(*)(const common::DataPtr&, const Texture::Params&))&Texture::create)
          .function("create", (TexturePtr(*)(const lost::math::Vec2&, const Texture::Params&))&Texture::create)
          .function("create", (TexturePtr(*)(const bitmap::BitmapPtr&, const Texture::Params&))&Texture::create);
      
      slub::package(state, "lost").package("gl").package("Texture")
        .clazz<Texture::Params>("Params")
          .constructor()
          .field("level", &Texture::Params::level)
          .field("internalFormat", &Texture::Params::internalFormat)
          .field("border", &Texture::Params::border)
          .field("format", &Texture::Params::format)
          .field("type", &Texture::Params::type)
          .field("wrapS", &Texture::Params::wrapS)
          .field("wrapT", &Texture::Params::wrapT)
          .field("minFilter", &Texture::Params::minFilter)
          .field("magFilter", &Texture::Params::magFilter)
          .field("sizeHint", &Texture::Params::sizeHint);
    }

    void LostGL(lua_State* state)
    {
      LostGLGL(state); // needs to come first since it creates the global gl table
      LostGLBuffer(state);
      LostGLContext(state);
      LostGLFrameBuffer(state);
      LostGLRenderBuffer(state);
      LostGLShaderProgram(state);
      LostGLShader(state);
      LostGLVertexShader(state);
      LostGLFragmentShader(state);
      LostGLUniform(state);
      LostGLUniformBlock(state);
      LostGLTexture(state);
    }

  }
}
