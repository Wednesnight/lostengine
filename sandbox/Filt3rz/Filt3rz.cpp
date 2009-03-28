#include "Filt3rz.h"
#include "lost/event/Receive.h"
#include "lost/gl/StateParam.h"

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::camera;
using namespace lost::application;
using namespace lost::event;
using namespace lost::math;
using namespace lost::gl;

Filt3rz::Filt3rz()
{
  fboSize.width = 256;
  fboSize.height = 256;
  numPanels = 4;
  gapWidth = 10;
  screenSize.width = fboSize.width*numPanels + (numPanels-1)*gapWidth;
  screenSize.height = fboSize.width;
  app = Application::create(bind(&Filt3rz::update, this, _1));
//  app->runLoopWaitsForEvents(true);
  app->addEventListener(lost::application::KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Filt3rz::keyHandler, this, _1)));
    
  window = app->createWindow("window", WindowParams("Filt3rz", Rect(50,200,screenSize.width,screenSize.height)));
  window->context->makeCurrent();
  setupFBOs();  
  setupBlurShader();
  setupTestBitmap();
  setupLightShader();
  renderState = State::create(ClearColor::create(blackColor));
  
}

Filt3rz::~Filt3rz()
{
  DOUT("shutting down");
}

void Filt3rz::setupBlurShader()
{
  blurShader = loadShader(app->loader, "convolute");
}

void Filt3rz::setupLightShader()
{
  lightShader = loadShader(app->loader, "lattice");
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

void Filt3rz::setupTestBitmap()
{
  testPic.reset(new Texture(app->loader->load("zim.jpg")));
}

void Filt3rz::setupFBOs()
{

  fboViewport.x = 0;
  fboViewport.y = 0;
  fboViewport.width = fboSize.width;
  fboViewport.height = fboSize.height;

  framebuffer.reset(new FrameBuffer());
  framebuffer->enable();
  tex.reset(new Texture(fboSize));
  framebuffer->attachColor(0, tex);
  tex->filter(GL_NEAREST);
  DOUT("FBO complete: "<<framebuffer->isComplete());

  TexturePtr depthBuffer(new Texture());
  depthBuffer->bind();
  depthBuffer->init(0,GL_DEPTH_COMPONENT24, fboSize.width, fboSize.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  framebuffer->attachDepth(depthBuffer);
  DOUT("FBO complete: "<<framebuffer->isComplete());
  if(!framebuffer->isComplete())
    throw runtime_error("couldn't create FBO");
  framebuffer->disable();
  
  fboCam.reset(new Camera2D(window->context, fboViewport));
  cubeCam.reset(new Camera3D(window->canvas->context, Rect(0,0,fboSize.width, fboSize.height)));
  cubeCam->fovY(45.0f);
  cubeCam->depth(Vec2(1.0f, 100.0f));
  cubeCam->position(Vec3(1,2,2));
  cubeCam->target(Vec3(0,0,0));
  cubeCam->stickToTarget(true);  
  fboCanvas.reset(new Canvas(window->context, cubeCam));
  
  fboRenderState = State::create(ClearColor::create(whiteColor),
                                 DepthTest::create(true));

}

void Filt3rz::renderFbo()
{
  framebuffer->enable();
  fboCanvas->camera->apply();
  fboCanvas->context->pushState(fboRenderState);
  fboCanvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  fboCanvas->setColor(whiteColor);
  glMatrixMode(GL_MODELVIEW);GLDEBUG;
  glLoadIdentity();GLDEBUG;
  glMatrixMode(GL_TEXTURE);GLDEBUG;
  glLoadIdentity();
//  fboCanvas->drawLine(Vec2(0,0), Vec2(256,256));
//  fboCanvas->drawRectTextured(Rect(0,0,testPic->dataWidth, testPic->dataHeight), testPic, true);
  float cubeSize = 1;
  glTranslatef(-1, .2,0);
  glRotatef(/*angle*/13, 0,1, 0);
  lightShader->enable();
  fboCanvas->drawSolidCube(cubeSize);
  lightShader->disable();
  fboCanvas->context->popState();
  framebuffer->disable();
}

void Filt3rz::update(boost::shared_ptr<lost::application::Application> app)
{
  // don't call this everywhere! call it at the beginning of the draw method.
  // makeCurrent won't perform unnecessary context switches
  window->context->makeCurrent(); 
  
  renderFbo();

  window->canvas->camera->apply();
  window->canvas->context->pushState(renderState);
  window->canvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  window->canvas->setColor(whiteColor);
  glMatrixMode(GL_MODELVIEW);GLDEBUG;
  glLoadIdentity();GLDEBUG;
  
  // activate and configure the blur shader here so the texture of the following rect will be postprocessed
  blurShader->enable();
  glActiveTexture(GL_TEXTURE0);
  tex->bind();
  (*blurShader)["colorMap"] = (GLuint)0;
  (*blurShader)["width"] = (float)fboSize.width;
  (*blurShader)["height"] = (float)fboSize.height;
  window->canvas->drawRectTextured(Rect(fboSize.width+10,0,fboSize.width, fboSize.height), tex, false);  
  blurShader->disable();
  
  window->canvas->drawRectTextured(Rect(0,0,fboSize.width, fboSize.height), tex, false);
  window->canvas->context->popState();
  window->context->swapBuffers();
}

void Filt3rz::keyHandler(boost::shared_ptr<lost::application::KeyEvent> event)
{
    if (event->key == K_ESCAPE) app->quit();
}

void Filt3rz::run()
{
  app->run();
}
