#ifndef FILT3RZ_H
#define FILT3RZ_H

#include <boost/shared_ptr.hpp>
#include "lost/application/Application.h"
#include "lost/application/KeyEvent.h"
#include "lost/gl/FrameBuffer.h"


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

  bool contextOk;
  
  void keyHandler(boost::shared_ptr<lost::application::KeyEvent> event);
  void update(boost::shared_ptr<lost::application::Application> app);

  void setupFBOs();
};

#endif