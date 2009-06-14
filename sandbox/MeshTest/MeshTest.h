#ifndef MESHTEST_H
#define MESHTEST_H

#include "lost/application/UiTasklet.h"
#include "lost/application/KeyEvent.h"
#include "lost/mesh/Line.h"
#include "lost/mesh/Quad.h"
#include "lost/camera/Camera3D.h"
#include "lost/camera/Camera2D.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/rg/Node.h"
#include "lost/meter/Meter.h"
#include "lost/font/TrueTypeFont.h"

struct MeshTest : public lost::application::UiTasklet
{  
  MeshTest();
  bool startup();
  bool main();
  bool shutdown();

  void update(double dt);
  void draw();

  void keyDownHandler(lost::application::KeyEventPtr event);

  lost::mesh::MeshPtr           cube;  
  lost::rg::NodePtr             scene;
  lost::rg::NodePtr             threedScene;
  lost::rg::NodePtr             textScene;
  lost::rg::NodePtr             activeScene;
  lost::meter::MeterPtr         meter;

  double passedSec;  
  float angle;
};

#endif