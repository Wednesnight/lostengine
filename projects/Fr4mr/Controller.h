#ifndef FR4MR_CONTROLLER_H
#define FR4MR_CONTROLLER_H

#include "lost/application/Application.h"
#include "lost/common/FpsMeter.h"
#include "lost/application/KeyEvent.h"
#include "lost/gl/State.h"
#include "lost/font/freetype/Library.h"
#include "lost/font/TrueTypeFont.h"
#include "lost/camera/Camera3D.h"
#include "lost/gl/ShaderHelper.h"


struct MyAppController
{
private:
  boost::shared_ptr<lost::application::Application> app;
  boost::shared_ptr<lost::application::Window> mainWindow;
  lost::gl::SharedState renderState;
  boost::shared_ptr<lost::common::FpsMeter> fpsMeter;


  void shaderInit();
  boost::shared_ptr<lost::gl::ShaderProgram> program;

  void drawModel(const boost::shared_ptr<lost::gl::Canvas>& canvas, double deltaSec);
  void drawModel2(const boost::shared_ptr<lost::gl::Canvas>& canvas, double deltaSec);
  void drawScene(const boost::shared_ptr<lost::gl::Canvas>& canvas, double deltaSec);
  void render(const double deltaSec);
  void mainLoop();
  void keyHandler(boost::shared_ptr<lost::application::KeyEvent> event);

  // all canvases will operate on the same context, so we don't have to constantly set the current context
  boost::shared_ptr<lost::gl::Canvas> fullscreen3dCanvas;
  boost::shared_ptr<lost::camera::Camera3D> fullscreen3dCam;  
  lost::gl::SharedState modelRenderState;  
  float angle;
  
  // font stuff
  boost::shared_ptr<lost::font::freetype::Library> lib;
  boost::shared_ptr<lost::font::TrueTypeFont> fnt;
  boost::shared_ptr<lost::font::Model> logo;
public:
  MyAppController();
  int run();
};

#endif