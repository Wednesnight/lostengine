#include "RayTracer.h"

#include <boost/bind.hpp>
#include <float.h>

#include "lost/math/Rect.h"
#include "lost/application/Window.h"
#include "lost/event/EventDispatcher.h"
#include "lost/event/Receive.h"
#include "lost/application/KeyCode.h"
#include "lost/common/Logger.h"
#include "lost/rg/ClearColor.h"
#include "lost/rg/Clear.h"
#include "lost/rg/Camera.h"
#include "lost/rg/DepthTest.h"
#include "lost/rg/Draw.h"
#include "lost/mesh/Quad.h"
#include "lost/gl/gl.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/math/Vec2.h"


using namespace lost;
using namespace lost::application;
using namespace lost::bitmap;
using namespace lost::camera;
using namespace lost::common;
using namespace lost::event;
using namespace lost::gl;
using namespace lost::math;
using namespace lost::mesh;
using namespace lost::rg;


RayTracer::RayTracer() {

  running = false;
  needsRedraw = true;

  config.taskletName = "RayTracer";
  config.taskletWaitForEvents = false;
  config.taskletHasWindow = true;
  
  config.windowTitle = config.taskletName;
  config.windowRect = Rect(320, 240, 320, 240);

  bitmap = Bitmap::create(config.windowRect.width, config.windowRect.height, COMPONENTS_RGBA);
  camera = Camera2D::create(Rect(0, 0, config.windowRect.width, config.windowRect.height));

  eye.x = 0.0f;
  eye.y = 0.0f;
  eye.z = -5.0f;

  world = Rect(-4.0f, -3.0f, 8.0f, 6.0f);

  maxDepth = 6;
  
  showLights = false;

  reset();
}


bool RayTracer::startup() {

  if(!Tasklet::startup())
    return false;
  
  // setup event listeners
  window->dispatcher->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&RayTracer::keyEvent, this, _1)));
  window->dispatcher->addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(bind(&RayTracer::resizeEvent, this, _1)));

  texture = Texture::create(bitmap);
  mesh = Quad::create(texture, false);

  mesh->material->blend = true;
  mesh->material->color = whiteColor;
  mesh->material->shader = loadShader(loader, "basic_texture");

  renderNode->add(ClearColor::create(blackColor));
  renderNode->add(Clear::create(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  renderNode->add(DepthTest::create(false));
  renderNode->add(rg::Camera::create(camera));
  renderNode->add(Draw::create(mesh));
  
  running = true;
  return running;
}


bool RayTracer::update(double deltaSec) {

  if (running && Tasklet::update(deltaSec)) {

    render(config.framerate);
  }

  return running;
}


void RayTracer::resizeEvent(ResizeEventPtr event) {

  config.windowRect.width = event->width;
  config.windowRect.height = event->height;
  camera->viewport(Rect(0, 0, config.windowRect.width, config.windowRect.height));
  bitmap->init(config.windowRect.width, config.windowRect.height, COMPONENTS_RGBA);
  if (mesh) {
    
    mesh->updateSize(Vec2(config.windowRect.width, config.windowRect.height));
  }
  
  reset();
  needsRedraw = true;
}


void RayTracer::keyEvent(KeyEventPtr event)
{
  switch (event->key)
  {
    case K_ESCAPE:
      running = false;
      break;
    case K_Q:
      eye.y += 0.25f;
      reset();
      needsRedraw = true;
      break;
    case K_E:
      eye.y -= 0.25f;
      reset();
      needsRedraw = true;
      break;
    case K_W:
      eye.z -= 0.25f;
      reset();
      needsRedraw = true;
      break;
    case K_S:
      eye.z += 0.25f;
      reset();
      needsRedraw = true;
      break;
    case K_A:
      eye.x -= 0.25f;
      reset();
      needsRedraw = true;
      break;
    case K_D:
      eye.x += 0.25f;
      reset();
      needsRedraw = true;
      break;
  }
}


void RayTracer::reset() {

  scale.x = world.width / config.windowRect.width;
  scale.y = world.height / config.windowRect.height;

  startTime = 0;
  numRays = 0;
  numIntersectionTests = 0;
  currentLine = 0;
  currentPosition.x = world.x;
  currentPosition.y = world.y;
}


void RayTracer::render(double timeout) {

  if (needsRedraw) {

    double start = timer.getTime();
    if (startTime == 0) {

      startTime = start;
    }

    shared_ptr<Primitive> primitive;

    // Gaussian kernel
    Vec2 offset[9];
    offset[0] = Vec2(-scale.x, -scale.y); offset[1] = Vec2(0.0f, -scale.y); offset[2] = Vec2(scale.x, -scale.y);
    offset[3] = Vec2(-scale.x, 0.0f);     offset[4] = Vec2(0.0f, 0.0f);     offset[5] = Vec2(scale.x, 0.0f);
    offset[6] = Vec2(-scale.x, scale.y);  offset[7] = Vec2(0.0f, scale.y);  offset[8] = Vec2(scale.x, scale.y);
    
    float kernel[9];
    kernel[0] = 1.0/16.0; kernel[1] = 2.0/16.0;	kernel[2] = 1.0/16.0;
    kernel[3] = 2.0/16.0;	kernel[4] = 4.0/16.0;	kernel[5] = 2.0/16.0;
    kernel[6] = 1.0/16.0; kernel[7] = 2.0/16.0;	kernel[8] = 1.0/16.0;
    
    for (int y = currentLine; y < config.windowRect.height; ++y) {

      currentPosition.x = world.x;
      for (int x = 0; x < config.windowRect.width; ++x) {

        Color color(0.0f, 0.0f, 0.0f);

        Vec3 direction = Vec3(currentPosition.x, currentPosition.y, 0.0f) - eye;
        direction = normalise(direction);

        Ray ray(eye, direction);

        float distance = 0.0f;

        shared_ptr<Primitive> p = raytrace(ray, color, 1, 1.0f, distance);
        // apply gaussian blur for edges
        if (p != primitive) {

          primitive = p;

          color = Color(0.0f, 0.0f, 0.0f);
          for(int i = 0; i < 9; ++i) {
            
            Color tmp(0.0f, 0.0f, 0.0f);
            Vec3 dir = Vec3(currentPosition.x + offset[i].x, currentPosition.y + offset[i].y, 0.0f) - eye;
            dir = normalise(dir);
            Ray r(eye, dir);
            raytrace(r, tmp, 1, 1.0f, distance);
            color += tmp * kernel[i];
          }
        }

        bitmap->pixel(x, y, color);

        currentPosition.x += scale.x;
      }
      currentPosition.y += scale.y;

      if (timer.getElapsed(start) >= timeout && y < config.windowRect.height - 1) {

        texture->init(bitmap);
        currentLine = y + 1;
        return;
      }
    }

    IOUT("Primitives         : " << scene.size());
    IOUT("Rays               : " << numRays);
    IOUT("Intersection tests : " << numIntersectionTests);
    IOUT("Duration           : " << timer.getElapsed(startTime) << " seconds");

    texture->init(bitmap);
    
    startTime = 0;
    numRays = 0;
    numIntersectionTests = 0;
    currentLine = 0;
    currentPosition.x = world.x;
    currentPosition.y = world.y;

    needsRedraw = false;

//    bitmap->write("/Users/jomanto/Downloads/result.tga");
  }
}


#define EPSILON 0.0001f
shared_ptr<Primitive> RayTracer::raytrace(Ray& ray, Color& color, int depth, float rindex, float& distance) {

  numRays++;

  shared_ptr<Primitive> primitive;

  if (depth > maxDepth) {

    return primitive;
  }

  distance = 1000000.0f;

  int result;

  for (Scene::iterator target = scene.begin(); target != scene.end(); ++target) {

    numIntersectionTests++;

    int res;
    if ((!(*target)->isLight || showLights) && (res = (*target)->intersect(ray, distance)) != 0) {

      result = res;
      primitive = *target;
    }
  }

  if (!primitive) {

    return primitive;
  }

  
  if (primitive->isLight) {

    color = yellowColor;
  } else {

    Vec3 intersection = ray.origin + ray.direction * distance;

    for (Scene::iterator light = scene.begin(); light != scene.end(); ++light) {
      
      if ((*light)->isLight) {

				float shade = 1.0f;
        Vec3 lightVector = (*light)->getLightVector(intersection);
        float tdist = len(lightVector);
        lightVector = lightVector * (1.0f / tdist);
        Vec3 lightOrigin = intersection + lightVector * EPSILON;
        Ray r(lightOrigin, lightVector);
        for (Scene::iterator target = scene.begin(); target != scene.end(); ++target) {

          numIntersectionTests++;
          
          if (!(*target)->isLight && (*target) != primitive && (*target)->intersect(r, tdist) != 0) {
            shade = 0.0f;
            break;
          }
        }

        if (shade > 0.0f) {

          Vec3 lightNormal = (*light)->getLightNormal(intersection);
          Vec3 normal = primitive->getNormal(intersection);
          
            // diffuse
          if (primitive->material.diffuse() > 0.0f) {

            float dotProduct = normal * lightNormal;
            if (dotProduct > 0.0f) {

              float diffuse = dotProduct * primitive->material.diffuse() * shade;
              color += (primitive->material.color() * diffuse) * (*light)->material.color();
            }
          }

          // specular
          if (primitive->material.specular() > 0.0f) {

            Vec3 reflectionVector = lightNormal - normal * (lightNormal * normal) * 2.0f;
            float dotProduct = ray.direction * reflectionVector;
            if (dotProduct > 0.0f) {

              float specular = powf(dotProduct, 20.0f) * primitive->material.specular() * shade;
              color += (*light)->material.color() * specular;
            } 
          }
        }
      }
    }

    // reflection
    float reflection = primitive->material.reflection();
    if (reflection > 0.0f)
    {
      Vec3 normal = primitive->getNormal(intersection);
      Vec3 reflectionVector = ray.direction - normal * (ray.direction * normal) * 2.0f;
      if (depth < maxDepth) 
      {
        Color rcol(0.0f, 0.0f, 0.0f);
        float dist;
        Vec3 O = intersection + reflectionVector * EPSILON;
        Ray reflectionRay(O, reflectionVector);
        raytrace(reflectionRay, rcol, depth + 1, rindex, dist);
        color += primitive->material.color() * rcol * reflection;
      }
    }

    // refraction
		float refraction = primitive->material.refraction();
		if (refraction > 0.0f && depth < maxDepth) {

			float primitiveRIndex = primitive->material.refractionIndex();
			float n = rindex / primitiveRIndex;
			Vec3 normal = primitive->getNormal(intersection) * (float)result;
			float cosI = -(normal * ray.direction);
			float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
			if (cosT2 > 0.0f) {

				Vec3 T = (ray.direction * n) + normal * (n * cosI - sqrtf(cosT2));
				Color rcol(0.0f, 0.0f, 0.0f);
				float dist;
        Vec3 O = intersection + T * EPSILON;
        Ray refractionRay(O, T);
				raytrace(refractionRay, rcol, depth + 1, rindex, dist);
        // apply Beer's law
				Color absorbance = primitive->material.color() * 0.15f * -dist;
				Color transparency = Color(expf(absorbance.r()), expf(absorbance.g()), expf(absorbance.b()));
				color += rcol * transparency;
			}
		}
  }
  return primitive;
}
