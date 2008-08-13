#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/TouchEvent.h"
#include <boost/bind.hpp>
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/gl/Draw.h"
#include "lost/bitmap/BitmapLoader.h"
#include "lost/platform/Platform.h"
#include "lost/application/KeySym.h"
#include "lost/common/FpsMeter.h"
#include "lost/model/parser/ParserOBJ.h"
#include "lost/model/Mesh.h"
#include "lost/model/MaterialOBJ.h"
#include "lost/model/RendererOBJ.h"
#include "lost/camera/CameraController.h"

using namespace std;
using namespace lost::gl;
using namespace lost::math;
using namespace lost::common;
using namespace lost::event;
using namespace lost::application;
using namespace lost::platform;
using namespace lost::bitmap;
using namespace lost::resource;
using namespace lost::application;
using namespace lost::model;
using namespace lost::camera;
using namespace boost;


Timer* redrawTimer;


struct Controller
{
  BitmapLoader loader;
  shared_ptr<Bitmap> bitmap;
  shared_ptr<Texture> texture;
  FpsMeter fpsMeter;
  
  shared_ptr<parser::ParserOBJ> modelParser;
  shared_ptr<Mesh>              mesh;
  shared_ptr<MaterialOBJ>       material;
  shared_ptr<RendererOBJ>       modelRenderer;  

  shared_ptr<CameraController> camera;

  Controller(shared_ptr<Loader> inLoader) : loader(inLoader) {}
  
  void redraw(shared_ptr<TimerEvent> event)
  {
    glClearColor(0,0,0,0);GLDEBUG;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;
    glEnable(GL_DEPTH_TEST);GLDEBUG;
    glDisable(GL_TEXTURE_2D);GLDEBUG;

//    lost::gl::utils::set3DProjection(appInstance->displayAttributes.width, appInstance->displayAttributes.height,
//                                     Vec3(0.0f, 3.0f, 15.0f), Vec3(0.0f, 3.0f, 0.0f), Vec3(0,1,0),
//                                     120, .1, 100);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(&camera->camera.getProjectionMatrix()[0][0]);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(&camera->camera.getViewMatrix()[0][0]);

    Vec4 vecAmbient  = luabind::object_cast<Vec4>(luabind::globals(*(appInstance->interpreter))["lightAmbient"]);
    Vec4 vecDiffuse  = luabind::object_cast<Vec4>(luabind::globals(*(appInstance->interpreter))["lightDiffuse"]);
    Vec4 vecSpecular = luabind::object_cast<Vec4>(luabind::globals(*(appInstance->interpreter))["lightSpecular"]);

    GLfloat shininess[]     = {luabind::object_cast<float>(luabind::globals(*(appInstance->interpreter))["lightShininess"])};
    GLfloat ambient[]       = {vecAmbient.x, vecAmbient.y, vecAmbient.z, vecAmbient.w};
    GLfloat diffuse[]       = {vecDiffuse.x, vecDiffuse.y, vecDiffuse.z, vecDiffuse.w};
    GLfloat specular[]      = {vecSpecular.x, vecSpecular.y, vecSpecular.z, vecSpecular.w};
    Vector3 cameraPosition  = camera->camera.getPosition();
    GLfloat position[]      = {cameraPosition.x, cameraPosition.y, cameraPosition.z, 0.0f};
    Vector3 cameraDirection = camera->camera.getViewDirection();
    GLfloat direction[]     = {cameraDirection.x, cameraDirection.y, cameraDirection.z};
    GLfloat cutoff[]        = {luabind::object_cast<float>(luabind::globals(*(appInstance->interpreter))["lightCutoff"])};
    
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
    glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_RESCALE_NORMAL);
    
    setColor(whiteColor);
    modelRenderer->render();
    
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);

//    modelRenderer->renderNormals();
//    modelRenderer->renderAABB();
    
    glScalef(10.0f, 10.0f, 10.0f);
    drawAxes(Vec3(10.0f, 10.0f, 10.0f));

    lost::gl::utils::set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));
    glMatrixMode(GL_MODELVIEW);GLDEBUG;
    glLoadIdentity();GLDEBUG;
/*
    glEnable(GL_TEXTURE_2D);GLDEBUG;
    glEnable(GL_BLEND);GLDEBUG;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    GLDEBUG;
    glColor4f(1, 1, 1,1);GLDEBUG;
//    drawLine(Vec2(0,0),Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));
    glEnableClientState(GL_VERTEX_ARRAY);GLDEBUG;
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);GLDEBUG;    
    drawLine(Vec2(0,0), Vec2(320,480));
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);GLDEBUG;
    drawRectTextured(Rect(10,10,texture->width,texture->height), *texture);
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    
    setColor(whiteColor);
    drawRectFilled(Rect(400,400,50,50));
*/    
    fpsMeter.render(2,2,event->passedSec);
    
    appInstance->swapBuffers();
  }
  
  void keyboard(shared_ptr<KeyEvent> inEvent)
  {
    switch (inEvent->key)
    {
      case K_ESCAPE :
        appInstance->quit();
        break;
      default :
        break;
    }
  }
  
  void init(shared_ptr<ApplicationEvent> event)
  {
    //setup resources
    //string filename = "gay_zombie.jpg";
    //string filename = "nomnomnom.jpg";
    //string filename = "buttonReleased.png";
    string filename = "stubs.jpg";
    bitmap = loader.load(filename);
    
    camera.reset(new CameraController(appInstance->displayAttributes, *appInstance));
    
    texture.reset(new Texture());
    texture->bind();
    texture->reset(0, bitmap->format, false, *bitmap);
    texture->wrap(GL_CLAMP_TO_EDGE);
    texture->filter(GL_LINEAR);
    
    DOUT("width: "<<texture->width<< " height: "<<texture->height);
    
    std::string modelname = luabind::object_cast<std::string>(luabind::globals(*(appInstance->interpreter))["modelFilename"]);
    float       modelSize = luabind::object_cast<float>(luabind::globals(*(appInstance->interpreter))["modelSize"]);
    modelParser.reset(new parser::ParserOBJ(appInstance->loader));
    modelParser->parseMesh(modelname, mesh, material);
    modelRenderer.reset(new RendererOBJ(mesh, material));
    modelRenderer->size = modelSize;

    redrawTimer = new Timer("redrawTimer", 1.0/30.0);
    redrawTimer->addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Controller::redraw, this, _1)));

    glViewport(0, 0, appInstance->displayAttributes.width, appInstance->displayAttributes.height);GLDEBUG;
  }
  
  void touches(shared_ptr<TouchEvent> event)
  {
    DOUT(event->type);
    DOUT("camera position   : " << camera->camera.getPosition().x << ", " << camera->camera.getPosition().y << ", " << camera->camera.getPosition().z);
    DOUT("camera orientation: " << camera->camera.getOrientation().x << ", " << camera->camera.getOrientation().y << ", " << camera->camera.getOrientation().z);
  }
};

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    Controller controller(appInstance->loader);
    
    app.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Controller::keyboard, &controller, _1)));
    app.addEventListener(ApplicationEvent::INIT(), receive<ApplicationEvent>(bind(&Controller::init, &controller, _1)));
    app.addEventListener(TouchEvent::TOUCHES_BEGAN(), receive<TouchEvent>(bind(&Controller::touches, &controller, _1)));
    app.addEventListener(TouchEvent::TOUCHES_MOVED(), receive<TouchEvent>(bind(&Controller::touches, &controller, _1)));
    app.addEventListener(TouchEvent::TOUCHES_ENDED(), receive<TouchEvent>(bind(&Controller::touches, &controller, _1)));
    app.addEventListener(TouchEvent::TOUCHES_CANCELLED(), receive<TouchEvent>(bind(&Controller::touches, &controller, _1)));
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}
