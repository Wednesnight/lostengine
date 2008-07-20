#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/KeySym.h"
#include "lost/application/ResizeEvent.h"
#include "lost/gl/Utils.h"
#include "lost/gl/Draw.h"
#include "lost/math/Vec2.h"
#include "lost/math/Matrix.h"
#include "lost/event/EventDispatcher.h"
#include "lost/gl/ShaderHelper.h"

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::event;
using namespace lost::math;
using namespace lost::gl;
using namespace lost::gl::utils;
using namespace lost::application;

namespace lost
{
  namespace gl
  {
    struct Camera
    {
      // window attributes
      Vec2 windowDimensions;
      // camera attributes
      Vec3  position;
      Vec3  target;
      Vec3  up;
      float fovy;
      Vec2  depth;
      
      Camera(const Vec2& inWindowDimensions, const Vec3& inPosition, const Vec3& inTarget, const Vec3& inUp, const float& inFovY, const Vec2& inDepth)
      : windowDimensions(inWindowDimensions),
        position(inPosition),
        target(inTarget),
        up(inUp),
        fovy(inFovY),
        depth(inDepth)
      {
      }
      ~Camera() {}
      
    };
    
    struct FPSCameraController
    {
      Camera& camera;
      bool    moveInitialized;
      Vec2    mousePos;
      
      FPSCameraController(Camera& inCamera, EventDispatcher& inDispatcher)
      : camera(inCamera),
        moveInitialized(false),
        mousePos(0, 0)
      {
        inDispatcher.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(boost::bind(&FPSCameraController::keyPressed, this, _1)));
        inDispatcher.addEventListener(MouseEvent::MOUSE_MOVE(), receive<MouseEvent>(boost::bind(&FPSCameraController::mouseMoved, this, _1)));
      }
      ~FPSCameraController() {}

      void keyPressed(shared_ptr<KeyEvent> event)
      {
        if (event->pressed)
        {
          Vec3 direction(camera.target - camera.position);
          normalise(direction);
          Matrix rotation;
          switch (event->key)
          {
            case K_W:
              camera.position += direction * 0.1;
              camera.target   += direction * 0.1;
              break;
            case K_S:
              camera.position -= direction * 0.1;
              camera.target   -= direction * 0.1;
              break;
            case K_A:
              rotation.initRotateY(90);
              camera.position -= (rotation * direction) * 0.1;
              camera.target   -= (rotation * direction) * 0.1;
              break;
            case K_D:
              rotation.initRotateY(90);
              camera.position += (rotation * direction) * 0.1;
              camera.target   += (rotation * direction) * 0.1;
              break;
          }
        }
      }
    
      void mouseMoved(shared_ptr<MouseEvent> event)
      {
        DOUT("mouseMoved(" << event->pos.x << ", " << event->pos.y << ")");
        if (!moveInitialized)
        {
          mousePos = event->pos;
          moveInitialized = true;
        }
        else
        {
          Vec3 direction(camera.target - camera.position);
          DOUT("direction: " << direction);

          float angleX = (event->pos.y - mousePos.y) * 0.1;
          float angleY = (event->pos.x - mousePos.x) * 0.1;
          DOUT("angleX: " << angleX << " angleY: " << angleY);
/*
          Matrix rotation;
          rotation.initRotateX(angleX);
          direction = rotation * direction;
          rotation.initRotateY(angleY);
          direction = rotation * direction;
          
          camera.target = camera.position + direction;
*/
/*
          float  angleZX = angle(Vec3(0, 0, 1), Vec3(0, direction.y, direction.z));
          if (angleZX > 90) angleZX = 180 - angleZX;
          Matrix matrixZX;
          matrixZX.initRotateX(angleZX);
          Matrix matrixZXBack(matrixZX);
          matrixZXBack.transpose();

          float  angleZY = angle(Vec3(0, 0, 1), Vec3(direction.x, 0, direction.z));
          if (angleZY > 90) angleZY = 180 - angleZY;
          Matrix matrixZY;
          matrixZY.initRotateY(angleZY);
          Matrix matrixZYBack(matrixZY);
          matrixZYBack.transpose();

          Matrix matrixRotX;
          matrixRotX.initRotateX(angleX);
          Matrix matrixRotY;
          matrixRotY.initRotateY(angleY);

          direction     = (matrixZYBack * (matrixZXBack * (matrixRotX * (matrixRotY * (matrixZX * (matrixZY * direction))))));
          camera.target = camera.position + direction;
//          camera.up     = matrixZYBack * matrixZXBack * matrixRotX * matrixRotY * matrixZX * matrixZY * camera.up;
*/
          float axisAngleX = rad2deg(atan(direction.z/direction.x));
          float axisAngleY = rad2deg(acos(direction.y/len(direction)));
          float axisAngleZ = rad2deg(atan(direction.x/direction.z));
          DOUT("axisAngleX: " << axisAngleX);
          DOUT("axisAngleY: " << axisAngleY);
          DOUT("axisAngleZ: " << axisAngleZ);

          // rotate over z-axis
          Matrix rotationY;
          DOUT("rotating over Z: " << axisAngleZ);
          rotationY.initRotateY(axisAngleZ);
          direction = rotationY * direction;

          // rotate on z-axis
          Matrix rotationX;
          DOUT("rotating on Z: " << 90-((axisAngleY < 0) ? -axisAngleY : axisAngleY));
          rotationX.initRotateX(90-((axisAngleY < 0) ? -axisAngleY : axisAngleY));
          direction = rotationX * direction;

          // apply new x-axis rotation
          Matrix newRotationX;
          newRotationX.initRotateX(angleX);
          direction = newRotationX * direction;
          
          // apply new y-axis rotation
          Matrix newRotationY;
          newRotationY.initRotateY(angleY);
          direction = newRotationY * direction;

          // recalculate up vector
          Matrix matrixUp;
          matrixUp.initRotateX(90);
          camera.up = matrixUp * direction;
          
          // undo rotation on z-axis
          rotationX.transpose();
          direction = rotationX * direction;
          camera.up = rotationX * camera.up;

          // undo rotation over z-axis
          rotationY.transpose();
          direction = rotationY * direction;
          camera.up = rotationY * camera.up;
                    
          camera.target = camera.position + direction;
          mousePos = event->pos;
        }
      }

    };
  }
}

shared_ptr<Camera>              cam;
shared_ptr<FPSCameraController> fps;
shared_ptr<ShaderProgram>       lightingShader;

void shaderInit()
{
  lightingShader = lost::gl::loadShader(appInstance->loader, "lighting");
  lightingShader->enable();
  lightingShader->validate();
  if(!lightingShader->validated())
  {
    DOUT("Problem found during validation: \n"<<lightingShader->log())
  }
  else
  {
    DOUT("Program validated OK");
  }
  (*lightingShader)["LightPosition"] = lost::math::Vec3(0,5,-5);
  (*lightingShader)["LightColor"]    = lost::common::Color(1,1,1);
  (*lightingShader)["EyePosition"]   = cam->position;
  (*lightingShader)["Specular"]      = lost::common::Color(1,1,1);
  (*lightingShader)["Ambient"]       = lost::common::Color(1,.1,.1);
  (*lightingShader)["SurfaceColor"]  = lost::common::Color(1,1,.1);
  (*lightingShader)["Kd"]            = 0.8f;
  lightingShader->disable();
}

void idle(shared_ptr<Event> event)
{
  int width  = appInstance->displayAttributes.width;
  int height = appInstance->displayAttributes.height;

  glDisable(GL_DEPTH_TEST);GLDEBUG;
  glDisable(GL_TEXTURE_2D);GLDEBUG;
  glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;

  // camera stuff
  if (!cam) 
  {
    cam.reset(new Camera(Vec2(width, height),   // window dimensions
                         Vec3(0, 0, -5),         // position
                         Vec3(0, 0, 0),         // target (projection center)
                         Vec3(0, 1, 0),         // up vector (rotation)
                         45,                    // field of view on Y-axis
                         Vec2(1, 100)));        // depth (znear, zfar)
  }
  if (!fps) 
  {
    glfwSetMousePos(width/2, height/2);
    glfwDisable(GLFW_MOUSE_CURSOR);
    fps.reset(new FPSCameraController(*cam, *appInstance));
  }
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(cam->fovy, cam->windowDimensions.x/cam->windowDimensions.y, cam->depth.x, cam->depth.y);
  gluLookAt(cam->position.x, cam->position.y, cam->position.z, cam->target.x, cam->target.y, cam->target.z, cam->up.x, cam->up.y, cam->up.z);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Draw the positive side of the lines x,y,z
  glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);                // Green for x axis
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);                // Red for y axis
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);                // Blue for z axis
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 10.0f);
  glEnd();
  
  // Dotted lines for the negative sides of x,y,z coordinates
  glEnable(GL_LINE_STIPPLE);                // Enable line stipple to use a dotted pattern for the lines
  glLineStipple(1, 0x0101);                    // Dotted stipple pattern for the lines
  glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);                    // Green for x axis
    glVertex3f(-10.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);                    // Red for y axis
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -10.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);                    // Blue for z axis
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -10.0f);
  glEnd();
  glDisable(GL_LINE_STIPPLE);                // Disable the line stipple
  
  // shader stuff
  if (!lightingShader)
  {
    shaderInit();
  }
  lightingShader->enable();
  setColor(whiteColor);
  glutSolidCube(1.0);
  lightingShader->disable();

  glfwSwapBuffers();  
}

void keyHandler(shared_ptr<KeyEvent> event)
{
  DOUT("key: " << event->key);
  if (event->pressed)
  {
    switch (event->key)
    {
      case K_ESCAPE:
        exit(0);
        break;
    }
  }
}

void resizeHandler(shared_ptr<ResizeEvent> event)
{
  glViewport (0, 0, event->width, event->height); 
}

void keyboard(unsigned char key, int x, int y)
{
  DOUT(key);
}

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    Timer redrawTimer("redraw", 1.0/60.0);
    redrawTimer.addEventListener(TimerEvent::TIMER_FIRED(), idle);
    app.addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(keyHandler));
    app.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(keyHandler));
    app.addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(resizeHandler));
    glutKeyboardFunc(keyboard);
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}
/*
#include <iostream>

#include "lost/platform/Platform.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/math/Vec3.h"
#include "lost/math/Rect.h"
#include "lost/math/Matrix.h"

using namespace std;
using namespace lost;
using namespace lost::math;
using namespace lost::gl::utils;

namespace lost
{
  namespace gl
  {
    struct Camera
    {
      Rect  windowSize;
      Vec3  origin;
      Vec3  target;
      Vec3  up;
      float fovy;
      float znear;
      float zfar;
      float rotationH;
      float rotationV;
      
      Camera(int windowWidth, int windowHeight, lost::math::Vec3 eye, lost::math::Vec3 at, lost::math::Vec3 inUp, float inFovY, float inZnear, float inZfar)
      : windowSize(0, 0, windowWidth, windowHeight),
      origin(eye),
      target(at),
      up(inUp),
      fovy(inFovY),
      znear(inZnear),
      zfar(inZfar),
      rotationH(0.0f),
      rotationV(0.0f)
      {
      }
      ~Camera() {}
      
      void apply()
      {
        target.x = origin.x + sin(rotationH);
        target.y = origin.y + sin(rotationV);
        target.z = origin.z - cos(rotationH) - cos(rotationV);
        glMatrixMode (GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(origin.x, origin.y, origin.z,
                  target.x, target.y, target.z,
                  up.x,     up.y,     up.z);
      }

      void reshape(float width, float height)
      {
        windowSize.width  = width;
        windowSize.height = height;
        glViewport (0, 0, windowSize.width, windowSize.height); 
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fovy, windowSize.width/windowSize.height, znear, zfar);
      }
      
      void rotateLeft(float angle = 0.05f)
      {
        rotationH -= angle;
      }
      
      void rotateRight(float angle = 0.05f)
      {
        rotationH += angle;
      }

      void rotateUp(float angle = 0.05f)
      {
        rotationV += angle;
      }

      void rotateDown(float angle = 0.05f)
      {
        rotationV -= angle;
      }

      void move(float direction)
      {
        origin.x += direction*(sin(rotationH))*0.1;
        origin.y += direction*(sin(rotationV))*0.1;
        origin.z += direction*(-cos(rotationH)-cos(rotationV))*0.1;
      }

      void strafe(float direction)
      {
      }
      
    };
  }
}

void init(void) 
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_FLAT);
}

float width  = 500;
float height = 500;
lost::gl::Camera camera(width, height, Vec3(0.0, 0.0, 5.0), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 1.0, 0.0), 90.0f, 1.5, 20.0);
void display(void)
{  
  glClear (GL_COLOR_BUFFER_BIT);
  glColor3f (1.0, 1.0, 1.0);

  camera.apply();

  glScalef (1.0, 2.0, 1.0);      // modeling transformation 
  glutWireCube (1.0);
  glFlush ();
}

void keyboard(unsigned char key, int x, int y)
{
  cout << key << endl;
  switch (key) {
    case 27:
      exit(0);
      break;
    case 'w':
      camera.move(1);
      break;
    case 's':
      camera.move(-1);
      break;
    case 'a':
      camera.strafe(1);
      break;
    case 'd':
      camera.strafe(-1);
      break;
    case '+':
      break;
    case '-':
      break;
  }
}

void keyboard_special(int key, int x, int y)
{
  switch (key) {
	  case GLUT_KEY_LEFT:
      camera.rotateLeft();
      break;
	  case GLUT_KEY_RIGHT:
      camera.rotateRight();
      break;
	  case GLUT_KEY_UP:
      camera.rotateUp();
      break;
	  case GLUT_KEY_DOWN:
      camera.rotateDown();
      break;
  }
}

void reshape (int w, int h)
{
  camera.reshape(w, h);
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize (width, height); 
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  init ();
  glutDisplayFunc(display); 
  glutIdleFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(keyboard_special);
  glutMainLoop();
  return 0;
}
#include "lost/common/Logger.h"
#include "lost/obj/Parser.h"
#include "lost/platform/Platform.h"
#include "lost/common/FileHelpers.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/math/Vec3.h"
#include "lost/math/Rect.h"
#include "lost/math/Matrix.h"

#include "glm.h"

using namespace std;
using namespace lost::common;
using namespace lost::obj;
using namespace lost::obj::parser;
using namespace lost::gl::utils;
using namespace lost::math;

// You need to declare your self a pointer to the model
// This is where the data will be loaded
GLMmodel* pmodel1 = NULL;



namespace lost
{
  namespace gl
  {
    struct Camera
    {
      Rect  windowSize;
      Vec3  origin;
      Vec3  target;
      Vec3  up;
      float fovy;
      float znear;
      float zfar;
      
      Camera(int windowWidth, int windowHeight, lost::math::Vec3 eye, lost::math::Vec3 at, lost::math::Vec3 inUp, float inFovY, float inZnear, float inZfar)
      : windowSize(0, 0, windowWidth, windowHeight),
        origin(eye),
        target(at),
        up(inUp),
        fovy(inFovY),
        znear(inZnear),
        zfar(inZfar)
      {
      }
      ~Camera() {}

      void apply()
      {
        set3DProjection(windowSize.width, windowSize.height, origin, target, up, fovy, znear, zfar);
      }
    };
  }
}



void init(void) 
{
  glClearColor (0.0, 0.0, 0.0, 0.0);   
  glEnable(GL_DEPTH_TEST);
  glShadeModel (GL_SMOOTH);
}




void drawmodel_box(void)
{
	// Load the model only if it hasn't been loaded before
	// If it's been loaded then pmodel1 should be a pointer to the model geometry data...otherwise it's null
  if (!pmodel1) 
	{
		// this is the call that actualy reads the OBJ and creates the model object
    pmodel1 = glmReadOBJ("gourd.obj");	
    if (!pmodel1) exit(0);
		// This will rescale the object to fit into the unity matrix
		// Depending on your project you might want to keep the original size and positions you had in 3DS Max or GMAX so you may have to comment this.
    glmUnitize(pmodel1);
		// These 2 functions calculate triangle and vertex normals from the geometry data.
		// To be honest I had some problem with very complex models that didn't look to good because of how vertex normals were calculated
		// So if you can export these directly from you modeling tool do it and comment these line
		// 3DS Max can calculate these for you and GLM is perfectly capable of loading them
    glmFacetNormals(pmodel1);        
		glmVertexNormals(pmodel1, 90.0);
  }
  // This is the call that will actualy draw the model
	// Don't forget to tell it if you want textures or not :))
  glmDraw(pmodel1, GLM_SMOOTH| GLM_TEXTURE);
	
}

boost::shared_ptr<lost::gl::Camera> camera;
void display(void)
{
	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
	
//  camera->apply();

  glTranslatef(0,0,-5);	

	glPushMatrix();	
  drawmodel_box();
	glPopMatrix();

	glutSwapBuffers();
}

void reshape (int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
  glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
  Matrix matrix;
  matrix.zero();
  switch (key) {
    case 27:
      exit(0);
      break;
	  case 'x':
      matrix.initRotateX(1.0f);
      camera->origin = matrix*camera->origin;
      break;
	  case 'X':
      matrix.initRotateX(-1.0f);
      camera->origin = matrix*camera->origin;
      break;
	  case 'y':
      camera->origin.y -= 10.0f;
      break;
	  case 'Y':
      camera->origin.y += 10.0f;
      break;
	  case 'z':
      camera->origin.z -= 10.0f;
      break;
	  case 'Z':
      camera->origin.z += 10.0f;
      break;
  }
}

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    string buffer = loadFile(string("cube.obj"));
    Scene scene;
    parse(buffer, scene);
    
    for (vector<Face>::iterator faceIndex = scene.faces.begin(); faceIndex != scene.faces.end(); ++faceIndex)
    {
      for (vector<Vertex>::iterator vertexIndex = (*faceIndex).vertices.begin(); vertexIndex != (*faceIndex).vertices.end(); ++vertexIndex)
      {
        cout << *vertexIndex << endl;
      }
    }
    
    camera.reset(new lost::gl::Camera(500, 500, Vec3(0,0,1), Vec3(0,0,0), Vec3(0,1,0), 90, 1, 100));
    
    glutInit(&argn, args);
    glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (500, 500); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow (args[0]);
    init ();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
    return 1;
  }

  return 0;
}
*/
