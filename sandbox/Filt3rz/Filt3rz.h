#ifndef FILT3RZ_H
#define FILT3RZ_H

#include <boost/shared_ptr.hpp>
#include "lost/application/Application.h"
#include "lost/application/KeyEvent.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/camera/Camera2D.h"
#include "lost/camera/Camera3D.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/font/TrueTypeFont.h"

struct Filt3rz : public lost::application::Tasklet
{
public:
  Filt3rz();
  ~Filt3rz();
private:
  lost::application::ApplicationPtr                 app;
  lost::application::WindowPtr                      window;
  lost::gl::FrameBufferPtr                          framebuffer;
  lost::gl::TexturePtr                              tex;
  lost::gl::StatePtr                                renderState;
  lost::gl::StatePtr                                fboRenderState;
  lost::gl::CanvasPtr                               fboCanvas;
  lost::gl::ShaderProgramPtr                        lightShader;
  lost::gl::ShaderProgramPtr                        blurShader;
  lost::gl::ShaderProgramPtr                        edgeShader;
  lost::gl::ShaderProgramPtr                        embossShader;
  lost::gl::ShaderProgramPtr                        sharpenShader;
  lost::camera::Camera3DPtr                         cubeCam;  

  lost::font::TrueTypeFontPtr                       ttf;
  lost::font::ModelPtr                              labelOriginal;
  lost::font::ModelPtr                              labelBlur;
  lost::font::ModelPtr                              labelEdge;
  lost::font::ModelPtr                              labelEmboss;
  lost::font::ModelPtr                              labelSharpen;
  
  void keyHandler(lost::application::KeyEventPtr event);

  virtual bool startup();
  virtual bool main();
  virtual bool shutdown();

  void setupFBOs();
  void setupBlurShader();
  void setupEdgeShader();
  void setupEmbossShader();
  void setupSharpenShader();
  void setupLightShader();
  void setupLabels();

  void renderFbo(double dt);

  void drawPanel(lost::gl::ShaderProgramPtr shader, uint16_t panelIndex);
  void drawLabel(lost::font::ModelPtr label,
                 const lost::common::Color& col,
                 uint32_t panelIndex);
  
  lost::math::Vec2 fboSize; // width and height of the offscreen buffer
  lost::math::Rect fboViewport;
  uint16_t  gapWidth;
  uint16_t  numPanels;
  lost::math::Vec2  screenSize;
  
  double passedSec;  
  float angle;
  uint32_t fontSize;
  uint32_t yinset;
};

#endif