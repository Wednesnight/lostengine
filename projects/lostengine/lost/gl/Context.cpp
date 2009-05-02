#include "lost/gl/Context.h"
#include "lost/lgl/lgl.h"
#include "lost/camera/Camera.h"
#include "lost/gl/ShaderProgram.h"
#include "lost/mesh/Mesh.h"
#include "lost/gl/FrameBuffer.h"

using namespace lost::mesh;

bool getBoolParam(GLenum pname)
{
  GLboolean result;
  glGetBooleanv(pname, &result);GLDEBUG;
  return result ? true : false;
}

int getIntParam(GLenum pname)
{
  int result;
  glGetIntegerv(pname, &result);GLDEBUG;
  return result;
}

template<typename T>
T getParam(GLenum pname);

template<> bool getParam(GLenum pname) { return getBoolParam(pname);GLDEBUG; }
template<> int getParam(GLenum pname) { return getIntParam(pname);GLDEBUG; }


#define CLIENTSTATE(member, newstate, pname)  \
if(member != newstate) \
{ \
  if(newstate) \
  { \
    glEnableClientState(pname);GLDEBUG; \
  } \
  else \
  { \
    glDisableClientState(pname);GLDEBUG; \
  } \
  member = newstate; \
}

#define SERVERSTATE(member, newstate, pname)  \
if(member != newstate) \
{ \
  if(newstate) \
  { \
    glEnable(pname);GLDEBUG; \
  } \
  else \
  { \
    glDisable(pname);GLDEBUG; \
  } \
  member = newstate; \
}

namespace lost
{
  namespace gl
  {

    Context::Context()
    {
      initialize();
      
      // this is only true on desktop systems. 
      // on systems like the iPhone, where you have to create the default buffer yourself, you
      // have to set the default in the context, after creation
      m_defaultFrameBuffer = 0; 

      vertexArrayEnabled = getParam<bool>(GL_VERTEX_ARRAY);
      normalArrayEnabled = getParam<bool>(GL_NORMAL_ARRAY);
      texCoordArrayEnabled = getParam<bool>(GL_TEXTURE_COORD_ARRAY);
      indexArrayEnabled = getParam<bool>(GL_INDEX_ARRAY);
      depthTestEnabled = getParam<bool>(GL_DEPTH_TEST);  
      blendEnabled = getParam<bool>(GL_BLEND);
      currentBlendFuncSource = getParam<int>(GL_BLEND_SRC);
      currentBlendFuncDestination = getParam<int>(GL_BLEND_DST);
      scissorEnabled = getParam<bool>(GL_SCISSOR_TEST);
      texture2DEnabled = getParam<bool>(GL_TEXTURE_2D);
      currentActiveTexture = getParam<int>(GL_ACTIVE_TEXTURE);
      currentModelTransform.initIdentity();
    }
    
    Context::~Context()
    {
      finalize();
    }

    void Context::bindFramebuffer(GLuint fbo)
    {
      lglBindFramebuffer(LGL_FRAMEBUFFER, fbo);GLDEBUG_THROW;        
    }
    
    void Context::bindDefaultFramebuffer()
    {
      bindFramebuffer(m_defaultFrameBuffer);
    }

    void Context::defaultFramebuffer(GLuint fbo)
    {
      m_defaultFrameBuffer = fbo;
    }
    
    GLuint Context::defaultFramebuffer()
    {
      return m_defaultFrameBuffer;
    }

    void Context::frameBuffer(FrameBufferPtr& fbo)
    {
      bindFramebuffer(fbo->buffer);
    }

    void Context::defaultFrameBuffer()
    {
      bindDefaultFramebuffer();
    }

    // FIXME: replace macros by dedficated functions that operate on member reference
    void Context::vertexArray(bool enable) { CLIENTSTATE(vertexArrayEnabled, enable, GL_VERTEX_ARRAY); }
    void Context::normalArray(bool enable) { CLIENTSTATE(normalArrayEnabled, enable, GL_NORMAL_ARRAY); }
    void Context::colorArray(bool enable) { CLIENTSTATE(colorArrayEnabled, enable, GL_COLOR_ARRAY); }
    void Context::texCoordArray(bool enable){ CLIENTSTATE(texCoordArrayEnabled, enable, GL_TEXTURE_COORD_ARRAY); }
    void Context::indexArray(bool enable){ CLIENTSTATE(indexArrayEnabled, enable, GL_INDEX_ARRAY); }
    void Context::depthTest(bool enable) { SERVERSTATE(depthTestEnabled, enable, GL_DEPTH_TEST); }
    void Context::blend(bool enable) {SERVERSTATE(blendEnabled, enable, GL_BLEND);}
    
    void Context::blendFunc(GLenum src, GLenum dest)
    {
      if((currentBlendFuncSource != src)
         ||
         (currentBlendFuncDestination != dest)
         )
      {
        glBlendFunc(src, dest);
        currentBlendFuncSource = src;
        currentBlendFuncDestination = dest;
      }
    }
    
    void Context::scissor(bool enable) {SERVERSTATE(scissorEnabled, enable, GL_SCISSOR_TEST);}
    void Context::texture2D(bool enable) {SERVERSTATE(texture2DEnabled, enable, GL_TEXTURE_2D);}
    
    void Context::color(const common::Color& col)
    {
      if(currentColor != col)
      {
        glColor4f(col.fv[0], col.fv[1], col.fv[2], col.fv[3]); GLDEBUG; // OpenGL ES compatible 
        currentColor = col;
      }
    }
  
    void Context::clearColor(const common::Color& col)
    {
      if(currentClearColor != col)
      {
        glClearColor(col.r(), col.g(), col.b(), col.a()); GLDEBUG;
        currentClearColor = col;
      }
    }

    // only update if the new cam is either a new one or the same one, but with the dirty flag set. 
    void Context::camera(camera::CameraPtr cam)
    {
      if (currentCam != cam || currentCam->needsUpdate)
      {
        math::Rect viewport = cam->viewport();
        glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
        glMatrixMode(GL_PROJECTION);GLDEBUG;
        glLoadMatrixf(cam->matrix().m);GLDEBUG;
        if (currentCam != cam) currentCam = cam;
      }
    }
    
    void Context::modelTransform(const math::Matrix& inTransform)
    {
      if(currentModelTransform != inTransform)
      {
        glMatrixMode(GL_MODELVIEW);GLDEBUG;
        glLoadMatrixf(inTransform.m);GLDEBUG;
        currentModelTransform = inTransform;
      }
    }
    
    void Context::clear(GLbitfield flags) { glClear(flags);GLDEBUG; }
    
    void Context::activeTexture(GLenum tex)
    {
      if(currentActiveTexture != tex)
      {
        glActiveTexture(tex);GLDEBUG;
        currentActiveTexture = tex;
      }
    }
    
    void Context::bindActiveTextures(const std::vector<TexturePtr>& textures)
    {
      if(textures.size() > 0)
      {
        uint32_t num = textures.size();
        for(uint32_t i=0; i<num; ++i)
        {
          activeTexture(GL_TEXTURE0+i); // the standard guarantees GL_TEXTUREi = GL_TEXTURE0+i
          textures[0]->bind();
        }
        activeTexture(GL_TEXTURE0); // reset 
      }
    }
    
    void Context::shader(ShaderProgramPtr prog)
    {
      if(currentShader && !prog)
        currentShader->disable();
      currentShader = prog;
      if(currentShader)
        currentShader->enable();
    }
    
    
    void Context::material(MaterialPtr mat)
    {
      color(mat->color);
      if(mat->textures.size()>0)
      {
        bindActiveTextures(mat->textures);
        texture2D(true);
      }
      else
      {
        texture2D(false);
      }
      shader(mat->shader);
    }
    
    
    void Context::draw(MeshPtr mesh)
    {
      gl::Buffer* ib = mesh->getIndexBuffer();
      gl::Buffer* vb = mesh->getVertexBuffer();
      gl::Buffer* nb = mesh->getNormalBuffer();
      gl::Buffer* cb = mesh->getColorBuffer();
      gl::Buffer* tcb = mesh->getTexCoordBuffer();
      
      vertexArray(true);
      vb->bindVertexPointer();
      if(nb)
      {
        normalArray(true);
        nb->bindNormalPointer();
      }
      else
      {
        normalArray(false);
      }
      
      if(cb)
      {
        colorArray(true);
        cb->bindColorPointer();
      }
      else
      {
        colorArray(false);
      }
      
      if(tcb)
      {
        texCoordArray(true);
        tcb->bindTexCoordPointer();
      }
      else
      {
        texCoordArray(false);
      }
      
      if(mesh->material)
        material(mesh->material);
      
      modelTransform(mesh->modelTransform);
      if (ib)
      {
        indexArray(true);
        ib->bind();
        ib->drawElements(mesh->drawMode);
      }
      else
      {
        indexArray(false);
        vb->drawArrays(mesh->drawMode);
      }
    }

  }
}
