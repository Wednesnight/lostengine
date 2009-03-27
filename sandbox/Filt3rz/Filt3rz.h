#ifndef FILT3RZ_H
#define FILT3RZ_H

#include <boost/shared_ptr.hpp>
#include "lost/application/Application.h"
#include "lost/application/KeyEvent.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/camera/Camera2D.h"

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
  boost::shared_ptr<lost::gl::Texture>              tex;
  boost::shared_ptr<lost::gl::State>                renderState;
  boost::shared_ptr<lost::gl::State>                fboRenderState;
  boost::shared_ptr<lost::camera::Camera2D>         fboCam;
  boost::shared_ptr<lost::gl::Canvas>               fboCanvas;
  
  void keyHandler(boost::shared_ptr<lost::application::KeyEvent> event);
  void update(boost::shared_ptr<lost::application::Application> app);

  void setupFBOs();
  void renderFbo();
  
  lost::math::Vec2 fboSize; // width and height of the offscreen buffer
  lost::math::Rect fboViewport;
};

#endif