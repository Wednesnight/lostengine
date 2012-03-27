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

#include "lost/gl/FrameBuffer.h"
#include "lost/math/Vec2.h"
#include <sstream>
#include <stdexcept>
#include "lost/gl/Context.h"
#include "lost/gl/gl.h"
#include "lost/lgl/lgl.h"
#include "lost/gl/Utils.h"
#include "lost/math/lmath.h"
#include "lost/math/io.h"

using namespace lost::math;

namespace lost
{
  namespace gl
  {

/**
 *  FrameBuffer::Attachment
 *
 *  holds a framebuffer attachment, that is: a texture or renderbuffer
 */

    FrameBuffer::AttachmentPtr FrameBuffer::Attachment::create(const math::Vec2& size, GLenum bitFormat, UsageType usageType)
    {
      return FrameBuffer::AttachmentPtr(new Attachment(size, bitFormat, usageType));
    }

    FrameBuffer::AttachmentPtr FrameBuffer::Attachment::create(const TexturePtr& texture)
    {
      return FrameBuffer::AttachmentPtr(new Attachment(texture));
    }

    FrameBuffer::AttachmentPtr FrameBuffer::Attachment::create(const RenderBufferPtr& renderBuffer)
    {
      return FrameBuffer::AttachmentPtr(new Attachment(renderBuffer));
    }

    FrameBuffer::Attachment::Attachment(const math::Vec2& size, GLenum bitFormat, UsageType usageType)
    {
      this->bitFormat = bitFormat;
      this->size = size;
      this->usageType = usageType;

      switch (usageType)
      {
        case UT_texture:
          createTexture();
          break;

        case UT_renderBuffer:
          createRenderBuffer();
          break;

        default:
          std::ostringstream os;
          os << "invalid usage type: " << usageType;
          throw std::runtime_error(os.str());
      }
    }

    void FrameBuffer::Attachment::createTexture()
    {
      texture.reset(new Texture());
      updateTexture();
    }
    
    void FrameBuffer::Attachment::updateTexture()
    {
      texture->bind();
      Texture::Params textureParams;
      textureParams.internalFormat = bitFormat;
      switch (bitFormat)
      {
        case LGL_DEPTH_COMPONENT16:
        case LGL_DEPTH_COMPONENT24:
#if !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE && !defined ANDROID
        case LGL_DEPTH_COMPONENT32:
#endif        
          textureParams.format = LGL_DEPTH_COMPONENT; // FIXME: this is probably the wrong constant, jst a try for ES
          textureParams.type = GL_FLOAT;
          break;
      }
      texture->init(size, textureParams);
      texture->filter(GL_NEAREST);
    }
    
    void FrameBuffer::Attachment::createRenderBuffer()
    {
      renderBuffer.reset(new RenderBuffer());
      updateRenderBuffer();
    }

    void FrameBuffer::Attachment::updateRenderBuffer()
    {
      renderBuffer->enable();
      renderBuffer->storage(bitFormat, (GLsizei)size.width, (GLsizei)size.height);
      renderBuffer->disable();
    }

    void FrameBuffer::Attachment::update()
    {
      switch (usageType)
      {
        case Attachment::UT_texture:
          updateTexture();
          break;
          
        case Attachment::UT_renderBuffer:
          updateRenderBuffer();
          break;
          
        default:
          std::ostringstream os;
          os << "invalid usage type: " << usageType;
          throw std::runtime_error(os.str());
      }
    }

    FrameBuffer::Attachment::Attachment(const TexturePtr& texture)
    {
      bitFormat = texture->internalFormat;
      size = Vec2((float)texture->width, (float)texture->height);
      usageType = UT_texture;

      this->texture = texture;
      this->texture->bind();
      this->texture->filter(GL_NEAREST);
    }

    FrameBuffer::Attachment::Attachment(const RenderBufferPtr& renderBuffer)
    {
      bitFormat = renderBuffer->internalFormat();
      size = Vec2((float)renderBuffer->width(), (float)renderBuffer->height());
      usageType = UT_renderBuffer;
      
      this->renderBuffer = renderBuffer;
    }

    FrameBuffer::Attachment::~Attachment()
    {
      if (texture) texture.reset();
      if (renderBuffer) renderBuffer.reset();
    }

    void FrameBuffer::Attachment::resize(const Vec2& size)
    {
      this->size = size;
      update();
    }

    void FrameBuffer::Attachment::resetBitFormat(GLenum bitFormat)
    {
      this->bitFormat = bitFormat;
      update();
    }

    void FrameBuffer::Attachment::attach(GLenum target)
    {
      switch (usageType)
      {
        case Attachment::UT_texture:
          texture->bind();
          lglFramebufferTexture2D(LGL_FRAMEBUFFER, target, GL_TEXTURE_2D, texture->texture, 0); GLDEBUG_THROW;
          break;
          
        case Attachment::UT_renderBuffer:
          renderBuffer->enable();
          lglFramebufferRenderbuffer(LGL_FRAMEBUFFER, target, LGL_RENDERBUFFER, renderBuffer->buffer); GLDEBUG_THROW;
          renderBuffer->disable();
          break;
          
        default:
          std::ostringstream os;
          os << "invalid usage type: " << usageType;
          throw std::runtime_error(os.str());
      }
    }

    void FrameBuffer::Attachment::bind()
    {
      switch (usageType)
      {
        case Attachment::UT_texture:
          texture->bind();
          break;
          
        case Attachment::UT_renderBuffer:
          renderBuffer->enable();
          break;
          
        default:
          std::ostringstream os;
          os << "invalid usage type: " << usageType;
          throw std::runtime_error(os.str());
      }
    }
    
    
/**
 *  FrameBuffer
 *
 *  the actual framebuffer implementation
 */

    FrameBufferPtr FrameBuffer::create(const math::Vec2& size, GLenum colorBits, GLenum depthBits, GLenum stencilBits)
    {
      return FrameBufferPtr(new FrameBuffer(size, colorBits, depthBits, stencilBits));
    }

    FrameBuffer::FrameBuffer(const math::Vec2& size, GLenum colorBits, GLenum depthBits, GLenum stencilBits)
    {
      Vec2 correctedSize((float)nextPowerOf2((unsigned long)size.width), (float)nextPowerOf2((unsigned long)size.height));
//      correctedSize.x = fmax(correctedSize.x, correctedSize.y);
//      correctedSize.y = fmax(correctedSize.x, correctedSize.y);
      this->size = correctedSize;

      lglGenFramebuffers(1, &buffer); GLDEBUG_THROW;

      bindFramebuffer();

      bool doCheck = false;
      if (colorBits != -1)
      {
        doCheck = true;
        AttachmentPtr attachment = Attachment::create(size, colorBits, Attachment::UT_texture);
        attachColorBuffer(0, attachment);
      }
      if (depthBits != -1)
      {
        doCheck = true;
        AttachmentPtr attachment = Attachment::create(size, depthBits, Attachment::UT_texture);
        attachDepthBuffer(attachment);
      }
      if (stencilBits != -1)
      {
        doCheck = true;
        AttachmentPtr attachment = Attachment::create(size, stencilBits, Attachment::UT_renderBuffer);
        attachStencilBuffer(attachment);
      }
      if (doCheck) check();

      // reset
      bindDefaultFramebuffer();
    }
    
    FrameBuffer::~FrameBuffer()
    {
      lglDeleteFramebuffers(1, &buffer); GLDEBUG;
    }

    void FrameBuffer::attachColorBuffer(uint8_t index, const TexturePtr& buffer)
    {
      attachColorBuffer(index, Attachment::create(buffer));
    }
    
    void FrameBuffer::attachColorBuffer(uint8_t index, const AttachmentPtr& buffer)
    {
      colorBuffers[index] = buffer;
      buffer->attach(LGL_COLOR_ATTACHMENT0 + index);

      // try other bit formats if attaching fails
      while (!isComplete())
      {
        switch (buffer->bitFormat)
        {
          case GL_RGB:
            throw std::runtime_error("FrameBuffer: couldn't determine compatible color buffer bit format");
          case GL_RGBA:
            buffer->resetBitFormat(GL_RGB);
            break;
#if !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE && !defined ANDROID
          case GL_RGBA8:
            buffer->resetBitFormat(GL_RGBA);
#endif
            break;
          default:
            std::ostringstream os;
            os << "invalid color buffer bit format: " << buffer->bitFormat;
            throw std::runtime_error(os.str());
        }
        buffer->attach(LGL_COLOR_ATTACHMENT0 + index);
      }
      check();
    }
    
    void FrameBuffer::attachDepthBuffer(const TexturePtr& buffer)
    {
      attachDepthBuffer(Attachment::create(buffer));
    }
    
    void FrameBuffer::attachDepthBuffer(const AttachmentPtr& buffer)
    {
      depthBuffer = buffer;
      depthBuffer->attach(LGL_DEPTH_ATTACHMENT);

      // try other bit formats if attaching fails
      while (!isComplete())
      {
        switch (depthBuffer->bitFormat)
        {
          case LGL_DEPTH_COMPONENT16:
            throw std::runtime_error("FrameBuffer: couldn't determine compatible depth buffer bit format");
          case LGL_DEPTH_COMPONENT24:
            depthBuffer->resetBitFormat(LGL_DEPTH_COMPONENT16);
            break;
#if !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE && !defined ANDROID
          case LGL_DEPTH_COMPONENT32:
            depthBuffer->resetBitFormat(LGL_DEPTH_COMPONENT24);
            break;
#endif            
          default:
            std::ostringstream os;
            os << "invalid depth buffer bit format: " << depthBuffer->bitFormat;
            throw std::runtime_error(os.str());
        }
        depthBuffer->attach(LGL_DEPTH_ATTACHMENT);
      }
      check();
    }
    
    void FrameBuffer::attachStencilBuffer(const TexturePtr& buffer)
    {
      attachStencilBuffer(Attachment::create(buffer));
    }
    
    void FrameBuffer::attachStencilBuffer(const AttachmentPtr& buffer)
    {
      stencilBuffer = buffer;
      buffer->attach(LGL_STENCIL_ATTACHMENT);
      check();
    }
    
    GLenum FrameBuffer::status()
    {
      return lglCheckFramebufferStatus(LGL_FRAMEBUFFER); GLDEBUG_THROW;
    }

    bool FrameBuffer::isComplete()
    {
      return (LGL_FRAMEBUFFER_COMPLETE == status());
    }

    void FrameBuffer::check()
    {
      std::ostringstream os;
      switch (status())
      {
        case LGL_FRAMEBUFFER_COMPLETE:
          // ok
          break;
        case LGL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
          throw std::runtime_error("FrameBuffer: invalid attachment");
          break;
        case LGL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
          throw std::runtime_error("FrameBuffer: missing attachment");
          break;
        case LGL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
          throw std::runtime_error("FrameBuffer: invalid dimensions");
          break;
#if !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE && !defined ANDROID
        case LGL_FRAMEBUFFER_INCOMPLETE_FORMATS:
          throw std::runtime_error("FrameBuffer: invalid format");
          break;

        case LGL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
          throw std::runtime_error("FrameBuffer: invalid draw buffer");
          break;
        case LGL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
          throw std::runtime_error("FrameBuffer: invalid read buffer");
          break;
#endif          
        case LGL_FRAMEBUFFER_UNSUPPORTED:
          /*
           * see: http://www.cs.kent.edu/~zhao/gpu/lectures/OpenGL_FrameBuffer_Object.pdf - page 23
           * if result is “FRAMEBUFFER_UNSUPPORTED”, application should try different
           * format combinations until one succeeds
           */
          os << "FrameBuffer: unsupported operation (size = " << size << "";
          if (depthBuffer)
          {
            os << ", depthBits = ";
            switch (depthBuffer->bitFormat)
            {
              case LGL_DEPTH_COMPONENT16: os << "16"; break;
              case LGL_DEPTH_COMPONENT24: os << "24"; break;
#if !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE && !defined ANDROID
              case LGL_DEPTH_COMPONENT32: os << "32"; break;
#endif
            }
          }
          os << ")";
          throw std::runtime_error(os.str());
          break;
        default:
          throw std::runtime_error("FrameBuffer: unknown error");
      }
    }

    void FrameBuffer::bindFramebuffer()
    {
      Context::getCurrent()->bindFramebuffer(buffer);
    }

    void FrameBuffer::bindDefaultFramebuffer()
    {
      Context::getCurrent()->bindDefaultFramebuffer();
    }

    void FrameBuffer::resize(const Vec2& size)
    {
      Vec2 correctedSize((float)nextPowerOf2((unsigned long)size.width), (float)nextPowerOf2((unsigned long)size.height));
      correctedSize.x = fmaxf(correctedSize.x, correctedSize.y);
      correctedSize.y = fmaxf(correctedSize.x, correctedSize.y);
      this->size = correctedSize;
      for (map<uint8_t, AttachmentPtr>::iterator idx = colorBuffers.begin(); idx != colorBuffers.end(); ++idx)
      {
        idx->second->resize(size);
        check();
      }
      if (depthBuffer) depthBuffer->resize(size);
      check();
      if (stencilBuffer) stencilBuffer->resize(size);
      check();
    }

    void FrameBuffer::bind()
    {
      bindFramebuffer();
      check();
    }

    /*
     * Produces some debug output regarding FBO sizing and bit formats
     */
    void FrameBuffer::setup()
    {
      FrameBufferPtr fb;
      /*
       * defaults
       */
      Vec2 size(0,0);
#if !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE && !defined ANDROID
      GLenum color = GL_RGBA8;
      GLenum depth = LGL_DEPTH_COMPONENT32;
#else
      GLenum color = GL_RGBA;
      GLenum depth = LGL_DEPTH_COMPONENT24;
#endif      
      bool done = false;
      unsigned int step = 1;

      IOUT("Determine sizing");
      while (!done) {
        try {
          fb = FrameBuffer::create(size, color, depth);
          fb->check();
          IOUT("Step " << step++ << " (ok): " << fb->size.x << "x" << fb->size.y);
          done = true;
        } catch (std::exception& e) {
          size.x = nextPowerOf2(size.x);
          size.y = nextPowerOf2(size.y);
          WOUT("Step " << step++ << " (failed): " << size.x << "x" << size.y << ", " << e.what());
          size.x += 1;
          size.y += 1;
        }
      }

      IOUT("FBO setup");
      IOUT("---------");
      IOUT("  size            : " << fb->size.x << "x" << fb->size.y);
      if (fb->colorBuffers.size() > 0) IOUT("  color component : " << ((fb->colorBuffers[0]->bitFormat == GL_RGB) ? "RGB" : (fb->colorBuffers[0]->bitFormat == GL_RGBA) ? "RGBA" : "RGBA8"));
      if (fb->depthBuffer) IOUT("  depth component : " << ((fb->depthBuffer->bitFormat == LGL_DEPTH_COMPONENT16) ? "16" : (fb->depthBuffer->bitFormat == LGL_DEPTH_COMPONENT24) ? "24" : "32"));
      
      fb.reset();
    }

  }
}
