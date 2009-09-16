/* Copyright (c) 2007 Scott Lembcke
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <math.h>

#include "chipmunk.h"
#include "moonbuggy.h"

/* NOTE: if you haven't read the Chipmunk documentation, you might
   want to do that now. The tutorial doesn't assume that you've read
   the entire API, but you should at least read the overview. Also, I
   don't deal with memory management issues in this tutorial. Keep
   that in mind. */

/* Number of terrain points. */
int terrain_points = 200;

/* This is the terrain height data. */
cpFloat terrain_data[] = {
  660.00, 660.00, 660.00, 660.00, 673.21, 688.42, 694.56, 692.55,
  685.40, 676.12, 667.75, 662.45, 658.93, 655.42, 650.17, 641.49,
  627.92, 610.08, 589.01, 565.71, 541.23, 516.58, 492.56, 469.57,
  447.97, 428.13, 410.60, 397.25, 392.66, 394.89, 400.70, 406.82,
  410.93, 413.87, 416.91, 421.30, 428.24, 436.05, 440.41, 437.09,
  421.93, 394.41, 355.57, 308.78, 257.99, 207.18, 160.31, 120.81,
  89.20, 65.17, 48.43, 38.67, 36.68, 45.03, 64.17, 92.26, 128.76,
  173.27, 224.20, 278.84, 334.48, 388.43, 438.31, 483.95, 525.96,
  564.95, 601.54, 633.88, 655.05, 665.87, 667.79, 662.25, 650.01,
  629.92, 604.68, 577.50, 551.55, 529.69, 512.49, 502.04, 500.20,
  502.72, 508.57, 518.31, 531.15, 545.99, 561.70, 577.30, 593.74,
  610.97, 628.13, 644.35, 658.81, 672.13, 684.78, 696.72, 708.00,
  718.65, 728.17, 736.14, 742.62, 747.63, 751.20, 752.58, 750.20,
  743.02, 730.05, 709.98, 682.99, 651.49, 616.61, 579.47, 541.18,
  503.87, 471.12, 444.10, 423.86, 411.44, 407.95, 414.29, 430.28,
  453.64, 482.36, 514.10, 545.66, 577.48, 610.42, 645.32, 682.66,
  719.61, 754.76, 787.26, 816.26, 840.95, 861.10, 876.94, 888.71,
  896.61, 900.84, 900.46, 894.59, 882.69, 864.24, 838.69, 805.77,
  765.56, 718.19, 670.07, 626.07, 586.87, 551.65, 518.20, 484.33,
  447.81, 408.39, 367.51, 324.70, 279.44, 231.25, 181.20, 134.59,
  96.96, 66.40, 40.75, 18.74, 1.97, -8.96, -13.56, -11.33, -2.28,
  11.64, 29.88, 52.04, 78.07, 108.53, 139.94, 171.90, 204.54,
  238.00, 272.25, 305.61, 336.90, 365.19, 389.61, 409.28, 424.38,
  434.79, 438.85, 437.12, 431.08, 422.77, 412.26, 398.92, 382.10,
  361.16, 336.82, 311.06, 285.61, 262.18, 242.50
};

/* This is the space we will be using for the . */
cpSpace *space;

/* This is the rigid body we will attach the ground segments to. */
cpBody *staticBody;

/* The rigid bodies for the chassis and wheel. They are declared here
   so they can be accessed from the update function. */
cpBody *chassis, *wheel1, *wheel2;

/* The init funtion is doing most of the work in this tutorial. We
   create a space and populate it with a bunch of interesting
   objects. */
void
moonBuggy_init()
{
  /* We first create a new space */
  space = cpSpaceNew();

  /* Next, you'll want to set the properties of the space such as the
     number of iterations to use in the constraint solver, the amount
     of gravity, or the amount of damping. In this case, we'll just
     set the gravity. */
  space->gravity = cpv(0.0f, -900.0f);

  /* This step is optional. While you don't have to resize the spatial
  hashes, doing so can greatly increase the speed of the collision
  detection. The first number should be the expected average size of
  the objects you are going to have, the second number is related to
  the number of objects you are putting. In general, if you have more
  objects, you want the number to be bigger, but only to a
  point. Finding good numbers to use here is largely going to be guess
  and check. */
  cpSpaceResizeStaticHash(space, 50.0f, 2000);
  cpSpaceResizeActiveHash(space, 50.0f, 100);
	
  /* This is the rigid body that we will be attaching our ground line
     segments to. We don't want it to move, so we give it an infinite
     mass and moment of inertia. We also aren't going to add it to our
     space. If we did, it would fall under the influence of gravity,
     and the ground would fall with it. */
  staticBody = cpBodyNew(INFINITY, INFINITY);
  
  /* This loop adds line segments for the terrain. */
  int i;
  cpVect a = cpv(0.0, terrain_data[0]);
  for(i=1; i<terrain_points; i++){
    cpVect b = cpv(i*50.0, terrain_data[i]);
    
    /* Collision shapes are attached to rigid bodies. When the rigid
       body moves, the collision shapes attached to it move as
       well. For the ground, we want to attach its collision shapes
       (the line segments) to our static, non-moving body that we've
       created. */
    cpShape *seg = cpSegmentShapeNew(staticBody, a, b, 0.0f);

    /* After you create a shape, you'll probably want to set some of
       it's properties. Possibilities include elasticity (e), surface
       velocity (surface_v), and friction (u). We'll just set the
       friction. */
    seg->u = 1.0;

    /* Lastly, we need to add it to a space for it to do any
       good. Because the ground never moves, we want to add it to the
       static shapes to allow Chipmunk to cache the collision
       information. Adding the line segments to the active shapes
       would work, but would be slower.  */
    cpSpaceAddStaticShape(space, seg);
		
    a = b;
  }
  
  /* These are the vertexes that will be used to create the buggy's
     chassis shape. You *MUST* specify them in a conterclockwise
     order, and they *MUST* form a convex polygon (no dents). If you
     need a non-convex polygon, simply attach more than one shape to
     the body. */
  cpVect chassis_verts[] = {
    cpv(-18,-18),
    cpv(-18, 18),
    cpv( 18, 18),
    cpv( 18,-18),
  };
	
  cpFloat chassis_mass = 5.0f;

  /* The moment of inertia (usually written simply as 'i') is like the
     mass of an object, but applied to its rotation. An object with a
     higher moment of inertia is harder to spin. Chipmunk has a couple
     of helper functions to help you calculate these. */
  cpFloat chassis_moment = cpMomentForPoly(chassis_mass, 4, chassis_verts, cpvzero);
	
  /* Create the rigid body for our buggy with the mass and moment of
     inertia we calculated. */
  chassis = cpBodyNew(chassis_mass, chassis_moment);

  /* Like usual, after something, you'll want to set it
     properties. Let's set the buggy's location to be just above the
     start of the terrain. */
  chassis->p = cpv(100.0f, 800.0f);

  /* Lastly, we need to add the body to a space for it to be
     useful. */
  cpSpaceAddBody(space, chassis);
	
	
	
  cpFloat wheel_offset_x = 40.0f;
  cpFloat wheel_offset_y = 30.0f;
	
  cpFloat wheel_radius = 15.0f;
  cpFloat wheel_mass = 1.0f;
  cpFloat wheel_moment = cpMomentForCircle(wheel_mass, wheel_radius, 0.0f, cpvzero);
	
  /* Next, we create our wheels, move them next to the chassis, and
     add them to the space. */
  wheel1 = cpBodyNew(wheel_mass, wheel_moment);
  wheel2 = cpBodyNew(wheel_mass, wheel_moment);
  wheel1->p = cpvadd(chassis->p, cpv(-wheel_offset_x, -wheel_offset_y));
  wheel2->p = cpvadd(chassis->p, cpv( wheel_offset_x, -wheel_offset_y));
  cpSpaceAddBody(space, wheel1);
  cpSpaceAddBody(space, wheel2);
	
  /* In order to attach the wheels to the chassis, we need to create
     joints. All of the joints are created slightly differently, but
     the all assume that the bodies they are connecting are already in
     place when you create the joint. Pin joints connect two bodies
     together with a massless rod. We want to attach the joint to the
     wheel at its center so that it rolls nicely, we could attach it
     to the chassis anywhere, though we'll just attach it to the
     center of the chassis as well. */
  cpSpaceAddJoint(space, cpPinJointNew(chassis, wheel1, cpvzero, cpvzero));
  cpSpaceAddJoint(space, cpPinJointNew(chassis, wheel2, cpvzero, cpvzero));
	
  /* Now we need to attach collision shapes to the chassis and
     wheels. The shapes themselves contain no useful information to
     you. Normally you'd keep them around simply so that you can
     remomve them from the space, or free them later. In this tutorial
     we won't be removing anything, and we're being lax about memory
     management. So we'll just recycle the same variable. */
  cpShape *shape;
	
  /* We create a polygon shape for the chassis. */
  shape = cpPolyShapeNew(chassis, 4, chassis_verts, cpvzero);
  shape->u = 0.5;
  cpSpaceAddShape(space, shape);
	
  /* Now we create some shapes for the wheels */
  shape = cpCircleShapeNew(wheel1, wheel_radius, cpvzero);
  shape->u = 1.5;
  cpSpaceAddShape(space, shape);

  shape = cpCircleShapeNew(wheel2, wheel_radius, cpvzero);
  shape->u = 1.5;
  cpSpaceAddShape(space, shape);
}

/* This variable will be used to store the user input. When the mouse
   button is down, the power becomes 1.0. */
cpFloat input_power = 0.0;

void
moonBuggy_input(int button, int state, int x, int y)
{
  input_power = (state ? 0.0 : 1.0);
}

/* This function is called everytime a frame is drawn. */
void
moonBuggy_update()
{
  /* Collision detection isn't amazing in Chipmunk yet. Fast moving
     objects can pass right though eachother if they move to much in a
     single step. To deal with this, you can just make your steps
     smaller and cpSpaceStep() sevral times. */
  int substeps = 3;

  /* This is the actual time step that we will use. */
  cpFloat dt = (1.0f/60.0f) / (cpFloat)substeps;
	
  int i;
  for(i=0; i<substeps; i++){
    /* In Chipmunk, the forces and torques on a body are not reset
       every step. If you keep accumulating forces on an object, it
       will quickly explode. Comment these lines out to see what I
       mean. This function simply zeros the forces and torques applied
       to a give body. */
    cpBodyResetForces(chassis);
    cpBodyResetForces(wheel1);
    cpBodyResetForces(wheel2);
		
    /* We need to calculate how much torque to apply to the wheel. The
       following equation roughly simulates a motor with a top
       speed. */
    cpFloat max_w = -100.0f;
    cpFloat torque = 60000.0f*fminf((wheel1->w - input_power*max_w)/max_w, 1.0f);

    /* Apply the torque to both the chassis and the wheel in opposite directions. */
    wheel1->t += torque;
    chassis->t -= torque;
		
    /* To simulate the nice soft suspension of the buggy, we apply
       spring forces between the wheels and chassis. This function
       takes a lot of parameters, read the documentation for a
       detailed description. */
    cpDampedSpring(chassis, wheel1, cpv(-40.0f, 40.0f), cpvzero, 70.0f, 400.0f, 15.0f, dt);
    cpDampedSpring(chassis, wheel2, cpv( 40.0f, 40.0f), cpvzero, 70.0f, 400.0f, 15.0f, dt);
    
    /* Finally, we step the space */
    cpSpaceStep(space, dt);
  }
}
