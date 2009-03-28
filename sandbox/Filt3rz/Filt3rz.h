#ifndef FILT3RZ_H
#define FILT3RZ_H

#include <boost/shared_ptr.hpp>
#include "lost/application/Application.h"
#include "lost/application/KeyEvent.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/camera/Camera2D.h"
#include "lost/camera/Camera3D.h"
#include "lost/gl/ShaderHelper.h"

struct Filt3rz
{
public:
  Filt3rz();
  ~Filt3rz();

  void run();

private:
  boost::shared_ptr<lost::application::Application> app;
  boost::shared_ptr<lost::application::Window>      window;
  boost::shared_ptr<lost::gl::FrameBuffer>          framebuffer;
  lost::gl::TexturePtr                              tex;
  boost::shared_ptr<lost::gl::State>                renderState;
  boost::shared_ptr<lost::gl::State>                fboRenderState;
  boost::shared_ptr<lost::camera::Camera2D>         fboCam;
  boost::shared_ptr<lost::gl::Canvas>               fboCanvas;
  lost::gl::ShaderProgramPtr                        lightShader;
  lost::gl::ShaderProgramPtr                        blurShader;
  lost::gl::TexturePtr                              testPic;
  boost::shared_ptr<lost::camera::Camera3D>         cubeCam;  
  
  void keyHandler(boost::shared_ptr<lost::application::KeyEvent> event);
  void update(boost::shared_ptr<lost::application::Application> app);

  void setupFBOs();
  void setupBlurShader();
  void setupLightShader();
  void setupTestBitmap();
  void renderFbo();
  
  lost::math::Vec2 fboSize; // width and height of the offscreen buffer
  lost::math::Rect fboViewport;
  uint16_t  gapWidth;
  uint16_t  numPanels;
  lost::math::Vec2  screenSize;
};

#endif