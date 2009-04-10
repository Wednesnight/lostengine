#ifndef MESHTEST_H
#define MESHTEST_H

#include "lost/application/UiTasklet.h"
#include "lost/application/KeyEvent.h"
#include "lost/mesh/Line.h"

struct MeshTest : public lost::application::UiTasklet
{  
  MeshTest();
  bool startup();
  bool main();
  bool shutdown();

  void keyHandler(lost::application::KeyEventPtr event);

  lost::gl::StatePtr            renderstate;
  lost::gl::StatePtr            linestate;
  lost::mesh::Line2D*           line;
  lost::mesh::MeshPtr           cube;

};

#endif