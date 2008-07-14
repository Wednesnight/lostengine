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

  glScalef (1.0, 2.0, 1.0);      /* modeling transformation */ 
  glutWireCube (1.0);
  glFlush ();
}

void keyboard(unsigned char key, int x, int y)
{
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
/*
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
