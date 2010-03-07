#include "lost/gl/Context.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/lgl/lgl.h"
#include "lost/camera/Camera.h"
#include <algorithm>

#if !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE
#include "lost/gl/ShaderProgram.h"
#endif

#include "lost/mesh/Mesh.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"

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
//      indexArrayEnabled = getParam<bool>(GL_INDEX_ARRAY);
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
      cullEnabled = getParam<bool>(GL_CULL_FACE);
      cullFaceMode = getParam<int>(GL_CULL_FACE_MODE);
      
      uint32_t maxNumVertexAttribs = getParam<int>(GL_MAX_VERTEX_ATTRIBS);
      DOUT("------------ max num vertex attributes: "<<maxNumVertexAttribs);
      // enable all, but not more than 16
      uint32_t arbitraryLimit = 16;
      uint32_t numEnabledVertexAttribs = std::min(maxNumVertexAttribs, arbitraryLimit);
      for(uint32_t i=0; i<numEnabledVertexAttribs; ++i)
      {
        glEnableVertexAttribArray(i);GLDEBUG;
      }
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

    void Context::bindFramebuffer(FrameBufferPtr& fbo)
    {
      fbo->bind();
    }

    void Context::bindFramebuffer(GLuint fbo)
    {
      lglBindFramebuffer(LGL_FRAMEBUFFER, fbo); GLDEBUG_THROW;
    }

    void Context::bindDefaultFramebuffer()
    {
      lglBindFramebuffer(LGL_FRAMEBUFFER, m_defaultFrameBuffer); GLDEBUG_THROW;
    }

    void Context::defaultFramebuffer(GLuint fbo)
    {
      m_defaultFrameBuffer = fbo;
    }
    
    GLuint Context::defaultFramebuffer()
    {
      return m_defaultFrameBuffer;
    }

    // FIXME: replace macros by dedficated functions that operate on member reference
    void Context::vertexArray(bool enable) { CLIENTSTATE(vertexArrayEnabled, enable, GL_VERTEX_ARRAY); }
    void Context::normalArray(bool enable) { CLIENTSTATE(normalArrayEnabled, enable, GL_NORMAL_ARRAY); }
    void Context::colorArray(bool enable) { CLIENTSTATE(colorArrayEnabled, enable, GL_COLOR_ARRAY); }
    void Context::texCoordArray(bool enable){ CLIENTSTATE(texCoordArrayEnabled, enable, GL_TEXTURE_COORD_ARRAY); }
//    void Context::indexArray(bool enable){ CLIENTSTATE(indexArrayEnabled, enable, GL_INDEX_ARRAY); }
    void Context::depthTest(bool enable) { SERVERSTATE(depthTestEnabled, enable, GL_DEPTH_TEST); }
    void Context::blend(bool enable) { SERVERSTATE(blendEnabled, enable, GL_BLEND);}

    void Context::cull(bool enable) { SERVERSTATE(cullEnabled, enable, GL_CULL_FACE);}
    void Context::cullFace(GLenum mode)
    {
      if(mode != cullFaceMode)
      {
        glCullFace(mode);GLDEBUG;
        cullFaceMode = mode;
      }
    }
    
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
        glViewport(viewport.x, viewport.y, viewport.width, viewport.height);GLDEBUG;
        glMatrixMode(GL_PROJECTION);GLDEBUG;
        glLoadMatrixf(cam->projectionMatrix().m);GLDEBUG;
        if (currentCam != cam) currentCam = cam;
      }
    }
    
    void Context::transform(const math::Matrix& inTransform)
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
    
#if !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE
    void Context::shader(ShaderProgramPtr prog)
    {
      if(currentShader && !prog)
        currentShader->disable();
      currentShader = prog;
      if(currentShader)
      {
        currentShader->enable();
      }
    }
#endif    
    
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
      if(mat->cull)
      {
        cull(true);
        cullFace(mat->cullMode);
      }
      else
      {
        cull(false);
      }
#if !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE
      shader(mat->shader);
#endif
    }
        
    void Context::draw(MeshPtr mesh)
    {
      HybridIndexBuffer* ib = mesh->indexBuffer.get();
      HybridVertexBuffer* vb = mesh->vertexBuffer.get();

      if(ib->dirty) {ib->upload();}
      if(vb->dirty) {vb->upload();}

      Buffer* gpuBuffer = vb->bufferForUsageType(UT_position);
      bind(gpuBuffer);
      vertexArray(true);
      
/*      const AttributePointerConfig& apc = vb->pointerConfigForUsageType(UT_position);
      glVertexPointer(apc.size,apc.type,apc.stride,apc.offset);GLDEBUG;

      if(vb->hasUsageType(UT_normal))
      {
        const AttributePointerConfig& apc = vb->pointerConfigForUsageType(UT_normal);
        Buffer* gpuBuffer = vb->bufferForUsageType(UT_normal);
        bind(gpuBuffer);
        normalArray(true);
        glNormalPointer(apc.type,apc.stride,apc.offset);GLDEBUG;        
      }
      else {
        normalArray(false);
      }

      if(vb->hasUsageType(UT_color))
      {
        Buffer* gpuBuffer = vb->bufferForUsageType(UT_color);
        bind(gpuBuffer);
        colorArray(true);
        const AttributePointerConfig& apc = vb->pointerConfigForUsageType(UT_color);
        glColorPointer(apc.size,apc.type,apc.stride,apc.offset);GLDEBUG;        
      }
      else {
        colorArray(false);
      }

      if(vb->hasUsageType(UT_texcoord0))
      {
        Buffer* gpuBuffer = vb->bufferForUsageType(UT_texcoord0);
        bind(gpuBuffer);
        texCoordArray(true);
        const AttributePointerConfig& apc = vb->pointerConfigForUsageType(UT_texcoord0);
        glTexCoordPointer(apc.size,apc.type,apc.stride,apc.offset);GLDEBUG;        
      }
      else {
        texCoordArray(false);
      }
*/      
      if(mesh->material)
      {
        material(mesh->material);      
      }
      transform(mesh->transform);
      bind(ib->gpuBuffers[0].get());
      
      // don't do anything if there's no shader
      if(currentShader)
      {
        // set automatic uniforms if the shader wants them
        if(currentShader->hasUniform("projectionMatrix")) { currentShader->set("projectionMatrix", currentCam->projectionMatrix()); }
        if(currentShader->hasUniform("modelViewMatrix"))
        {
          if(currentCam->hasModelViewMatrix)
            currentShader->set("modelViewMatrix", currentCam->mModelViewMatrix*mesh->transform);
          else
          {
            currentShader->set("modelViewMatrix", mesh->transform);
          }
        }
        if(currentShader->hasUniform("color")) { currentShader->set("color", mesh->material->color); }
        if(currentShader->hasUniform("texture0")) { currentShader->setInt("texture0", 0); }

        // map vertex attributes from buffer to shader according to the vertex buffers attribute map
        VertexAttributeMap::iterator i = vb->vertexAttributeMap.begin();
        VertexAttributeMap::iterator end = vb->vertexAttributeMap.end();
        for(i = vb->vertexAttributeMap.begin(); i!= end; ++i)
        {
          const std::string& attributeName = i->second;
          UsageType ut = i->first;
          if(currentShader->hasAttribute(attributeName))
          {
            const VertexAttribute& va = currentShader->name2vertexAttribute[attributeName];
            const AttributePointerConfig apc = vb->pointerConfigForUsageType(ut);
            glVertexAttribPointer(va.location, apc.size, apc.type, apc.normalise, apc.stride, apc.offset);GLDEBUG;
          }
        }
      }
      
      glDrawElements(mesh->drawMode, ib->hostBuffer->count, ib->type, 0);
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
