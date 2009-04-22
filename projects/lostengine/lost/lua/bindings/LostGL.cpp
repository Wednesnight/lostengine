#include "lost/lua/bindings/LostGL.h"
#include "lost/lua/lua.h"

#include "lost/gl/Canvas.h"
#include "lost/gl/Context.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/gl/gl.h"
#include "lost/gl/Mesh.h"
#include "lost/gl/RenderBuffer.h"
#include "lost/gl/ShaderProgram.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/gl/State.h"
#include "lost/gl/Texture.h"

using namespace boost;
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

    void LostGLCanvas(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<Canvas, lost::shared_ptr<Canvas> >("Canvas")
            .def(constructor<const shared_ptr<Context>&, const Rect&>())
            .def(constructor<const shared_ptr<Context>&, const shared_ptr<Camera>&>())
            .def_readonly("context", &Canvas::context)
            .def_readonly("camera", &Canvas::camera)
            .def("clear", &Canvas::clear)
            .def("setColor", &Canvas::setColor)
            .def("drawPoint", (void(Canvas::*)(const Vec2&))&Canvas::drawPoint)
            .def("drawPoint", (void(Canvas::*)(const Vec3&))&Canvas::drawPoint)
            .def("drawLine", (void(Canvas::*)(const Vec2&, const Vec2&))&Canvas::drawLine)
            .def("drawLine", (void(Canvas::*)(const Vec3&, const Vec3&))&Canvas::drawLine)
            .def("drawBezierCurve", &Canvas::drawBezierCurve)
            .def("drawBezierCurveFilled", &Canvas::drawBezierCurveFilled)
            .def("drawRectOutline", &Canvas::drawRectOutline)
            .def("drawRectOutlineRounded", &Canvas::drawRectOutlineRounded)
            .def("drawRectFilled", &Canvas::drawRectFilled)
            .def("drawRectFilledRounded", &Canvas::drawRectFilledRounded)
            .def("drawRectTextured", (void(Canvas::*)(const Rect&, shared_ptr<const Texture>, const Vec2&, const Vec2&, const Vec2&, const Vec2&, bool))&Canvas::drawRectTextured)
            .def("drawRectTextured", (void(Canvas::*)(const Rect&, shared_ptr<const Texture>, bool))&Canvas::drawRectTextured)
            .def("drawMesh2D", &Canvas::drawMesh2D)
            .def("drawMesh3D", &Canvas::drawMesh3D)
            .def("drawAABB", &Canvas::drawAABB)
            .def("drawAxes", &Canvas::drawAxes)
            .def("writeScreenshot", &Canvas::writeScreenshot)
        ]
      ];
    }

    void LostGLContext(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<Context, lost::shared_ptr<Context> >("Context")
            .def("makeCurrent", &Context::makeCurrent)
            .def("swapBuffers", &Context::swapBuffers)
            .def("pushState", &Context::pushState)
            .def("popState", &Context::popState)
            .def("vsync", &Context::vsync)
            .def("multithreaded", &Context::multithreaded)
        ]
      ];
    }

    void LostGLFrameBuffer(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<FrameBuffer, shared_ptr<FrameBuffer> >("FrameBuffer")
            .def(constructor<ContextPtr>())
            .def("enable", &FrameBuffer::enable)
            .def("disable", &FrameBuffer::disable)
            .def("attachDepth", (void(FrameBuffer::*)(shared_ptr<Texture>))&FrameBuffer::attachDepth)
            .def("attachDepth", (void(FrameBuffer::*)(shared_ptr<RenderBuffer>))&FrameBuffer::attachDepth)
            .def("attachColor", (void(FrameBuffer::*)(int, shared_ptr<Texture>))&FrameBuffer::attachColor)
            .def("status", &FrameBuffer::status)
        ]
      ];
    }

    void glDebug()
    {
      GLDEBUG;
    }

    // stupid win32 __stdcall mapping
#ifdef WIN32
  #define LOST_LUA_GL_PREFIX(s) _##s
    
    void _glClearColor(GLclampf p1,GLclampf p2,GLclampf p3, GLclampf p4)
    {
      glClearColor(p1, p2, p3, p4);
    }
    
    void _glClear(GLbitfield p1)
    {
      glClear(p1);
    }
    
    void _glEnable(GLenum p1)
    {
      glEnable(p1);
    }
    
    void _glDisable(GLenum p1)
    {
      glDisable(p1);
    }
    
    void _glEnableClientState(GLenum p1)
    {
      glEnableClientState(p1);
    }
    
    void _glDisableClientState(GLenum p1)
    {
      glDisableClientState(p1);
    }
    
    void _glMatrixMode(GLenum p1)
    {
      glMatrixMode(p1);
    }
    
    void _glLoadIdentity()
    {
      glLoadIdentity();
    }
    
    void _glViewport(GLint p1,GLint p2,GLsizei p3,GLsizei p4)
    {
      glViewport(p1, p2, p3, p4);
    }
    
    void _glLineWidth(GLfloat p1)
    {
      glLineWidth(p1);
    }
    
    void _glTranslatef(GLfloat p1,GLfloat p2,GLfloat p3)
    {
      glTranslatef(p1, p2, p3);
    }
    
    void _glPushAttrib(GLbitfield p1)
    {
      glPushAttrib(p1);
    }
    
    void _glPopAttrib()
    {
      glPopAttrib();
    }
    
    void _glPushMatrix()
    {
      glPushMatrix();
    }
    
    void _glPopMatrix()
    {
      glPopMatrix();
    }
    
    void _glScissor(GLint x, GLint y, GLsizei w, GLsizei h)
    {
      glScissor(x, y, w, h);
    }
#else
  // not win32
  #define LOST_LUA_GL_PREFIX(s) s
#endif
    
    void LostGLGL(lua_State* state)
    {
      module(state, "gl")
      [
        def("GLDEBUG", &glDebug),
        def("glClearColor", &LOST_LUA_GL_PREFIX(glClearColor)),
        def("glClear", &LOST_LUA_GL_PREFIX(glClear)),
        def("glEnable", &LOST_LUA_GL_PREFIX(glEnable)),
        def("glDisable", &LOST_LUA_GL_PREFIX(glDisable)),
        def("glEnableClientState", &LOST_LUA_GL_PREFIX(glEnableClientState)),
        def("glDisableClientState", &LOST_LUA_GL_PREFIX(glDisableClientState)),
        def("glMatrixMode", &LOST_LUA_GL_PREFIX(glMatrixMode)),
        def("glLoadIdentity", &LOST_LUA_GL_PREFIX(glLoadIdentity)),
        def("glViewport", &LOST_LUA_GL_PREFIX(glViewport)),
        def("glLineWidth", &LOST_LUA_GL_PREFIX(glLineWidth)),
        def("glScissor", &LOST_LUA_GL_PREFIX(glScissor)),
        def("glTranslate", &LOST_LUA_GL_PREFIX(glTranslatef)),
        def("glPushMatrix", &LOST_LUA_GL_PREFIX(glPushMatrix)),
        def("glPopMatrix", &LOST_LUA_GL_PREFIX(glPopMatrix))
#if TARGET_OPENGL
        ,def("glPushAttrib", &LOST_LUA_GL_PREFIX(glPushAttrib)),
        def("glPopAttrib", &LOST_LUA_GL_PREFIX(glPopAttrib))
#endif
      ];
      
#if TARGET_OPENGL
      globals(state)["gl"]["GL_CLAMP"] = GL_CLAMP;
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
    }

    void Mesh2DSetFace(Mesh2D* mesh, unsigned int p1, unsigned int p2)
    {
      mesh->setFace(p1, p2);
    }

    void LostGLMesh(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<Mesh2D, shared_ptr<Mesh2D> >("Mesh2D")
            .def(constructor<uint32_t, uint32_t, uint32_t, shared_ptr<Texture> >())
            .def("setVertexCount", &Mesh2D::setVertexCount)
            .def("setVertex", &Mesh2D::setVertex)
            .def("setTexcoordCount", &Mesh2D::setTexcoordCount)
            .def("setTexcoord", &Mesh2D::setTexcoord)
            .def("setFaceCount", &Mesh2D::setFaceCount)
            .def("setFace", &Mesh2DSetFace),
          
          class_<Mesh3D, shared_ptr<Mesh3D> >("Mesh3D")
            .def(constructor<uint32_t, uint32_t, uint32_t, shared_ptr<Texture> >())
            .def("setVertexCount", &Mesh3D::setVertexCount)
            .def("setVertex", &Mesh3D::setVertex)
            .def("setTexcoordCount", &Mesh3D::setTexcoordCount)
            .def("setTexcoord", &Mesh3D::setTexcoord)
            .def("setFaceCount", &Mesh3D::setFaceCount)
            .def("setFace", &Mesh3D::setFace)
        ]
      ];
    }

    void LostGLRenderBuffer(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<RenderBuffer, shared_ptr<RenderBuffer> >("RenderBuffer")
            .def(constructor<>())
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
          class_<ShaderProgram, ShaderProgramPtr >("ShaderProgram")
            .def(constructor<>())
            .def("enable", &ShaderProgram::enable)
            .def("disable", &ShaderProgram::disable)
            .def("log", &ShaderProgram::log)
            .def("validate", &ShaderProgram::validate)
            .def("validated", &ShaderProgram::validated)
            .def("setInt", (void(ShaderProgram::*)(const std::string& inName, GLint inVal)) &ShaderProgram::setInt)
            .def("setFloat", (void(ShaderProgram::*)(const std::string& inName, float inVal)) &ShaderProgram::setFloat)
            .def("set", (void(ShaderProgram::*)(const std::string& inName, const lost::common::Color& inVal)) &ShaderProgram::set)
            .def("set", (void(ShaderProgram::*)(const std::string& inName, const lost::math::Vec4& inVal))  &ShaderProgram::set)
            .def("set", (void(ShaderProgram::*)(const std::string& inName, const lost::math::Vec2& inVal)) &ShaderProgram::set)
            .def("set", (void(ShaderProgram::*)(const std::string& inName, const lost::math::Vec3& inVal)) &ShaderProgram::set)            
        ]
      ];
    }    

    void LostGLState(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<gl::State, StatePtr>("State")
            .def("param", &gl::State::param)
            .scope
            [
              def("create", (StatePtr(*)())&gl::State::create),
              def("create", (StatePtr(*)(const ParamPtr&))&gl::State::create),
              def("create", (StatePtr(*)(const ParamPtr&, const ParamPtr&))&gl::State::create),
              def("create", (StatePtr(*)(const ParamPtr&, const ParamPtr&, const ParamPtr&))&gl::State::create),
              def("create", (StatePtr(*)(const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&))&gl::State::create),
              def("create", (StatePtr(*)(const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&))&gl::State::create),
              def("create", (StatePtr(*)(const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&))&gl::State::create),
              def("create", (StatePtr(*)(const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&))&gl::State::create)
            ],

          class_<gl::AlphaTest, ParamPtr>("AlphaTest")
            .scope
            [
              def("create", (ParamPtr(*)(bool))&gl::AlphaTest::create)
            ],
          
          class_<gl::DepthTest, ParamPtr>("DepthTest")
            .scope
            [
              def("create", (ParamPtr(*)(bool))&gl::DepthTest::create)
            ],
          
          class_<gl::Texture2D, ParamPtr>("Texture2D")
            .scope
            [
              def("create", (ParamPtr(*)(bool))&gl::Texture2D::create)
            ],
          
          class_<gl::Blend, ParamPtr>("Blend")
            .scope
            [
              def("create", (ParamPtr(*)(bool))&gl::Blend::create)
            ],
          
          class_<gl::Scissor, ParamPtr>("Scissor")
            .scope
            [
              def("create", (ParamPtr(*)(bool))&gl::Scissor::create)
            ],
          
          class_<gl::NormalArray, ParamPtr>("NormalArray")
            .scope
            [
              def("create", (ParamPtr(*)(bool))&gl::NormalArray::create)
            ],
          
          class_<gl::VertexArray, ParamPtr>("VertexArray")
            .scope
            [
              def("create", (ParamPtr(*)(bool))&gl::VertexArray::create)
            ],
          
          class_<gl::TextureArray, ParamPtr>("TextureArray")
            .scope
            [
              def("create", (ParamPtr(*)(bool))&gl::TextureArray::create)
            ],
          
          class_<gl::ClearColor, ParamPtr>("ClearColor")
            .scope
            [
              def("create", (ParamPtr(*)(const lost::common::Color&))&gl::ClearColor::create)
            ],
          
          class_<gl::BlendFunc, ParamPtr>("BlendFunc")
            .scope
            [
              def("create", (ParamPtr(*)(GLenum, GLenum))&gl::BlendFunc::create)
            ],
          
          class_<gl::ScissorBox, ParamPtr>("ScissorBox")
            .scope
            [
              def("create", (ParamPtr(*)(const math::Rect&))&gl::ScissorBox::create)
            ]
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
          class_<Texture, shared_ptr<Texture> >("Texture")
            .def(constructor<>())
            .def(constructor<shared_ptr<lost::bitmap::Bitmap> >())
            .def("bind", &Texture::bind)
            .def("init", (void(Texture::*)(shared_ptr<File>, const Texture::Params&))&Texture::init)
            .def("init", (void(Texture::*)(shared_ptr<Bitmap>, const Texture::Params&))&Texture::init)
            .def("init", (void(Texture::*)(const lost::math::Vec2&, const Texture::Params&))&Texture::init)
            .def("init", (void(*)(Texture*, GLint, GLenum, GLsizei, GLsizei, GLint, GLenum, GLenum))&TextureInit)
            .def("filter", &Texture::filter)
            .def("wrap", &Texture::wrap)
            .def_readwrite("width", &Texture::width)
            .def_readwrite("height", &Texture::height)
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

    void LostGL(lua_State* state)
    {
      LostGLCanvas(state);
      LostGLContext(state);
      LostGLFrameBuffer(state);
      LostGLGL(state);
      LostGLMesh(state);
      LostGLRenderBuffer(state);
      LostGLShaderProgram(state);
      LostGLState(state);
      LostGLTexture(state);
    }

  }
}
