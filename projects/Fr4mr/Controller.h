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
  lost::shared_ptr<lost::application::Application> app;
  lost::shared_ptr<lost::application::Window> mainWindow;
  lost::gl::SharedState renderState;
  lost::shared_ptr<lost::common::FpsMeter> fpsMeter;


  void shaderInit();
  lost::shared_ptr<lost::gl::ShaderProgram> program;

  void drawModel(const lost::shared_ptr<lost::gl::Canvas>& canvas, double deltaSec);
  void drawModel2(const lost::shared_ptr<lost::gl::Canvas>& canvas, double deltaSec);
  void drawScene(const lost::shared_ptr<lost::gl::Canvas>& canvas, double deltaSec);
  void render(const double deltaSec);
  void mainLoop();
  void keyHandler(lost::shared_ptr<lost::application::KeyEvent> event);

  // all canvases will operate on the same context, so we don't have to constantly set the current context
  lost::shared_ptr<lost::gl::Canvas> fullscreen3dCanvas;
  lost::shared_ptr<lost::camera::Camera3D> fullscreen3dCam;  
  lost::gl::SharedState modelRenderState;  
  float angle;
  
  // font stuff
  lost::shared_ptr<lost::font::freetype::Library> lib;
  lost::shared_ptr<lost::font::TrueTypeFont> fnt;
  lost::shared_ptr<lost::font::Model> logo;
public:
  MyAppController();
  int run();
};

#endif
