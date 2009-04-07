/*
 *  batchtest.cpp
 *  Filt3rz
 *
 *  Created by Tony Kostanjsek on 04.04.09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "lost/mesh/Mesh.h"
#include "lost/mesh/Line.h"

using namespace lost;
using namespace lost::math;
using namespace lost::common;
using namespace lost::mesh;

void batchtest()
{
  gl::ElementArrayBuffer<Vec2> eb;

  Mesh2D mesh1;
  Mesh3D mesh2;

  Line2D l1(Vec2(0,0), Vec2(100,100));
  Line3D l2(Vec3(1,2,3), Vec3(4,5,6));
  
}