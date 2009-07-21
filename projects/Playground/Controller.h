#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "lost/application/UiTasklet.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/DropEvent.h"
#include "lost/mesh/Line.h"
#include "lost/mesh/Quad.h"
#include "lost/camera/Camera3D.h"
#include "lost/camera/Camera2D.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/rg/Node.h"
#include "lost/meter/Meter.h"
#include "lost/font/TrueTypeFont.h"
#include "lost/application/Application.h"

struct Controller : public lost::application::UiTasklet
{  
  Controller(const lost::application::ApplicationPtr& inApplication);
  bool startup();
  bool main();
  bool shutdown();

  void draw();

  void keyDownHandler(lost::application::KeyEventPtr event);
  void fileDropHandler(lost::application::DropEventPtr event);

  bool running;
  lost::application::ApplicationPtr application;
  lost::rg::NodePtr textScene;
};

#endif
