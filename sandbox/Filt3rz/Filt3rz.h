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

struct Filt3rz
{
public:
  Filt3rz();
  ~Filt3rz();

  void run();

private:
  boost::shared_ptr<lost::application::Application> app;
  boost::shared_ptr<lost::application::Window>      window;
  boost::shared_ptr<lost::gl::FrameBuffer>          framebuffer;
  lost::gl::TexturePtr                              tex;
  boost::shared_ptr<lost::gl::State>                renderState;
  boost::shared_ptr<lost::gl::State>                fboRenderState;
  boost::shared_ptr<lost::camera::Camera2D>         fboCam;
  boost::shared_ptr<lost::gl::Canvas>               fboCanvas;
  lost::gl::ShaderProgramPtr                        lightShader;
  lost::gl::ShaderProgramPtr                        blurShader;
  lost::gl::ShaderProgramPtr                        edgeShader;
  lost::gl::ShaderProgramPtr                        embossShader;
  lost::gl::ShaderProgramPtr                        sharpenShader;
  boost::shared_ptr<lost::camera::Camera3D>         cubeCam;  

  boost::shared_ptr<lost::font::TrueTypeFont>     ttf;
  boost::shared_ptr<lost::font::Model>            labelOriginal;
  boost::shared_ptr<lost::font::Model>            labelBlur;
  boost::shared_ptr<lost::font::Model>            labelEdge;
  boost::shared_ptr<lost::font::Model>            labelEmboss;
  boost::shared_ptr<lost::font::Model>            labelSharpen;
  
  void keyHandler(boost::shared_ptr<lost::application::KeyEvent> event);
  void update(boost::shared_ptr<lost::application::Application> app);

  void setupFBOs();
  void setupBlurShader();
  void setupEdgeShader();
  void setupEmbossShader();
  void setupSharpenShader();
  void setupLightShader();
  void renderFbo(double dt);

  void drawPanel(lost::gl::ShaderProgramPtr shader, uint16_t panelIndex);
  void drawLabel(boost::shared_ptr<lost::font::Model> label,
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