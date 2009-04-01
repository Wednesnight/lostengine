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
using namespace lost::font;
using namespace lost::resource;

Filt3rz::Filt3rz()
{
  fboSize.width = 256;
  fboSize.height = 256;
  numPanels = 5;
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
  setupEdgeShader();
  setupEmbossShader();
  setupSharpenShader();
  setupLightShader();
  renderState = State::create(ClearColor::create(Color(.3,.2,0,1)));
  passedSec = lost::platform::currentTimeSeconds();
  angle = 0;
  
  freetype::LibraryPtr ftlib(new freetype::Library);
  FilePtr file = app->loader->load("miserable.ttf");
  freetype::FacePtr fnt(new freetype::Face(ftlib, file));
  ttf.reset(new TrueTypeFont(ftlib, file));  
  ttf->atlasSize = Vec2(512,512);  
  
  uint32_t fontSize = 30;
  yinset = fontSize+3;
  labelOriginal = ttf->render("Original", fontSize);
  labelBlur = ttf->render("Blur", fontSize);
  labelEdge = ttf->render("Edge Detect", fontSize);
  labelEmboss = ttf->render("Emboss", fontSize);
  labelSharpen = ttf->render("Sharpen", fontSize);
}

Filt3rz::~Filt3rz()
{
  DOUT("shutting down");
}

void Filt3rz::setupBlurShader()
{
  blurShader = loadShader(app->loader, "blur");
  blurShader->enable();
  (*blurShader)["width"] = (float)fboSize.width;
  (*blurShader)["height"] = (float)fboSize.height;
  (*blurShader)["colorMap"] = (GLuint)0;
  blurShader->disable();
}

void Filt3rz::setupEdgeShader()
{
  edgeShader = loadShader(app->loader, "edge");
  edgeShader->enable();
  (*edgeShader)["width"] = (float)fboSize.width;
  (*edgeShader)["height"] = (float)fboSize.height;
  (*edgeShader)["colorMap"] = (GLuint)0;
  edgeShader->disable();
}

void Filt3rz::setupEmbossShader()
{
  embossShader = loadShader(app->loader, "emboss");
  embossShader->enable();
  (*embossShader)["width"] = (float)fboSize.width;
  (*embossShader)["height"] = (float)fboSize.height;
  (*embossShader)["colorMap"] = (GLuint)0;
  embossShader->disable();
}

void Filt3rz::setupSharpenShader()
{
  sharpenShader = loadShader(app->loader, "sharpen");
  sharpenShader->enable();
  (*sharpenShader)["width"] = (float)fboSize.width;
  (*sharpenShader)["height"] = (float)fboSize.height;
  (*sharpenShader)["colorMap"] = (GLuint)0;
  sharpenShader->disable();
}

void Filt3rz::setupLightShader()
{
  lightShader = loadShader(app->loader, "light");
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
  
//  fboCam.reset(new Camera2D(window->context, fboViewport));
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

void Filt3rz::renderFbo(double dt)
{
  framebuffer->enable();
  fboCanvas->camera->apply();
  fboCanvas->context->pushState(fboRenderState);
  fboCanvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  fboCanvas->setColor(whiteColor);
  glMatrixMode(GL_MODELVIEW);GLDEBUG;
  glLoadIdentity();GLDEBUG;
//  fboCanvas->drawLine(Vec2(0,0), Vec2(256,256));
//  fboCanvas->drawRectTextured(Rect(0,0,testPic->dataWidth, testPic->dataHeight), testPic, true);
  float cubeSize = 1;
  glTranslatef(0, .2,0);
  angle = fmod(dt*50+angle, 360);  
  glRotatef(angle, 0,1, 0);
  glRotatef(angle*.3, 1,0, 0);
  lightShader->enable();
  fboCanvas->drawSolidCube(cubeSize);
  lightShader->disable();
  fboCanvas->context->popState();
  framebuffer->disable();
}

void Filt3rz::drawPanel(ShaderProgramPtr shader, uint16_t panelIndex)
{
  shader->enable();
  glActiveTexture(GL_TEXTURE0);
  tex->bind();
  window->canvas->drawRectTextured(Rect(panelIndex*fboSize.width+panelIndex*10,0,fboSize.width, fboSize.height), tex, false);  
  shader->disable();
}

void Filt3rz::drawLabel(lost::font::ModelPtr label,
                       const lost::common::Color& col,
                       uint32_t panelIndex)
{
    float xOffset = panelIndex*fboSize.width + panelIndex*gapWidth + (fboSize.width - label->size.width)/2.0f;
    float yOffset = fboSize.height - yinset;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(xOffset, yOffset, 0);
    window->canvas->setColor(col);
    label->render(window->canvas);
}              

void Filt3rz::update(lost::application::ApplicationPtr app)
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
  window->canvas->setColor(whiteColor);
  glMatrixMode(GL_MODELVIEW);GLDEBUG;
  glLoadIdentity();GLDEBUG;

  // draw original buffer texture
  window->canvas->drawRectTextured(Rect(0,0,fboSize.width, fboSize.height), tex, false);
  drawPanel(blurShader, 1);
  drawPanel(edgeShader, 2);
  drawPanel(embossShader, 3);
  drawPanel(sharpenShader, 4);

  drawLabel(labelOriginal, blackColor, 0);
  drawLabel(labelBlur, blackColor, 1);
  drawLabel(labelEdge, whiteColor, 2);
  drawLabel(labelEmboss, whiteColor, 3);
  drawLabel(labelSharpen, blackColor, 4);

  window->canvas->context->popState();
  window->context->swapBuffers();
  
  passedSec = currentSec;
}

void Filt3rz::keyHandler(lost::application::KeyEventPtr event)
{
    if (event->key == K_ESCAPE) app->quit();
}

void Filt3rz::run()
{
  app->run();
}
