#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/KeySym.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/TouchEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/gl/gl.h"
#include "lost/lgl/lgl.h"
#include "chipmunk.h"
#include "moonbuggy.h"
#include "lost/common/FpsMeter.h"
#include "lost/gl/Utils.h"
#include "lost/math/Vec2.h"
#include "lost/event/Receive.h"

#define SLEEP_TICKS 16

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::event;
using namespace lost::application;

extern cpSpace *space;
extern cpBody *staticBody;

extern cpBody *chassis;

int ticks = 0;

shared_ptr<lost::common::FpsMeter> fpsMeter;

shared_ptr<Timer> redrawTimer;

void drawCircle(cpFloat x, cpFloat y, cpFloat r, cpFloat a)
{
	const int segs = 15;
	const cpFloat coef = 2.0*M_PI/(cpFloat)segs;
	
  float p[segs*2+2];
	int n;
  for(n = 0; n < segs; n++)
  {
		cpFloat rads = n*coef;
    p[n*2]   = r*cos(rads + a) + x;
    p[n*2+1] = r*sin(rads + a) + y;
	}
  p[segs*2]   = x;
  p[segs*2+1] = y;

  glVertexPointer(2, GL_FLOAT, 0, p);
  glDrawArrays(GL_LINE_LOOP, 0, segs+1);
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
  appInstance->context->drawLine(lost::math::Vec2(a.x, a.y), lost::math::Vec2(b.x, b.y));
}

void drawPolyShape(cpShape *shape)
{
	cpBody *body = shape->body;
	cpPolyShape *poly = (cpPolyShape *)shape;
	
	int num = poly->numVerts;
	cpVect *verts = poly->verts;
	
  float p[num*2];
	int i;
	for(i = 0; i < num; i++)
  {
		cpVect v = cpvadd(body->p, cpvrotate(verts[i], body->rot));
		p[i*2]   = v.x;
		p[i*2+1] = v.y;
	}
  
  glVertexPointer(2, GL_FLOAT, 0, p);
  glDrawArrays(GL_LINE_LOOP, 0, num);    
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
  float p[arb->numContacts*2];
	int i;
	for(i=0; i < arb->numContacts; i++)
  {
		cpVect v = arb->contacts[i].p;
		p[i*2]   = v.x;
    p[i*2+1] = v.y;
	}

  glVertexPointer(2, GL_FLOAT, 0, p);
  glDrawArrays(GL_POINTS, 0, arb->numContacts);    
}

void mouseButtonDown(shared_ptr<Event> event)
{
  moonBuggy_input(0, 0, 0, 0);
  //DOUT("mb stuff");
}

void mouseButtonUp(shared_ptr<Event> event)
{
  moonBuggy_input(0, 1, 0, 0);
  //DOUT("mb stuff");
}

void mouseMove(shared_ptr<MouseEvent> event)
{
  //DOUT("mm stuff");
}

void keyHandler(shared_ptr<KeyEvent> event)
{
  //DOUT("kb stuff");
  if (event->key == K_ESCAPE && event->pressed)
    appInstance->quit();
}

void resize(shared_ptr<ResizeEvent> event)
{
  DOUT("resize w: "<<event->width << " h:" << event->height);
  glViewport (0, 0, event->width, event->height);
}

void redraw(shared_ptr<TimerEvent> event)
{
  // init
  glClearColor(1.0, 1.0, 1.0, 0.0);GLDEBUG;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);GLDEBUG;
	glPointSize(3.0);GLDEBUG;
	glMatrixMode(GL_PROJECTION);GLDEBUG;
	glLoadIdentity();GLDEBUG;
	lglOrtho(-1000.0f, 1000.0f, -750.0, 750.0, -1.0, 1.0);GLDEBUG;
#if TARGET_OPENGL
	glScalef(2.0, 2.0, 1.0);GLDEBUG;
#else
	glScalef(4.0, 2.0, 1.0);GLDEBUG;
#endif
	glMatrixMode(GL_MODELVIEW);GLDEBUG;
  
  glEnableClientState(GL_VERTEX_ARRAY);GLDEBUG;
  glDisable(GL_DEPTH_TEST);GLDEBUG;
  glDisable(GL_TEXTURE_2D);GLDEBUG;
  
  // display
	int i;
	
	glLoadIdentity();GLDEBUG;
	glTranslatef(-chassis->p.x, -chassis->p.y, 0.0);GLDEBUG;
	
	glColor4f(0.0, 0.0, 0.0, 1.0);GLDEBUG;
	cpSpaceHashEach(space->activeShapes, &drawObject, NULL);
	cpSpaceHashEach(space->staticShapes, &drawObject, NULL);
  
	cpArray *bodies = space->bodies;
	int num = bodies->num;
	
  float p[num*2];
  glColor4f(0.0, 0.0, 1.0, 1.0);GLDEBUG;
  for(i = 0; i < num; i++){
    cpBody *body = static_cast<cpBody*>(bodies->arr[i]);
    p[i*2]   = body->p.x;
    p[i*2+1] = body->p.y;
  }
  glVertexPointer(2, GL_FLOAT, 0, p);GLDEBUG;
  glDrawArrays(GL_POINTS, 0, num);GLDEBUG;

  glColor4f(1.0, 0.0, 0.0, 1.0);GLDEBUG;
  cpArrayEach(space->arbiters, &drawCollisions, NULL);

  appInstance->context->set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(appInstance->displayAttributes->width, appInstance->displayAttributes->height));
  glMatrixMode(GL_MODELVIEW);GLDEBUG;
  glLoadIdentity();GLDEBUG;
  fpsMeter->render(appInstance->displayAttributes->width - fpsMeter->width, 0, event->passedSec);

  glDisableClientState(GL_VERTEX_ARRAY);GLDEBUG;

	appInstance->swapBuffers();
	ticks++;
	
	moonBuggy_update();
}

void init(shared_ptr<Event> event)
{
	fpsMeter.reset(new FpsMeter(appInstance->context));
	redrawTimer.reset(new Timer("redraw", 0.016));
  redrawTimer->addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(redraw));
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
    app.addEventListener(TouchEvent::TOUCHES_ENDED(), receive<TouchEvent>(mouseButtonUp));
    app.addEventListener(TouchEvent::TOUCHES_CANCELLED(), receive<TouchEvent>(mouseButtonUp));
    app.addEventListener(MouseEvent::MOUSE_DOWN(), receive<MouseEvent>(mouseButtonDown));
    app.addEventListener(TouchEvent::TOUCHES_BEGAN(), receive<TouchEvent>(mouseButtonDown));
    app.addEventListener(MouseEvent::MOUSE_MOVE(), receive<MouseEvent>(mouseMove));
    app.addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(keyHandler));
    app.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(keyHandler));
    app.addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(resize));
    app.addEventListener(ApplicationEvent::INIT(), receive<Event>(init));
    
    app.run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}
