#ifndef FUNKYFRACTALS_H
#define FUNKYFRACTALS_H

#include <map>
#include <string>
#include "lost/application/Application.h"
#include "lost/application/Window.h"
#include "lost/gl/Canvas.h"
#include "lost/gl/State.h"
#include "lost/common/FpsMeter.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/camera/Camera3D.h"
#include "lost/application/KeyEvent.h"
#include "lost/lsystem/LSystem.h"
#include "lost/math/Matrix.h"

struct FunkyFractals
{
  lost::application::ApplicationPtr application;
  lost::application::WindowPtr      window;
  lost::camera::Camera3DPtr         camera;
  lost::gl::CanvasPtr               canvas;

  lost::gl::StatePtr         renderState;
  lost::common::FpsMeterPtr  fpsMeter;
  lost::gl::ShaderProgramPtr lightShader;

  lost::lsystem::LSystemPtr      lsystem;
  std::map<char, std::string>    lsystemVars;
  lost::lsystem::LSystemStatePtr lsystemState;
  
  FunkyFractals();
  void setupLightShader(const lost::gl::CanvasPtr& canvas);

  void run();
  void redraw(lost::application::ApplicationPtr application);
  void renderLSystemNode(lost::math::Matrix& matrix, lost::lsystem::LSystemResult& result);

  void keyHandler(lost::application::KeyEventPtr event);
};

#endif
