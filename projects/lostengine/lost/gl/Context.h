/**
 * Note:
 *
 *   - application::gl::Context is designed to live within an application::Window
 *   - to reduce overhead the platform specific part expects that an OpenGL context already exists and is set as the thread's current context!
 *   - the ctor gets the current context and uses that handle for it's complete lifecycle (e.g. makeCurrent, swapBuffers)
 *
 * There is only one proper way to initialize an application::gl::Context:
 *   1.) create an application::Window (it'll create and activate an OpenGL context) or create an OpenGL context and make it the thread's current!
 *   2.) create the application::gl::Context
 *
 */

#ifndef LOST_APPLICATION_GL_CONTEXT_H
#define LOST_APPLICATION_GL_CONTEXT_H

#include "lost/math/Matrix.h"
#include "lost/common/Color.h"
#include "lost/platform/shared_ptr.h"
#include <vector>
#include "lost/gl/gltypes.h"

namespace lost
{
  namespace camera { struct Camera; typedef lost::shared_ptr<Camera> CameraPtr; }
  namespace mesh
  {
    struct Mesh; typedef lost::shared_ptr<Mesh> MeshPtr;
    struct Material; typedef lost::shared_ptr<Material> MaterialPtr;
  }
  namespace gl
  {
    struct Context;
    typedef lost::shared_ptr<Context> ContextPtr;

    struct ShaderProgram;
    typedef lost::shared_ptr<ShaderProgram> ShaderProgramPtr;

    struct Texture;
    typedef lost::shared_ptr<Texture> TexturePtr;

    struct FrameBuffer;
    typedef lost::shared_ptr<FrameBuffer> FrameBufferPtr;
    
    struct Buffer;
    
    struct Context
    {
    // private for now, deliberately no getters
    private:  
      bool vertexArrayEnabled;
      bool normalArrayEnabled;
      bool colorArrayEnabled;
      bool texCoordArrayEnabled;
      bool indexArrayEnabled;
      bool depthTestEnabled;
      bool blendEnabled;
      GLenum currentBlendFuncSource;
      GLenum currentBlendFuncDestination;
      bool scissorEnabled;
      math::Rect currentScissorRect;
      bool texture2DEnabled;
      common::Color currentColor;
      common::Color currentClearColor;
      camera::CameraPtr currentCam;
      GLenum currentActiveTexture;
      ShaderProgramPtr currentShader;
      math::Matrix currentTransform;
      Buffer* currentBuffer;
      
      /**
       * forward declaration for platform specific stuff
       */
      struct ContextHiddenMembers;
      ContextHiddenMembers* hiddenMembers;

      /**
       * hidden ctor/dtor utility methods for platform specific stuff
       */
      void initialize();
      void finalize();

      GLuint m_defaultFrameBuffer;
      
    public:
      Context();
      ~Context();

      /**
       * sets this OpenGL context as the thread's current context
       */
      void makeCurrent();

      /**
       * swaps buffers :)
       */
      void swapBuffers();

      void vsync(bool enable); // true to enable vsync to prevent tearing
      void multithreaded(bool enable); // true to enable multithreaded OpenGL execution on Mac
      
      void bindFramebuffer(FrameBufferPtr& fbo);
      void bindFramebuffer(GLuint fbo);

      void bindDefaultFramebuffer();
      void defaultFramebuffer(GLuint fbo);
      GLuint defaultFramebuffer();

      void vertexArray(bool enable);
      void normalArray(bool enable);
      void colorArray(bool enable);
      void texCoordArray(bool enable);
      void indexArray(bool enable);
      void depthTest(bool enable);
      void blend(bool enable);
      void blendFunc(GLenum src, GLenum dest);
      void scissor(bool enable);
      void texture2D(bool enable);
      
      void scissorRect(const math::Rect& rect); // sets the current scissoring region to rect
      void color(const common::Color& col); // sets the current drawing color to col
      void clearColor(const common::Color& col); // sets the current clear color to col
      
      void camera(camera::CameraPtr cam);
      void transform(const math::Matrix& inTransform); // sets the GL_MODEL_VIEW matrix to inTransform if the new one is != old one
      
      void clear(GLbitfield flags);    
      
      void activeTexture(GLenum tex);
      void bindActiveTextures(const std::vector<TexturePtr>& textures);
      void material(mesh::MaterialPtr mat);
      
      // meshes must have index and vertexbuffer, or you will crash. Everything else is optional.
      void draw(mesh::MeshPtr mesh);
      void drawSeparateBuffers(mesh::MeshPtr mesh);
      void drawInterleavedBuffers(mesh::MeshPtr mesh);
      
      void shader(ShaderProgramPtr prog); // makes prog the active shader, switching the previous active shader off. null values are ok.

      // writes the current framebuffer with the current viewport configurtaion to a file as a tga, with optional alpha channel.
      void writeScreenshot(const std::string& fullPathName, bool withAlphaChannel);
      
      static void* getCurrentOsSpecific();
      static void setCurrentOsSpecififc(void* ctx);
      static Context* getCurrent();
      
      void bind(Buffer* buffer);
    };
  }
}

#endif
