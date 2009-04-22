#ifndef OBJECT0R_H
#define OBJECT0R_H

#include "lost/gl/gl.h"
#include "lost/gl/Context.h"
#include "lost/application/Application.h"
#include "lost/common/Logger.h"
#include "lost/common/FpsMeter.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/platform/Platform.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/event/Receive.h"
#include "lost/obj/Scene.h"
#include "lost/obj/Parser.h"
#include "lost/camera/Camera.h"

using namespace boost;
using namespace lost;
using namespace lost::application;
using namespace lost::camera;
using namespace lost::common;
using namespace lost::event;
using namespace lost::gl;
using namespace lost::math;
using namespace lost::obj;
using namespace lost::platform;
using namespace lost::resource;

struct Object0r
{
private:
  shared_ptr<Application> app;
  shared_ptr<Window> mainWindow;
  shared_ptr<Window> secondWindow;
  shared_ptr<Context> context;
  shared_ptr<State> state;
  shared_ptr<FpsMeter> fpsMeter;
  shared_ptr<DisplayAttributes> displayAttributes;
  double passedSec;

  shared_ptr<Mesh3D> mesh;

  shared_ptr<Camera> camera;
  bool moveInitialized;
  Vec2 mousePos;

  void drawWindow1()
  {
    mainWindow->context->makeCurrent();
    glViewport(0, 0, 800, 600);
    context->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    context->set3DProjection(camera->position(), camera->target(), camera->up(), camera->fovY(), camera->depth());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    context->drawMesh3D(mesh, GL_TRIANGLES);

    mainWindow->context->swapBuffers();
  }
  
  void drawWindow2(const double& deltaSec)
  {
    secondWindow->context->makeCurrent();
    glViewport(0, 0, 160, 100);
    context->pushState(state);
    context->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    context->set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(160,100));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    fpsMeter->render(0, 0, deltaSec);
    context->popState();
    secondWindow->context->swapBuffers();
  }
  
  void mainLoop()
  {
    double currentSec = lost::platform::currentTimeSeconds();
    drawWindow1();
    drawWindow2(currentSec - passedSec);
    app->processEvents();
    passedSec = currentSec;
  }
  
  void keyHandler(lost::shared_ptr<KeyEvent> event)
  {
    if (event->key == K_ESCAPE) app->quit();

    if (event->character == "w") camera->move(Vec3(0, 0, -0.25));
    else if (event->character == "a") camera->move(Vec3(-0.25, 0, 0));
    else if (event->character == "s") camera->move(Vec3(0, 0, 0.25));
    else if (event->character == "d") camera->move(Vec3(0.25, 0, 0));
    else if (event->character == "q") camera->move(Vec3(0, -0.25, 0));
    else if (event->character == "e") camera->move(Vec3(0, 0.25, 0));
  }
  
  void mouseHandler(lost::shared_ptr<MouseEvent> event)
  {
    if (!moveInitialized)
    {
      mousePos        = event->pos;
      moveInitialized = true;
    }
    else
    {
      // x-axis rotation
      float dx = -1.0 * (event->pos.y - mousePos.y) * 0.1;
      // y-axis rotation
      float dy = (event->pos.x - mousePos.x) * 0.1;

      camera->rotate(Vec3(dx, dy, 0.0));
      mousePos = event->pos;
    }
  }
  
public:
  Object0r()
  : passedSec(currentTimeSeconds()),
    moveInitialized(false)
  {
    app = Application::create(boost::bind(&Object0r::mainLoop, this));

    mainWindow = app->createWindow("window", WindowParams("Application", Vec2(800, 600), Vec2(100, 100)));

    secondWindow = app->createWindow("window2", WindowParams("FPSMeter", Vec2(160, 100), Vec2(100, 100)));
    displayAttributes.reset(new DisplayAttributes);
    context.reset(new Context(displayAttributes));
    state = context->copyState();
    state->clearColor = Color(0,0,0,0);
    fpsMeter.reset(new FpsMeter(context));

    app->addEventListener(MouseEvent::MOUSE_MOVE(), receive<MouseEvent>(bind(&Object0r::mouseHandler, this, _1)));
    app->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Object0r::keyHandler, this, _1)));

    camera.reset(new Camera());
    camera->position(Vec3(0,3,15));
    camera->target(Vec3(0,3,0));
    camera->stickToTarget(false);
    
    shared_ptr<File> file = app->loader->load("sponza.obj");
    Scene scene;
    parser::parse(file->str(), scene);
    mesh.reset(new Mesh3D(scene.vertices.size(), scene.faces.size()*3));
    for (unsigned int idx = 0; idx < scene.vertices.size(); idx++)
    {
      mesh->setVertex(idx, Vec3(scene.vertices[idx].x, scene.vertices[idx].y, scene.vertices[idx].z));
    }
    for (unsigned int idx = 0; idx < scene.faces.size(); idx++)
    {
      mesh->setFace(idx*3, scene.faces[idx].vertices[0].vertex_index);
      mesh->setFace(idx*3+1, scene.faces[idx].vertices[1].vertex_index);
      mesh->setFace(idx*3+2, scene.faces[idx].vertices[2].vertex_index);
    }
  }
  
  int run()
  {
    app->run();
    return 0;
  }
};

#endif
