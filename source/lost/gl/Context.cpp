/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/gl/Context.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/lgl/lgl.h"
#include "lost/camera/Camera.h"
#include <algorithm>
#include "lost/gl/ShaderProgram.h"
#include "lost/mesh/Mesh.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"
#include "lost/gl/UniformBlock.h"
#include "lost/profiler/Blackbox.h"
#include "thread_specific_ptr.h"
#include "lost/common/Logger.h"
#include <assert.h>

using namespace lost::mesh;
using namespace lost::bitmap;

void contextCleanup(void* ctx)
{
  DOUT("");
}

tthread::thread_specific_ptr<lost::gl::Context> context_ptr(contextCleanup);

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

map<void*, Context*> glContext2lostGlContext;

    Context::Context()
    {
      initialize();
      glContext2lostGlContext[getCurrentOsSpecific()] = this;
      // this is only true on desktop systems. 
      // on systems like the iPhone, where you have to create the default buffer yourself, you
      // have to set the default in the context, after creation
      m_defaultFrameBuffer = 0; 

      depthTestEnabled = getParam<bool>(GL_DEPTH_TEST);  
      blendEnabled = getParam<bool>(GL_BLEND);
      currentBlendFuncSource = GL_ZERO; //getParam<int>(GL_BLEND_SRC);
      currentBlendFuncDestination = GL_ZERO; //getParam<int>(GL_BLEND_DST);
      scissorEnabled = getParam<bool>(GL_SCISSOR_TEST);
      texture2DEnabled = getParam<bool>(GL_TEXTURE_2D);
      currentActiveTexture = getParam<int>(GL_ACTIVE_TEXTURE);
      currentTransform.initIdentity();
      currentClearColor = getParam<lost::common::Color>(GL_COLOR_CLEAR_VALUE);
      currentScissorRect = getParam<lost::math::Rect>(GL_SCISSOR_BOX);
      currentBuffer = NULL;
      cullEnabled = getParam<bool>(GL_CULL_FACE);
      cullFaceMode = getParam<int>(GL_CULL_FACE_MODE);
      
      // reset active textures
      for(uint32_t i=0; i<_maxTextures; ++i)
      {
        activeTextures[i] = 0;
      }
      clearVertexAttributeEnabled();
      clearVertexAttributeRequired();
    }
    
    Context::~Context()
    {
      clearCurrent();
      finalize();
      map<void*, Context*>::iterator pos;
      pos = glContext2lostGlContext.find(Context::getCurrentOsSpecific());
      if(pos != glContext2lostGlContext.end())
      {
        glContext2lostGlContext.erase(pos);      
      }
    }

    void Context::cleanup()
    {
      currentCam.reset();
      currentShader.reset();
      currentBuffer = NULL;
    }

    void Context::bindFramebuffer(const FrameBufferPtr& fbo)
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
        glBlendFunc(src, dest);GLDEBUG;
        currentBlendFuncSource = src;
        currentBlendFuncDestination = dest;
      }
    }
    
    void Context::scissor(bool enable) {SERVERSTATE(scissorEnabled, enable, GL_SCISSOR_TEST);}
    
    void Context::scissorRect(const math::Rect& rect)
    {
      if(currentScissorRect != rect)
      {
        glScissor((GLint)rect.x, (GLint)rect.y, (GLsizei)rect.width, (GLsizei)rect.height);
        currentScissorRect = rect;
      }
    }

    void Context::pushScissorRect(const math::Rect& v)
    {
      _scissorRectStack.push_back(v);
      scissorRect(v);
      scissor(true);
    }
    
    void Context::pushClippedScissorRect(const math::Rect& v)
    {
      math::Rect r = v;
      if(_scissorRectStack.size())
      {
        r.clipTo(_scissorRectStack[_scissorRectStack.size()-1]);
      }
      pushScissorRect(r);
    }
    
    void Context::popScissorRect()
    {
      if(!_scissorRectStack.size()) return; // don't do anything if stack is empty
      
      _scissorRectStack.pop_back();
      uint32_t s = _scissorRectStack.size();
      if(s > 0)
      {
        scissorRect(_scissorRectStack[s-1]);
        scissor(true);
      }
      else
      {
        scissor(false);
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

    void Context::viewport(const math::Rect& val)
    {
      if(val != currentViewport)
      {
        glViewport((GLint)val.x, (GLint)val.y, (GLsizei)val.width, (GLsizei)val.height);GLDEBUG;
        currentViewport = val;
      }
    }

    // only update if the new cam is either a new one or the same one, but with the dirty flag set. 
    void Context::camera(const camera::CameraPtr& cam)
    {
      if (currentCam != cam || currentCam->needsUpdate)
      {
        viewport(cam->viewport());
        if (currentCam != cam) currentCam = cam;
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
    
    void Context::bindTexture(GLuint tex, bool override)
    {
      uint32_t idx = currentActiveTexture - GL_TEXTURE0; // have to subtract since GL_TEXTURE0 is some arbitrary hex value and not zero based
      assert((idx>=0) && (idx<_maxTextures));
      if(override) // set current entry to something other than incoming value so it gets set
      {
        activeTextures[idx] = (tex-1);
      }
      if(activeTextures[idx] != tex)
      {
        glBindTexture(GL_TEXTURE_2D, tex); GLDEBUG_THROW;        
        activeTextures[idx] = tex;
      }
    }
    
    void Context::bindTextures(const vector<TexturePtr>& textures)
    {
      if(textures.size() > 0)
      {
        uint32_t num = textures.size();
        for(uint32_t i=0; i<num; ++i)
        {
          activeTexture(GL_TEXTURE0+i); // the standard guarantees GL_TEXTUREi = GL_TEXTURE0+i
          Texture* texture = textures[i].get();
          bindTexture(texture->texture, texture->neverBeenBound);
          if(texture->neverBeenBound)
          {
            texture->neverBeenBound = false;
          }
        }
        activeTexture(GL_TEXTURE0); // reset 
      }
    }
    
    void Context::shader(const ShaderProgramPtr& prog)
    {
      if(currentShader && !prog) {currentShader->disable();}
//      if(currentShader != prog) 
//      {
        currentShader = prog;
        if(currentShader)
        {
          BB_SET_CLEAR("lost.gl.Context.shaderSwitch",true);
          BB_INC("lost.gl.Context.shaderSwitch");
          currentShader->enable();
        }
//      }
    }
    
    void Context::material(const MaterialPtr& mat)
    {
      if(mat->textures.size()>0)
      {
        bindTextures(mat->textures);
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
      shader(mat->shader);
    }

    void Context::applyUniforms(UniformBlock* ub)
    {
      for(UniformBlock::VariantMap::iterator i = ub->variantMap.begin(); 
          i != ub->variantMap.end();
          ++i)
      {
        Variant& v = i->second;
        string name = i->first;
        switch(v.type)
        {
          case VT_int     : currentShader->setInt(name, v.i);   break;
          case VT_float   : currentShader->setFloat(name, v.f); break;
          case VT_bool    : currentShader->setBool(name, v.b);  break;
          case VT_color   : currentShader->set(name, v.color);  break;
          case VT_vec2    : currentShader->set(name, v.vec2);   break;
          case VT_vec3    : currentShader->set(name, v.vec3);   break;
          case VT_vec4    : currentShader->set(name, v.vec4);   break;
          case VT_matrix  : currentShader->set(name, v.matrix); break;
          default         : break;
        }
      }
    }

    void Context::vertexAttributeEnable(uint32_t idx, bool enable)
    {
      _vertexAttributeRequired[idx] = enable;
      if(_vertexAttributeEnabled[idx] != enable)
      {
        _vertexAttributeEnabled[idx] = enable;
        if(enable){
          glEnableVertexAttribArray(idx);GLDEBUG;
        }
        else {
          glDisableVertexAttribArray(idx);GLDEBUG;          
        }
      }
    }
    
    void Context::clearVertexAttributeEnabled()
    {
      for(uint32_t i=0; i<_maxVertexAttributes; ++i)
      {
        _vertexAttributeEnabled[i] = false;
      }
    }
    
    void Context::clearVertexAttributeRequired()
    {
      for(uint32_t i=0; i<_maxVertexAttributes; ++i)
      {
        _vertexAttributeRequired[i] = false;
      }
    }

    void Context::disableUnrequiredVertexAttributes()
    {
      for(uint32_t i=0; i<_maxVertexAttributes; ++i)
      {
        if(!_vertexAttributeRequired[i] && _vertexAttributeEnabled[i])
        {
          vertexAttributeEnable(i, false);
        }
      }
    }
        
    void Context::draw(const MeshPtr& mesh)
    {
      BB_SET_CLEAR("lost.gl.Context.draw", true);    
      BB_SET_CLEAR("lost.gl.Context.bindBuffer", true);   
      BB_SET_CLEAR("lost.gl.Context.vertexAttributeEnable",true)
       
      BB_INC("lost.gl.Context.draw");
      clearVertexAttributeRequired();
      HybridIndexBuffer* ib = mesh->indexBuffer.get();
      HybridVertexBuffer* vb = mesh->vertexBuffer.get();

      if(ib->dirty) {ib->upload();}
      if(vb->dirty) {vb->upload();}

      Buffer* gpuBuffer = vb->bufferForUsageType(UT_position);
      bind(gpuBuffer);
      
      if(mesh->material)
      {
        material(mesh->material);      
      }
      bind(ib->gpuBuffer.get());

      // store the enabled vertex attributes
      vector<GLint> enabledVertexAttributes;
      // don't do anything if there's no shader
      if(currentShader)
      {
        // set automatic uniforms if the shader wants them
        if(currentShader->hasUniform("projectionMatrix")) { currentShader->set("projectionMatrix", currentCam->projectionMatrix() * currentCam->viewMatrix()); }
        if(currentShader->hasUniform("modelViewMatrix")) { currentShader->set("modelViewMatrix", mesh->transform); }
        if(currentShader->hasUniform("viewport")) { math::Rect v = currentCam->viewport(); currentShader->set("viewport", math::Vec2(v.width, v.height)); }
        if(currentShader->hasUniform("depth")) { currentShader->set("depth", currentCam->depth()); }
        if(currentShader->hasUniform("color")) { currentShader->set("color", mesh->material->color); }
        if(currentShader->hasUniform("texture0")) { currentShader->setInt("texture0", 0); }
        if(currentShader->hasUniform("texture1")) { currentShader->setInt("texture1", 1); }
        if(currentShader->hasUniform("texture2")) { currentShader->setInt("texture2", 2); }
        
        // set mesh specific uniforms after automatic ones
        if(mesh->material->uniforms)
        {
          applyUniforms(mesh->material->uniforms.get());
        }

        // map vertex attributes from buffer to shader according to the vertex buffers attribute map
        VertexAttributeMap::iterator i = vb->vertexAttributeMap.begin();
        VertexAttributeMap::iterator end = vb->vertexAttributeMap.end();
        for(i = vb->vertexAttributeMap.begin(); i!= end; ++i)
        {
          const string& attributeName = i->second;
          UsageType ut = i->first;
          if(currentShader->hasAttribute(attributeName))
          {
            const VertexAttribute& va = currentShader->name2vertexAttribute[attributeName];
            const AttributePointerConfig apc = vb->pointerConfigForUsageType(ut);
            vertexAttributeEnable(va.location, true);
            BB_INC("lost.gl.Context.vertexAttributeEnable")
            glVertexAttribPointer(va.location, apc.size, apc.type, apc.normalise, apc.stride, apc.offset);GLDEBUG;
            enabledVertexAttributes.push_back(va.location);
          }
        }
      }
      
      disableUnrequiredVertexAttributes();
      glDrawElements(ib->drawMode, ib->hostBuffer->count, ib->type, 0);GLDEBUG;
    }

    /** Uses glReadPixels to retrieve the current framebuffer data as rgba and saves it
     * as a tga file to the specified file path.
     *
     * @param fullPathName full path name of file to be saved. You must ensure that the location is writable.
     *        
     */
    void Context::writeScreenshot(const string& fullPathName,
                                 bool withAlphaChannel)
    {
      GLenum format = withAlphaChannel ? GL_RGBA : GL_RGB;
      Components bitmapFormat = withAlphaChannel ? bitmap::COMPONENTS_RGBA : bitmap::COMPONENTS_RGB;
      Bitmap bmp((uint32_t)currentCam->viewport().width,
                 (uint32_t)currentCam->viewport().height,
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
//      if(buffer != currentBuffer)
//      {
      BB_INC("lost.gl.Context.bindBuffer");
        glBindBuffer(buffer->target, buffer->buffer);GLDEBUG;
//        currentBuffer = buffer;
//      }
    }

    Context* Context::getCurrent()
    {
      if(!context_ptr.get())
      {
        context_ptr.reset(glContext2lostGlContext[getCurrentOsSpecific()]);
      }
      return context_ptr.get(); 
    }
  }
}
