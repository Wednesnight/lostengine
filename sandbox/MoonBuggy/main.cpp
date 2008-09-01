#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/gl/gl.h"
#include "chipmunk.h"
#include "moonbuggy.h"
#include "lost/common/FpsMeter.h"
#include "lost/gl/Utils.h"
#include "lost/math/Vec2.h"

#define SLEEP_TICKS 16

extern cpSpace *space;
extern cpBody *staticBody;

extern cpBody *chassis;

int ticks = 0;

lost::common::FpsMeter fpsMeter;

void drawCircle(cpFloat x, cpFloat y, cpFloat r, cpFloat a)
{
	const int segs = 15;
	const cpFloat coef = 2.0*M_PI/(cpFloat)segs;
	
	int n;
	glBegin(GL_LINE_LOOP); {
		for(n = 0; n < segs; n++){
			cpFloat rads = n*coef;
			glVertex2f(r*cos(rads + a) + x, r*sin(rads + a) + y);
		}
		glVertex2f(x,y);
	} glEnd();
}

void drawCircleShape(cpShape *shape)
{
	cpBody *body = shape->body;
	cpCircleShape *circle = (cpCircleShape *)shape;
	cpVect c = cpvadd(body->p, cpvrotate(circle->c, body->rot));
	drawCircle(c.x, c.y, circle->r, body->a);
}

void drawSegmentShape(cpShape *shape)
{
	cpBody *body = shape->body;
	cpSegmentShape *seg = (cpSegmentShape *)shape;
	cpVect a = cpvadd(body->p, cpvrotate(seg->a, body->rot));
	cpVect b = cpvadd(body->p, cpvrotate(seg->b, body->rot));
	
	glBegin(GL_LINES); {
		glVertex2f(a.x, a.y);
		glVertex2f(b.x, b.y);
	} glEnd();
}

void drawPolyShape(cpShape *shape)
{
	cpBody *body = shape->body;
	cpPolyShape *poly = (cpPolyShape *)shape;
	
	int num = poly->numVerts;
	cpVect *verts = poly->verts;
	
	int i;
	glBegin(GL_LINE_LOOP);
	for(i=0; i<num; i++){
		cpVect v = cpvadd(body->p, cpvrotate(verts[i], body->rot));
		glVertex2f(v.x, v.y);
	} glEnd();
}

void drawObject(void *ptr, void *unused)
{
	cpShape *shape = static_cast<cpShape*>(ptr);
	switch(shape->type){
		case CP_CIRCLE_SHAPE:
			drawCircleShape(shape);
			break;
		case CP_SEGMENT_SHAPE:
			drawSegmentShape(shape);
			break;
		case CP_POLY_SHAPE:
			drawPolyShape(shape);
			break;
		default:
			printf("Bad enumeration in drawObject().\n");
	}
}

void drawCollisions(void *ptr, void *data)
{
	cpArbiter *arb = static_cast<cpArbiter*>(ptr);
	int i;
	for(i=0; i<arb->numContacts; i++){
		cpVect v = arb->contacts[i].p;
		glVertex2f(v.x, v.y);
	}
}

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::event;
using namespace lost::application;

void mouseButtonDown(shared_ptr<MouseEvent> event)
{
  moonBuggy_input(0, 0, 0, 0);
  DOUT("mb stuff");
}

void mouseButtonUp(shared_ptr<MouseEvent> event)
{
  moonBuggy_input(0, 1, 0, 0);
  DOUT("mb stuff");
}

void mouseMove(shared_ptr<MouseEvent> event)
{
  //DOUT("mm stuff");
}

void keyHandler(shared_ptr<KeyEvent> event)
{
  DOUT("kb stuff");
}

void resize(shared_ptr<ResizeEvent> event)
{
  DOUT("resize w: "<<event->width << " h:" << event->height);
}

void redraw(shared_ptr<TimerEvent> event)
{
  // init
  glClearColor(1.0, 1.0, 1.0, 0.0);  
	glPointSize(3.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1000.0f, 1000.0f, -750.0, 750.0, -1.0, 1.0);
	glScalef(2.0, 2.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
  
  // display
	int i;
	
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(-chassis->p.x, -chassis->p.y, 0.0);
	
	glColor3f(0.0, 0.0, 0.0);
	cpSpaceHashEach(space->activeShapes, &drawObject, NULL);
	cpSpaceHashEach(space->staticShapes, &drawObject, NULL);
  
	cpArray *bodies = space->bodies;
	int num = bodies->num;
	
	glBegin(GL_POINTS); {
		glColor3f(0.0, 0.0, 1.0);
		for(i=0; i<num; i++){
			cpBody *body = static_cast<cpBody*>(bodies->arr[i]);
			glVertex2f(body->p.x, body->p.y);
		}
		
		glColor3f(1.0, 0.0, 0.0);
		cpArrayEach(space->arbiters, &drawCollisions, NULL);
	} glEnd();

  glEnableClientState(GL_VERTEX_ARRAY);GLDEBUG;
  glDisable(GL_DEPTH_TEST);GLDEBUG;
  glDisable(GL_TEXTURE_2D);GLDEBUG;
  
  lost::gl::utils::set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  fpsMeter.render(appInstance->displayAttributes.width - fpsMeter.width, 0, event->passedSec);

  glDisableClientState(GL_VERTEX_ARRAY);GLDEBUG;

	appInstance->swapBuffers();
	ticks++;
	
	moonBuggy_update();
}

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    cpInitChipmunk();    
    moonBuggy_init();
 
    Application app;
    app.addEventListener(MouseEvent::MOUSE_UP(), receive<MouseEvent>(mouseButtonUp));
    app.addEventListener(MouseEvent::MOUSE_DOWN(), receive<MouseEvent>(mouseButtonDown));
    app.addEventListener(MouseEvent::MOUSE_MOVE(), receive<MouseEvent>(mouseMove));
    app.addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(keyHandler));
    app.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(keyHandler));
    app.addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(resize));
    
    Timer redrawTimer("redraw", 0.016);redrawTimer.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(redraw));
    
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}