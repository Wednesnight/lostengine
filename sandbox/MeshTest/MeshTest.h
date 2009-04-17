#ifndef MESHTEST_H
#define MESHTEST_H

#include "lost/application/UiTasklet.h"
#include "lost/application/KeyEvent.h"
#include "lost/mesh/Line.h"
#include "lost/camera/Camera3D.h"
#include "lost/camera/Camera2D.h"
#include "lost/gl/XContext.h"

struct MeshTest : public lost::application::UiTasklet
{  
  MeshTest();
  bool startup();
  bool main();
  bool shutdown();

  void keyHandler(lost::application::KeyEventPtr event);

  void draw2D();
  void draw3D();

  lost::gl::XContextPtr ctx;

  lost::mesh::Line2DPtr           line;
  lost::mesh::MeshPtr           cube;
  lost::camera::CameraPtr       camera2D;
  lost::camera::Camera3DPtr     camera3D;

};

#endif