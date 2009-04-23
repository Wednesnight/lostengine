#include "Filt3rz.h"

#include "lost/application/Applicationevent.h"
#include "lost/event/Receive.h"
#include "lost/gl/StateParam.h"
#include "lost/application/Window.h"
#include "lost/mesh/Loader.h"

using namespace std;
using namespace boost;
using namespace lost;
using namespace luabind;
using namespace lost::common;
using namespace lost::camera;
using namespace lost::application;
using namespace lost::event;
using namespace lost::math;
using namespace lost::gl;
using namespace lost::font;
using namespace lost::resource;
using namespace lost::mesh;

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
  animated = true;
}

bool Filt3rz::startup()
{
  /**
   * initialize thread based members
   */
  DOUT("startup");
  window->context->makeCurrent(); // FIXME: MUST be called before XContext construction or we will crash because NO context is current 
  ctx.reset(new XContext(window->context)); 
  ctx->makeCurrent();

  fboViewport.x = 0;
  fboViewport.y = 0;
  fboViewport.width = fboSize.width;
  fboViewport.height = fboSize.height;
  
  framebuffer = FrameBuffer::createFrameBuffer(window->context, fboSize, GL_RGBA, 24);
  tex = framebuffer->colorTextures[0];

  cam3D.reset(new Camera3D(window->canvas->context, Rect(0,0,fboSize.width, fboSize.height)));
  cam3D->fovY(45.0f);
  cam3D->depth(Vec2(1.0f, 1000.0f));
  cam3D->position(Vec3(1,2,2));
  cam3D->target(Vec3(0,0,0));
  cam3D->stickToTarget(true);  

  cam2D = window->canvas->camera;

  lua->globals["fboSize"] = fboSize;
  lua->globals["cubeCam"] = cam3D;
  call_function<void>(lua->globals["init"], loader);

  blurShader = lua->globals["blurShader"];
  lightShader = lua->globals["lightShader"];
  edgeShader = lua->globals["edgeShader"];
  embossShader = lua->globals["embossShader"];
  sharpenShader = lua->globals["sharpenShader"];
  radialShader = lua->globals["radialShader"];
  ssaoShader = lua->globals["ssaoShader"];
  sepiaShader = lua->globals["sepiaShader"];
  heatsigShader = lua->globals["heatsigShader"];

//  setupLabels();
  mesh = lua->globals["mesh"];

  normalPanel = Quad2D::create(framebuffer->colorTextures[0], false);
  normalPanel->modelTransform = MatrixTranslation(Vec3(0,fboSize.height,0));  

  blurPanel = Quad2D::create(framebuffer->colorTextures[0], false);
  blurPanel->modelTransform = MatrixTranslation(Vec3(fboSize.width,fboSize.height,0));  
  blurPanel->material->shader = blurShader;

  edgePanel = Quad2D::create(framebuffer->colorTextures[0], false);
  edgePanel->modelTransform = MatrixTranslation(Vec3(2*fboSize.width,fboSize.height,0));  
  edgePanel->material->shader = edgeShader;

  embossPanel = Quad2D::create(framebuffer->colorTextures[0], false);
  embossPanel->modelTransform = MatrixTranslation(Vec3(3*fboSize.width,fboSize.height,0));  
  embossPanel->material->shader = embossShader;

  sharpenPanel = Quad2D::create(framebuffer->colorTextures[0], false);
  sharpenPanel->modelTransform = MatrixTranslation(Vec3(4*fboSize.width,fboSize.height,0));  
  sharpenPanel->material->shader = sharpenShader;
  
  radialPanel = Quad2D::create(framebuffer->colorTextures[0], false);
  radialPanel->modelTransform = MatrixTranslation(Vec3(0,0,0));  
  radialPanel->material->shader = radialShader;
  
  ssaoPanel = Quad2D::create(framebuffer->colorTextures[0], false);
  ssaoPanel->modelTransform = MatrixTranslation(Vec3(fboSize.width,0,0));  
  ssaoPanel->material->shader = ssaoShader;
  
  sepiaPanel = Quad2D::create(framebuffer->colorTextures[0], false);
  sepiaPanel->modelTransform = MatrixTranslation(Vec3(2*fboSize.width,0,0));  
  sepiaPanel->material->shader = sepiaShader;
  
  heatsigPanel = Quad2D::create(framebuffer->colorTextures[0], false);
  heatsigPanel->modelTransform = MatrixTranslation(Vec3(3*fboSize.width,0,0));  
  heatsigPanel->material->shader = heatsigShader;
  
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
  labelSSAO.reset();
  labelSepia.reset();
  labelHeatSig.reset();
  
  mesh.reset();
  framebuffer.reset();
  tex.reset();
  lightShader.reset();
  blurShader.reset();
  edgeShader.reset();
  embossShader.reset();
  sharpenShader.reset();
  radialShader.reset();
  ssaoShader.reset();
  sepiaShader.reset();
  heatsigShader.reset();
  cam3D.reset();

  normalPanel.reset();
  blurPanel.reset();
  edgePanel.reset();
  embossPanel.reset();
  sharpenPanel.reset();
  radialPanel.reset();
  ssaoPanel.reset();
  sepiaPanel.reset();
  heatsigPanel.reset();

  window.reset();

  return true;
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
  labelSSAO = ttf->render("SSAO", fontSize);
  labelSepia = ttf->render("Sepia", fontSize);
  labelHeatSig = ttf->render("Heat Signature", fontSize);
}

void Filt3rz::update(double dt)
{
  if (animated)
  {
    angle = fmod(dt*50+angle, 360);
    mesh->modelTransform = MatrixRotX(angle) * MatrixRotY(angle);
  }
}

void Filt3rz::drawPanel(ShaderProgramPtr shader, uint16_t panelIndex, uint16_t rowIndex)
{
  shader->enable();
  glActiveTexture(GL_TEXTURE0);
  tex->bind();
  /**
   * depth tex required by SSAO shader
   */
  glActiveTexture(GL_TEXTURE1);
  framebuffer->depthTexture->bind();
  window->canvas->drawRectTextured(Rect(panelIndex*fboSize.width, rowIndex*fboSize.height, fboSize.width, fboSize.height), tex, false);
  shader->disable();
  // FIXME: seems like we need to reset active texture
  glActiveTexture(GL_TEXTURE0);
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

void Filt3rz::draw()
{
  // draw 3D cube into framebuffer, lightShader on white
  ctx->frameBuffer(framebuffer);
  ctx->camera(cam3D);
  ctx->clearColor(whiteColor);
  ctx->depthTest(true);
  ctx->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ctx->draw(mesh);
  ctx->defaultFrameBuffer();
    
  ctx->camera(cam2D);
  ctx->depthTest(false);
  ctx->clearColor(grayColor);
  ctx->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ctx->draw(normalPanel);
  ctx->draw(blurPanel);
  ctx->draw(edgePanel);
  ctx->draw(embossPanel);
  ctx->draw(sharpenPanel);
  ctx->draw(radialPanel);
  ctx->draw(ssaoPanel);
  ctx->draw(sepiaPanel);
  ctx->draw(heatsigPanel);

  // draw original buffer texture
/*  window->canvas->setColor(whiteColor);
  window->canvas->drawRectTextured(Rect(0, fboSize.height, fboSize.width, fboSize.height), tex, false);
  drawPanel(blurShader, 1, 1);
  drawPanel(edgeShader, 2, 1);
  drawPanel(embossShader, 3, 1);
  drawPanel(sharpenShader, 4, 1);
  drawPanel(radialShader, 0, 0);
  drawPanel(ssaoShader, 1, 0);
  drawPanel(sepiaShader, 2, 0);
  drawPanel(heatsigShader, 3, 0);*/

  // draw outlines
/*  window->canvas->setColor(grayColor);
  for (unsigned int row = 0; row < numRows; row++)
  {
    for (unsigned int idx = 0; idx < numPanels; idx++)
    {
      window->canvas->drawRectOutline(Rect(idx*fboSize.width, row*fboSize.height, fboSize.width, fboSize.height));
    }
  }*/

/*  drawLabel(labelOriginal, blackColor, 0, 1);
  drawLabel(labelBlur, blackColor, 1, 1);
  drawLabel(labelEdge, whiteColor, 2, 1);
  drawLabel(labelEmboss, whiteColor, 3, 1);
  drawLabel(labelSharpen, blackColor, 4, 1);
  drawLabel(labelRadial, blackColor, 0, 0);
  drawLabel(labelSSAO, blackColor, 1, 0);
  drawLabel(labelSepia, blackColor, 2, 0);
  drawLabel(labelHeatSig, blackColor, 3, 0);*/

  ctx->swapBuffers();
}

bool Filt3rz::main()
{
  double currentSec = lost::platform::currentTimeSeconds();
  double delta = currentSec - passedSec;
  
  update(delta);
  draw();
  
  passedSec = currentSec;
  return true;
}

void Filt3rz::keyHandler(lost::application::KeyEventPtr event)
{
  if (event->key == K_ESCAPE) eventDispatcher->dispatchEvent(ApplicationEventPtr(new ApplicationEvent(ApplicationEvent::QUIT())));
  if (event->key == K_SPACE) animated = !animated;
}
