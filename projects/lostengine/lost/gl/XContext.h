#ifndef LOST_GL_XCONTEXT_H
#define LOST_GL_XCONTEXT_H

// FIXME: move header includes to cpp, use forwards here
#include "lost/mesh/Mesh.h"
#include "lost/gl/Context.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/common/Color.h"
#include "lost/camera/Camera.h"
#include "lost/gl/ShaderProgram.h"

namespace lost
{
namespace gl
{

struct XContext;
typedef lost::shared_ptr<XContext> XContextPtr;

struct XContext
{  
  XContext(ContextPtr inCtx);
  virtual ~XContext();
  
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
  
  void color(const common::Color& col); // sets the current drawing color to col
  void clearColor(const common::Color& col); // sets the current clear color to col
  
  void frameBuffer(FrameBufferPtr buffer); // sets buffer as the currently active framebuffer
  void defaultFrameBuffer(); // sets the deafut framebuffer as the cutrrently active framebuffer
    
  void camera(camera::CameraPtr cam);
  void modelTransform(const math::Matrix& inTransform); // sets the GL_MODEL_VIEW matrix to inTransform if the new one is != old one

  void clear(GLbitfield flags);    
  void swapBuffers();
  void makeCurrent();
  
  void activeTexture(GLenum tex);
  void bindActiveTextures(const std::vector<TexturePtr>& textures);
  void material(mesh::MaterialPtr mat);
  
  // meshes must have index and vertexbuffer, or you will crash. Everything else is optional.
  void draw(mesh::MeshPtr mesh);
  
  void shader(ShaderProgramPtr prog); // makes prog the active shader, switching the previous active shader off. null values are ok.
// private for now, deliberately no getters, 
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
  bool texture2DEnabled;
  common::Color currentColor;
  common::Color currentClearColor;
  ContextPtr ctx;  
  camera::CameraPtr currentCam;
  GLenum currentActiveTexture;
  ShaderProgramPtr currentShader;
  math::Matrix currentModelTransform;
};
}
}

#endif
