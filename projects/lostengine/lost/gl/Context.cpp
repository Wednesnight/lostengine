#include "lost/gl/Context.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/lgl/lgl.h"
#include "lost/camera/Camera.h"
#include "lost/gl/ShaderProgram.h"
#include "lost/mesh/Mesh.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/bitmap/Bitmap.h"

using namespace lost::mesh;
using namespace lost::bitmap;

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
template<> lost::common::Color getParam(GLenum pname)
{
  lost::common::Color result;
  glGetFloatv(pname, result.fv);GLDEBUG;
  return result;
}
template<> lost::math::Rect getParam(GLenum pname)
{
  float rect[4];
  glGetFloatv(pname, rect);GLDEBUG;
  return lost::math::Rect(rect[0], rect[1], rect[2], rect[3]);
}


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

std::map<void*, Context*> glContext2lostGlContext;

    Context::Context()
    {
      initialize();
      glContext2lostGlContext[getCurrentOsSpecific()] = this;
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
      currentTransform.initIdentity();
      currentClearColor = getParam<lost::common::Color>(GL_COLOR_CLEAR_VALUE);
      currentScissorRect = getParam<lost::math::Rect>(GL_SCISSOR_BOX);
      currentBuffer = NULL;
    }
    
    Context::~Context()
    {
      finalize();
      std::map<void*, Context*>::iterator pos;
      pos = glContext2lostGlContext.find(Context::getCurrentOsSpecific());
      if(pos != glContext2lostGlContext.end())
      {
        glContext2lostGlContext.erase(pos);      
      }
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
    
    void Context::scissorRect(const math::Rect& rect)
    {
      if(currentScissorRect != rect)
      {
        glScissor(rect.x, rect.y, rect.width, rect.height);
        currentScissorRect = rect;
      }
    }

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
        glLoadMatrixf(cam->projectionMatrix().m);GLDEBUG;
        if (currentCam != cam) currentCam = cam;
      }
    }
    
    void Context::transform(const math::Matrix& inTransform)
    {
//      if(currentTransform != inTransform)
      {
        glMatrixMode(GL_MODELVIEW);GLDEBUG;
        if(currentCam && currentCam->hasModelViewMatrix)
        {
          glLoadMatrixf(currentCam->mModelViewMatrix.m);GLDEBUG;
          glMultMatrixf(inTransform.m);GLDEBUG;
        }
        else
        {
          glLoadMatrixf(inTransform.m);GLDEBUG;
        }
        currentTransform = inTransform;
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
          textures[i]->bind();
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
      if(mat->blend)
      {
        blend(true);
        blendFunc(mat->blendSrc, mat->blendDest);
      }
      else
      {
        blend(false);
      }
      shader(mat->shader);
    }
        
    void Context::draw(MeshPtr mesh)
    {
      HybridIndexBuffer* ib = mesh->_indexBuffer.get();
      HybridVertexBuffer* vb = mesh->_vertexBuffer.get();

      if(ib->dirty) {ib->upload();}
      if(vb->dirty) {vb->upload();}

      VertexBuffer* gpuBuffer = vb->bufferForUsageType(UT_vertex);
      bind(gpuBuffer);
      vertexArray(true);
      // FIXME: preprocess and store the XXXpointer values in a helper struct in the hybrid buffer
      
      GLint size = vb->hostBuffer->numScalarsForUsageType(UT_vertex);
      GLenum type = vb->hostBuffer->layout.glScalarTypeFromUsageType(UT_vertex);
      GLsizei stride = vb->hostBuffer->layout.stride(UT_vertex); 
      GLvoid* offset = (GLvoid*)vb->hostBuffer->layout.offset(UT_vertex);
            
      glVertexPointer(size,
                      type,
                      stride,
                      offset);GLDEBUG;

      if(vb->hasUsageType(UT_normal))
      {
        VertexBuffer* gpuBuffer = vb->bufferForUsageType(UT_normal);
        bind(gpuBuffer);
        normalArray(true);
        glNormalPointer(vb->hostBuffer->layout.glScalarTypeFromUsageType(UT_normal),
                        vb->hostBuffer->layout.stride(UT_normal),
                        (const GLvoid*)vb->hostBuffer->layout.offset(UT_normal));GLDEBUG;        
      }
      else {
        normalArray(false);
      }

      if(vb->hasUsageType(UT_color))
      {
        VertexBuffer* gpuBuffer = vb->bufferForUsageType(UT_color);
        bind(gpuBuffer);
        colorArray(true);
        glColorPointer(vb->hostBuffer->numScalarsForUsageType(UT_color),
                        vb->hostBuffer->layout.glScalarTypeFromUsageType(UT_color),
                        vb->hostBuffer->layout.stride(UT_color),
                        (const GLvoid*)vb->hostBuffer->layout.offset(UT_color));GLDEBUG;        
      }
      else {
        colorArray(false);
      }

      if(vb->hasUsageType(UT_texcoord0))
      {
        VertexBuffer* gpuBuffer = vb->bufferForUsageType(UT_texcoord0);
        bind(gpuBuffer);
        texCoordArray(true);

        GLint size = vb->hostBuffer->numScalarsForUsageType(UT_texcoord0);
        GLenum type = vb->hostBuffer->layout.glScalarTypeFromUsageType(UT_texcoord0);
        GLsizei stride = vb->hostBuffer->layout.stride(UT_texcoord0); 
        const GLvoid* offset = (const GLvoid*)vb->hostBuffer->layout.offset(UT_texcoord0);
        
        glTexCoordPointer(size,
                          type,
                          stride,
                          offset);GLDEBUG;        
      }
      else {
        texCoordArray(false);
      }
      
      if(mesh->material)
        material(mesh->material);      
      transform(mesh->transform);
      bind(ib->indexBuffer.get());
      glDrawElements(mesh->drawMode, ib->hostBuffer->count, ib->type, 0);GLDEBUG;
    }

    /** Uses glReadPixels to retrieve the current framebuffer data as rgba and saves it
     * as a tga file to the specified file path.
     *
     * @param fullPathName full path name of file to be saved. You must ensure that the location is writable.
     *        
     */
    void Context::writeScreenshot(const std::string& fullPathName,
                                 bool withAlphaChannel)
    {
      GLenum format = withAlphaChannel ? GL_RGBA : GL_RGB;
      Bitmap::Components bitmapFormat = withAlphaChannel ? Bitmap::COMPONENTS_RGBA : Bitmap::COMPONENTS_RGB;
      Bitmap bmp(currentCam->viewport().width,
                 currentCam->viewport().height,
                 bitmapFormat);
      glReadPixels(0,
                   0,
                   bmp.width,
                   bmp.height,
                   format,
                   GL_UNSIGNED_BYTE,                        
                   bmp.data); GLDEBUG_THROW;
      bmp.flip();
      bmp.write(fullPathName);
    } 

    void Context::bind(Buffer* buffer)
    {
      if(buffer != currentBuffer)
      {
        glBindBuffer(buffer->target, buffer->buffer);GLDEBUG;
        currentBuffer = buffer;
      }
    }

    Context* Context::getCurrent()
    {
      return glContext2lostGlContext[getCurrentOsSpecific()];
    }
  }
}
