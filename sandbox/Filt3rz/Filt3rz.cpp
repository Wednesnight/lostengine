#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include "lost/application/Applicationevent.h"
#include "Filt3rz.h"
#include "lost/event/Receive.h"
#include "lost/gl/StateParam.h"
#include "lost/application/Window.h"
#include "lost/model/Loader.h"

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::camera;
using namespace lost::application;
using namespace lost::event;
using namespace lost::math;
using namespace lost::gl;
using namespace lost::font;
using namespace lost::resource;

Filt3rz::Filt3rz()
{
  /**
   * initialize instance based members
   */
  fboSize.width = 256;
  fboSize.height = 256;
  numPanels = 5;
  numRows = 2;
  screenSize.width = fboSize.width * numPanels;
  screenSize.height = fboSize.height * numRows;
  windowParams = WindowParams("Filt3rz", Rect(50, 200, screenSize.width, screenSize.height));

  //  waitsForEvents = true;
  eventDispatcher->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Filt3rz::keyHandler, this, _1)));

  passedSec = lost::platform::currentTimeSeconds();
  angle = 0;
}

bool Filt3rz::startup()
{
  /**
   * initialize thread based members
   */
  window->context->makeCurrent();
  DOUT("startup");

  renderState = State::create(ClearColor::create(grayColor));
  
  setupFBOs();  
  setupBlurShader();
  setupEdgeShader();
  setupEmbossShader();
  setupSharpenShader();
  setupRadialShader();
  setupLightShader();
  setupLabels();

  return true;
}

bool Filt3rz::shutdown()
{
  /**
   * cleanup thread based members
   */

  DOUT("shutdown");

  ttf.reset();
  labelOriginal.reset();
  labelBlur.reset();
  labelEdge.reset();
  labelEmboss.reset();
  labelSharpen.reset();
  labelRadial.reset();
  
  mesh.reset();
  framebuffer.reset();
  tex.reset();
  renderState.reset();
  fboRenderState.reset();
  fboCanvas.reset();
  lightShader.reset();
  blurShader.reset();
  edgeShader.reset();
  embossShader.reset();
  sharpenShader.reset();
  radialShader.reset();
  cubeCam.reset();

  renderState.reset();
  window.reset();

  return true;
}

void Filt3rz::setupBlurShader()
{
  blurShader = loadShader(loader, "blur");
  blurShader->enable();
  (*blurShader)["width"] = (float)fboSize.width;
  (*blurShader)["height"] = (float)fboSize.height;
  (*blurShader)["colorMap"] = (GLuint)0;
  blurShader->disable();
}

void Filt3rz::setupEdgeShader()
{
  edgeShader = loadShader(loader, "edge");
  edgeShader->enable();
  (*edgeShader)["width"] = (float)fboSize.width;
  (*edgeShader)["height"] = (float)fboSize.height;
  (*edgeShader)["colorMap"] = (GLuint)0;
  edgeShader->disable();
}

void Filt3rz::setupEmbossShader()
{
  embossShader = loadShader(loader, "emboss");
  embossShader->enable();
  (*embossShader)["width"] = (float)fboSize.width;
  (*embossShader)["height"] = (float)fboSize.height;
  (*embossShader)["colorMap"] = (GLuint)0;
  embossShader->disable();
}

void Filt3rz::setupSharpenShader()
{
  sharpenShader = loadShader(loader, "sharpen");
  sharpenShader->enable();
  (*sharpenShader)["width"] = (float)fboSize.width;
  (*sharpenShader)["height"] = (float)fboSize.height;
  (*sharpenShader)["colorMap"] = (GLuint)0;
  sharpenShader->disable();
}

void Filt3rz::setupRadialShader()
{
  radialShader = loadShader(loader, "radial");
  radialShader->enable();
  (*radialShader)["colorMap"] = (GLuint)0;
  radialShader->disable();
}

void Filt3rz::setupLightShader()
{
  lightShader = loadShader(loader, "light");
  lightShader->enable();
  lightShader->validate();
  if(!lightShader->validated())
  {
    DOUT("Problem found during validation: \n"<<lightShader->log())
  }
  else
  {
    DOUT("Program validated OK");
  }
  (*lightShader)["LightPosition"] = Vec3(-1,.7,10);
  (*lightShader)["LightColor"]    = Color(1,1,1);
  (*lightShader)["EyePosition"]   = cubeCam->position();
  (*lightShader)["Specular"]      = Color(1,1,.1);
  (*lightShader)["Ambient"]       = Color(.1,.5,1);
  (*lightShader)["Kd"]            = 0.8f;
  (*lightShader)["Scale"]         = Vec2(0.7, 3.7);
  (*lightShader)["Threshold"]     = Vec2(.3, .2);
  (*lightShader)["SurfaceColor"]  = Color(1,.1,.1);
  lightShader->disable();
}


void Filt3rz::setupFBOs()
{

  fboViewport.x = 0;
  fboViewport.y = 0;
  fboViewport.width = fboSize.width;
  fboViewport.height = fboSize.height;
  
  framebuffer = FrameBuffer::createFrameBuffer(window->context, fboSize, GL_RGBA, 24);
  tex = framebuffer->colorTextures[0];

  cubeCam.reset(new Camera3D(window->canvas->context, Rect(0,0,fboSize.width, fboSize.height)));
  cubeCam->fovY(45.0f);
  cubeCam->depth(Vec2(1.0f, 100.0f));
  cubeCam->position(Vec3(1,2,2));
  cubeCam->target(Vec3(0,0,0));
  cubeCam->stickToTarget(true);  
  fboCanvas.reset(new Canvas(window->context, cubeCam));
  
  fboRenderState = State::create(ClearColor::create(whiteColor),
                                 DepthTest::create(true));

//  mesh = lost::model::Loader::obj(loader, "cessna_tri.obj");
//  mesh = lost::model::Loader::obj(loader, "cube_tri.obj");
  mesh = lost::model::Loader::obj(loader, "magnolia_tri.obj");
}

void Filt3rz::setupLabels()
{
  freetype::LibraryPtr ftlib(new freetype::Library);
  FilePtr file = loader->load("miserable.ttf");
  freetype::FacePtr fnt(new freetype::Face(ftlib, file));
  ttf.reset(new TrueTypeFont(ftlib, file));  
  ttf->atlasSize = Vec2(512,512);  
  
  uint32_t fontSize = 30;
  yinset = fontSize+3;
  labelOriginal = ttf->render("Original", fontSize);
  labelBlur = ttf->render("Gaussian Blur", fontSize);
  labelEdge = ttf->render("Edge Detect", fontSize);
  labelEmboss = ttf->render("Emboss", fontSize);
  labelSharpen = ttf->render("Sharpen", fontSize);
  labelRadial = ttf->render("Radial Blur", fontSize);
}

void Filt3rz::renderFbo(double dt)
{
  framebuffer->enable();
  fboCanvas->camera->apply();
  fboCanvas->context->pushState(fboRenderState);
  fboCanvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  fboCanvas->setColor(whiteColor);
  glMatrixMode(GL_MODELVIEW);GLDEBUG;
  glLoadIdentity();GLDEBUG;
  angle = fmod(dt*50+angle, 360);  
  glRotatef(angle, 0, 1, 0);
  glRotatef(angle, 1, 0, 0);
  lightShader->enable();
  glScalef(1.5, 1.5, 1.5);
  mesh->draw(fboCanvas->context);
  glScalef(1.0, 1.0, 1.0);
  lightShader->disable();
  fboCanvas->context->popState();
  framebuffer->disable();
}

void Filt3rz::drawPanel(ShaderProgramPtr shader, uint16_t panelIndex, uint16_t rowIndex)
{
  shader->enable();
  glActiveTexture(GL_TEXTURE0);
  tex->bind();
  window->canvas->drawRectTextured(Rect(panelIndex*fboSize.width, rowIndex*fboSize.height, fboSize.width, fboSize.height), tex, false);
  shader->disable();
}

void Filt3rz::drawLabel(lost::font::ModelPtr label,
                       const lost::common::Color& col,
                       uint32_t panelIndex,
                       uint32_t rowIndex)
{
    float xOffset = panelIndex * fboSize.width + (fboSize.width - label->size.width) / 2.0f;
    float yOffset = (rowIndex+1) * fboSize.height - yinset;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(xOffset, yOffset, 0);
    window->canvas->setColor(col);
    label->render(window->canvas);
}              

bool Filt3rz::main()
{
  double currentSec = lost::platform::currentTimeSeconds();
  double delta = currentSec - passedSec;
  // don't call this everywhere! call it at the beginning of the draw method.
  // makeCurrent won't perform unnecessary context switches
  window->context->makeCurrent(); 
  
  renderFbo(delta);
  window->canvas->camera->apply();
  window->canvas->context->pushState(renderState);
  window->canvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);GLDEBUG;
  glLoadIdentity();GLDEBUG;

  // draw original buffer texture
  window->canvas->setColor(whiteColor);
  window->canvas->drawRectTextured(Rect(0, fboSize.height, fboSize.width, fboSize.height), tex, false);
  drawPanel(blurShader, 1, 1);
  drawPanel(edgeShader, 2, 1);
  drawPanel(embossShader, 3, 1);
  drawPanel(sharpenShader, 4, 1);
  drawPanel(radialShader, 0, 0);

  // draw outlines
  window->canvas->setColor(grayColor);
  for (unsigned int row = 0; row < numRows; row++)
  {
    for (unsigned int idx = 0; idx < numPanels; idx++)
    {
      window->canvas->drawRectOutline(Rect(idx*fboSize.width, row*fboSize.height, fboSize.width, fboSize.height));
    }
  }

  drawLabel(labelOriginal, blackColor, 0, 1);
  drawLabel(labelBlur, blackColor, 1, 1);
  drawLabel(labelEdge, whiteColor, 2, 1);
  drawLabel(labelEmboss, whiteColor, 3, 1);
  drawLabel(labelSharpen, blackColor, 4, 1);
  drawLabel(labelRadial, blackColor, 0, 0);

  window->canvas->context->popState();
  window->context->swapBuffers();
  
  passedSec = currentSec;
  return true;
}

void Filt3rz::keyHandler(lost::application::KeyEventPtr event)
{
  if (event->key == K_ESCAPE) eventDispatcher->dispatchEvent(ApplicationEventPtr(new ApplicationEvent(ApplicationEvent::QUIT())));
}
