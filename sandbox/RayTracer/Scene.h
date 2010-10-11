#ifndef SCENE_H
#define SCENE_H

#include "Primitive.h"
#include "Plane.h"
#include "Sphere.h"
#include "Light.h"

#include <list>
#include <boost/shared_ptr.hpp>

#include "lost/common/Color.h"
#include "lost/math/Vec3.h"

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::math;

struct Scene : public list<shared_ptr<Primitive> > {

  Scene() {

    shared_ptr<Light> l(new Light(Vec3(5.0f, 2.0f, 0.0f), 0.1f));
    l->material.color(Color(0.6f, 0.6f, 0.6f));
    push_back(l);
    
    shared_ptr<Light> l2(new Light(Vec3(-5.0f, 0.0f, 0.0f), 0.1f));
    l2->material.color(Color(0.8f, 0.8f, 0.8f));
    push_back(l2);
    
    shared_ptr<Plane> p(new Plane(Vec3(0.0f, 1.0f, 0.0f), 4.4f));
    p->material.reflection(0.0f);
    p->material.diffuse(1.0f);
    p->material.color(Color(0.4f, 0.3f, 0.3f));
    push_back(p);

    shared_ptr<Plane> p2(new Plane(Vec3(0.4f, 0.0f, -1.0f), 12.0f));
    p2->material.reflection(0.0f);
    p2->material.refraction(0.0f);
    p2->material.specular(0.0f);
    p2->material.diffuse(0.6f);
    p2->material.color(Color(0.5f, 0.3f, 0.5f));
    push_back(p2);
    
    shared_ptr<Plane> p3(new Plane(Vec3(0.0f, -1.0f, 0.0f), 7.4f));
    p3->material.reflection(0.0f);
    p3->material.refraction(0.0f);
    p3->material.specular(0.0f);
    p3->material.diffuse(0.5f);
    p3->material.color(Color(0.4f, 0.7f, 0.7f));
    push_back(p3);

    shared_ptr<Sphere> s(new Sphere(Vec3(1.0f, -0.8f, 3.0f), 2.5f));
    s->material.reflection(0.6f);
    s->material.refraction(0.8f);
    s->material.refractionIndex(1.3f);
    s->material.color(Color(0.7f, 0.7f, 0.7f));
    push_back(s);

    shared_ptr<Sphere> s2(new Sphere(Vec3(-5.5f, -0.5f, 7.0f), 2.5f));
    s2->material.reflection(1.0f);
    s2->material.diffuse(0.1f);
    s2->material.color(Color(0.7f, 0.7f, 1.0f));
    push_back(s2);

    shared_ptr<Sphere> s3(new Sphere(Vec3(-1.5f, -3.8f, 1), 1.5f));
    s3->material.reflection(0.0f);
    s3->material.refraction( 0.8f );
    s3->material.color(Color(1.0f, 0.4f, 0.4f));
    push_back(s3);

    for (int x = 0; x < 8; ++x) {

      for (int y = 0; y < 7; ++y) {

        shared_ptr<Sphere> s(new Sphere(Vec3(-4.5f + x * 1.5f, -4.3f + y * 1.5f, 10), 0.3f));
        s->material.reflection(0.0f);
        s->material.refraction(0.0f);
        s->material.specular(0.6f);
        s->material.diffuse(0.6f);
        s->material.color(Color(0.3f, 1.0f, 0.4f));
        push_back(s);
      }
    }
  }
  
};

#endif
