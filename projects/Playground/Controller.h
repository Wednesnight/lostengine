#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "lost/application/Tasklet.h"
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

struct Controller : public lost::application::Tasklet
{  
  Controller();
  bool startup();
  bool update();
  bool shutdown();

  void draw();

  void keyDownHandler(lost::application::KeyEventPtr event);
  void fileDropHandler(lost::application::DropEventPtr event);

  lost::rg::NodePtr textScene;
};

#endif
