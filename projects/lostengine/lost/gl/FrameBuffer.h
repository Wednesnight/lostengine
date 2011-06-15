#ifndef LOST_GL_FRAMEBUFFER_H
#define LOST_GL_FRAMEBUFFER_H

#include "lost/gl/Texture.h"
#include "lost/gl/RenderBuffer.h"
#include "lost/gl/forward.h"
#include "lost/math/forward.h"

namespace lost
{
  namespace gl
  {

    struct FrameBuffer
    {

      struct Attachment;
      typedef lost::shared_ptr<Attachment> AttachmentPtr;

      struct Attachment
      {

        enum UsageType
        {
          UT_texture      = 0,
          UT_renderBuffer = 1
        };

      private:
        void createTexture();
        void updateTexture();
        void createRenderBuffer();
        void updateRenderBuffer();

        void update();

      public:
        math::Vec2 size;
        GLenum     bitFormat;
        UsageType  usageType;

        TexturePtr      texture;      // for UsageType UT_texture
        RenderBufferPtr renderBuffer; // for UsageType UT_renderBuffer

        static AttachmentPtr create(const math::Vec2& size, GLenum bitFormat, UsageType usageType = UT_texture);
        static AttachmentPtr create(const TexturePtr& texture);            // for UsageType UT_texture
        static AttachmentPtr create(const RenderBufferPtr& renderBuffer);  // for UsageType UT_renderBuffer

        Attachment(const math::Vec2& size, GLenum bitFormat, UsageType usageType = UT_texture);
        Attachment(const TexturePtr& texture);            // for UsageType UT_texture
        Attachment(const RenderBufferPtr& renderBuffer);  // for UsageType UT_renderBuffer

        ~Attachment();

        void attach(GLenum target);

        void resize(const math::Vec2& size);
        void resetBitFormat(GLenum bitFormat);

        void bind();
        
      };
      
    private:
      GLuint buffer;

      void bindFramebuffer();
      static void bindDefaultFramebuffer();

    public:
      math::Vec2 size;

      map<uint8_t, AttachmentPtr> colorBuffers;
      AttachmentPtr                           depthBuffer;
      AttachmentPtr                           stencilBuffer;

      static FrameBufferPtr create(const math::Vec2& size, GLenum colorBits = -1, GLenum depthBits = -1, GLenum stencilBits = -1);

      FrameBuffer(const math::Vec2& size, GLenum colorBits = -1, GLenum depthBits = -1, GLenum stencilBits = -1);
      ~FrameBuffer();

      void attachColorBuffer(uint8_t index, const TexturePtr& buffer);
      void attachColorBuffer(uint8_t index, const AttachmentPtr& buffer);
      void attachDepthBuffer(const TexturePtr& buffer);
      void attachDepthBuffer(const AttachmentPtr& buffer);
      void attachStencilBuffer(const TexturePtr& buffer);
      void attachStencilBuffer(const AttachmentPtr& buffer);
      
      GLenum status();
      bool isComplete();
      void check();
      
      void resize(const math::Vec2& size);
      
      void bind();

      static void setup();
    };

  }
}

#endif
