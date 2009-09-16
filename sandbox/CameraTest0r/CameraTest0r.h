#ifndef CAMERATEST0R_H
#define CAMERATEST0R_H

#include "lost/application/UiTasklet.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/mesh/Line.h"
#include "lost/mesh/Quad.h"
#include "lost/camera/Camera3D.h"
#include "lost/camera/Camera2D.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/rg/Node.h"
#include "lost/meter/Meter.h"
#include "lost/math/Rect.h"

struct CameraTest0r : public lost::application::UiTasklet
{  
  CameraTest0r();
  bool startup();
  bool main();
  
  void update(double dt);
  void draw();
  
  void keyHandler(lost::application::KeyEventPtr event);
  
  double passedSec;
  lost::rg::NodePtr scene;
};

#endif
