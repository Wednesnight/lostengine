#ifndef FILT3RZ_H
#define FILT3RZ_H
#include "lost/application/Application.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/Tasklet.h"
#include "lost/mesh/Mesh.h"
#include "lost/rg/Node.h"

struct Filt3rz : public lost::application::Tasklet
{
public:
  Filt3rz();
private:
  lost::rg::NodePtr                                 scene;
  lost::mesh::MeshPtr                               model;

  void keyHandler(lost::application::KeyEventPtr event);

  virtual bool startup();
  virtual bool update();
  virtual bool shutdown();

  void update(double dt);
  void draw();

  void setupLabels();
  
  lost::math::Vec2 fboSize; // width and height of the offscreen buffer
  uint16_t  numPanels;
  uint16_t  numRows;
  lost::math::Vec2  screenSize;
  
  double passedSec;  
  float angle;
  uint32_t fontSize;
  uint32_t yinset;
  bool animated;
};

#endif
