#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "lost/application/Tasklet.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/DropEvent.h"
#include "lost/rg/Node.h"
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

  lost::rg::NodePtr rootNode;
};

#endif
