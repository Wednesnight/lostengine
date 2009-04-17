#ifndef LOST_GL_XCONTEXT_H
#define LOST_GL_XCONTEXT_H

// FIXME: move header includes to cpp, use forwards here
#include "lost/mesh/Mesh.h"
#include "lost/gl/Context.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/common/Color.h"
#include "lost/camera/Camera.h"

namespace lost
{
namespace gl
{

struct XContext;
typedef boost::shared_ptr<XContext> XContextPtr;

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
  void scissor(bool enable);
  void texture2D(bool enable);
  
  void color(const common::Color& col); // sets the current drawing color to col
  void clearColor(const common::Color& col); // sets the current clear color to col
  
  void frameBuffer(FrameBufferPtr buffer); // sets buffer as the currently active framebuffer
  void defaultFrameBuffer(); // sets the deafut framebuffer as the cutrrently active framebuffer
    
  void camera(camera::CameraPtr cam);
  void modelTransform(const math::Matrix& inTransform);

  void clear(GLbitfield flags);    
  void swapBuffers();
  void makeCurrent();
  
  // meshes must have index and vertexbuffer, or you will crash. Everything else is optional.
  void draw(mesh::MeshPtr mesh);
  
  
// private for now, deliberately no getters, 
private:  
  bool vertexArrayEnabled;
  bool normalArrayEnabled;
  bool colorArrayEnabled;
  bool texCoordArrayEnabled;
  bool indexArrayEnabled;
  bool depthTestEnabled;
  bool blendEnabled;
  bool scissorEnabled;
  bool texture2DEnabled;
  common::Color currentColor;
  common::Color currentClearColor;
  ContextPtr ctx;  
  camera::CameraPtr currentCam;
  
  math::Matrix currentModelTransform;
};
}
}

#endif