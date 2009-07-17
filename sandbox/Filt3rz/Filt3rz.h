#ifndef FILT3RZ_H
#define FILT3RZ_H

#include "lost/platform/shared_ptr.h"
#include "lost/application/Application.h"
#include "lost/application/KeyEvent.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/camera/Camera2D.h"
#include "lost/camera/Camera3D.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/font/TrueTypeFont.h"
#include "lost/application/UiTasklet.h"
#include "lost/mesh/Mesh.h"
#include "lost/mesh/Quad.h"
#include "lost/mesh/Line.h"

struct Filt3rz : public lost::application::UiTasklet
{
public:
  Filt3rz();
private:
  lost::gl::FrameBufferPtr                          framebuffer;
  lost::gl::TexturePtr                              tex;
  lost::gl::ShaderProgramPtr                        lightShader;
  lost::gl::ShaderProgramPtr                        blurShader;
  lost::gl::ShaderProgramPtr                        edgeShader;
  lost::gl::ShaderProgramPtr                        embossShader;
  lost::gl::ShaderProgramPtr                        sharpenShader;
  lost::gl::ShaderProgramPtr                        radialShader;
  lost::gl::ShaderProgramPtr                        ssaoShader;
  lost::gl::ShaderProgramPtr                        sepiaShader;
  lost::gl::ShaderProgramPtr                        heatsigShader;
  lost::camera::Camera3DPtr                         cam3D;
  lost::camera::Camera2DPtr                         cam2D;

  lost::font::TrueTypeFontPtr                       ttf;
/*  lost::font::ModelPtr                              labelOriginal;
  lost::font::ModelPtr                              labelBlur;
  lost::font::ModelPtr                              labelEdge;
  lost::font::ModelPtr                              labelEmboss;
  lost::font::ModelPtr                              labelSharpen;
  lost::font::ModelPtr                              labelRadial;
  lost::font::ModelPtr                              labelSSAO;
  lost::font::ModelPtr                              labelSepia;
  lost::font::ModelPtr                              labelHeatSig;*/

  lost::mesh::MeshPtr                             mesh;
  lost::mesh::Quad2DPtr                             normalPanel;
  lost::mesh::Quad2DPtr                             blurPanel;
  lost::mesh::Quad2DPtr                             edgePanel;
  lost::mesh::Quad2DPtr                             embossPanel;
  lost::mesh::Quad2DPtr                             sharpenPanel;
  lost::mesh::Quad2DPtr                             radialPanel;
  lost::mesh::Quad2DPtr                             ssaoPanel;
  lost::mesh::Quad2DPtr                             sepiaPanel;
  lost::mesh::Quad2DPtr                             heatsigPanel;

  void keyHandler(lost::application::KeyEventPtr event);

  virtual bool startup();
  virtual bool main();
  virtual bool shutdown();

  void update(double dt);
  void draw();

  void setupLabels();

/*  void drawPanel(lost::gl::ShaderProgramPtr shader, uint16_t panelIndex, uint16_t rowIndex);
  void drawLabel(lost::font::ModelPtr label,
                 const lost::common::Color& col,
                 uint32_t panelIndex,
                 uint32_t rowIndex);*/
  
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
