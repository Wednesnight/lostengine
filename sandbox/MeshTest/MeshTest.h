#ifndef MESHTEST_H
#define MESHTEST_H

#include "lost/application/UiTasklet.h"
#include "lost/application/KeyEvent.h"
#include "lost/mesh/Line.h"
#include "lost/mesh/Quad.h"
#include "lost/camera/Camera3D.h"
#include "lost/camera/Camera2D.h"
#include "lost/gl/ShaderHelper.h"

struct MeshTest : public lost::application::UiTasklet
{  
  MeshTest();
  bool startup();
  bool main();
  bool shutdown();

  void update(double dt);
  void draw();

  void keyHandler(lost::application::KeyEventPtr event);

  lost::mesh::Line2DPtr         line;
  lost::mesh::Mesh3DPtr         cube;
  lost::mesh::Quad2DPtr         quad;
  lost::mesh::Quad2DPtr         quad2;
  lost::mesh::Quad2DPtr         quad3;
  lost::camera::CameraPtr       camera2D;
  lost::camera::Camera3DPtr     camera3D;
  
  double passedSec;  
  float angle;
};

#endif