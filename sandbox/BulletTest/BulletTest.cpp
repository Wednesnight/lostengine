#include "BulletTest.h"
#include "lost/application/Window.h"
#include "lost/math/Rect.h"
#include "lost/common/Color.h"
#include "lost/rg/ClearColor.h"
#include "lost/rg/Clear.h"
#include "lost/rg/Camera.h"
#include "lost/rg/Draw.h"
#include "lost/mesh/Loader.h"
#include "lost/mesh/Quad.h"
#include "lost/gl/gl.h"
#include "lost/gl/Texture.h"
#include "lost/event/Receive.h"
#include "lost/application/KeyCode.h"
#include "lost/event/EventDispatcher.h"
#include "lost/platform/Platform.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/math/Quat.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

using namespace lost;
using namespace lost::application;
using namespace lost::camera;
using namespace lost::common;
using namespace lost::event;
using namespace lost::gl;
using namespace lost::math;
using namespace lost::mesh;
using namespace lost::platform;
using namespace lost::rg;

BulletTest::BulletTest()
{
  name = "BulletTest";
  waitForEvents = false;
}

void BulletTest::init()
{
  Tasklet::init();

  // setup window

  // setup event listeners
  window->dispatcher->addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(bind(&BulletTest::resize, this, _1)));
  window->dispatcher->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&BulletTest::keyDown, this, _1)));
  window->dispatcher->addEventListener(MouseEvent::MOUSE_MOVE(), receive<MouseEvent>(bind(&BulletTest::mouseMove, this, _1)));

  Tasklet::init();
}

bool BulletTest::startup()
{
  if(!Tasklet::startup())
    return false;
  
  camera = Camera3D::create(Rect(0,0,));
  camera->position(Vec3(0,10,25));
  camera->target(Vec3(0,8,0));
  camera->depth(Vec2(1.0, 1000.0));

  ShaderProgramPtr shader = loadShader(loader, "color");

  mesh = mesh::Loader::obj(loader->load("cube_tri.obj"));
  mesh->material->shader = shader;
  mesh->material->color = Color(.7,.1,.1);

  renderNode->add(ClearColor::create(blackColor));
  renderNode->add(Clear::create(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  renderNode->add(rg::Camera::create(camera));
  QuadPtr plane = Quad::create(Rect(0,0,1000,1000));
  plane->transform = MatrixRotX(90) * MatrixTranslation(Vec3(-500,-500,0));
  plane->material->shader = shader;
  plane->material->color = grayColor;
  renderNode->add(Draw::create(plane));

  broadphase = new btDbvtBroadphase();
  
  collisionConfiguration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfiguration);
  
  solver = new btSequentialImpulseConstraintSolver;
  
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
  
  dynamicsWorld->setGravity(btVector3(0,-10,0));
  
  
  groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
  
  groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
  btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
  groundRigidBody = new btRigidBody(groundRigidBodyCI);
  dynamicsWorld->addRigidBody(groundRigidBody);
  
  meshShape = new btBoxShape(btVector3(1,1,1));

  running = true;

  return running;
}

bool BulletTest::update()
{
  running = running && Tasklet::update();
  if (running)
  {
    static double passedSec = currentTimeSeconds();
    double currentSec = currentTimeSeconds();
    dynamicsWorld->stepSimulation(currentSec-passedSec);
    passedSec = currentSec;

    btCollisionObjectArray objects = dynamicsWorld->getCollisionObjectArray();
    for (unsigned int i = 0; i < objects.size(); i++)
    {
      btRigidBody *rigidBody = btRigidBody::upcast(objects[i]);
      if (!rigidBody || rigidBody == groundRigidBody)
      {
        continue;
      }
      
      if (!rigidBody->wantsSleeping())
      {
        btTransform trans;
        rigidBody->getMotionState()->getWorldTransform(trans);
        Mesh* mesh = (Mesh*)((btDefaultMotionState*)rigidBody->getMotionState())->m_userPointer;
        Matrix m;
        trans.getOpenGLMatrix(m.m);
        mesh->transform = m;
      }
    }
  }
  return running;
}

bool BulletTest::shutdown()
{
  btCollisionObjectArray objects = dynamicsWorld->getCollisionObjectArray();
  for (unsigned int i = 0; i < objects.size(); i++)
  {
    btRigidBody *rigidBody = btRigidBody::upcast(objects[i]);
    if (!rigidBody)
    {
      continue;
    }
    dynamicsWorld->removeRigidBody(rigidBody);
    delete rigidBody->getMotionState();
    delete rigidBody;
  }

  delete meshShape;
  delete groundShape;
  
  delete dynamicsWorld;
  delete solver;
  delete collisionConfiguration;
  delete dispatcher;
  delete broadphase;

  return Tasklet::shutdown();
}

btRigidBody* BulletTest::instantiateMesh(const Vec3& pos)
{
  MeshPtr m = Mesh::create();
  m->vertexBuffer = mesh->vertexBuffer;
  m->indexBuffer = mesh->indexBuffer;
  m->material = mesh->material;
  DrawPtr drawNode = Draw::create(m);
  renderNode->add(drawNode);
  
  btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(pos.x,pos.y,pos.z)));
  motionState->m_userPointer = m.get();
  btScalar mass = 1;
  btVector3 inertia(0,0,0);
  meshShape->calculateLocalInertia(mass,inertia);
  btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,motionState,meshShape,inertia);
  btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
  dynamicsWorld->addRigidBody(rigidBody);
  return rigidBody;
}

void BulletTest::instantiateMesh(const Vec3& pos, const Vec3& impulse)
{
  btRigidBody* rigidBody = instantiateMesh(pos);
  rigidBody->applyCentralImpulse(btVector3(impulse.x,impulse.y,impulse.z));
}

void BulletTest::keyDown(KeyEventPtr event)
{
  switch (event->key)
  {
    case K_SPACE:
      instantiateMesh(camera->position(), camera->direction());
      break;

    case K_ESCAPE:
      running = false;
      break;

    case K_W:
      camera->move(Vec3(0,0,-1));
      break;

    case K_S:
      camera->move(Vec3(0,0,1));
      break;

    case K_A:
      camera->move(Vec3(-1,0,0));
      break;

    case K_D:
      camera->move(Vec3(1,0,0));
      break;
  }
}

void BulletTest::mouseMove(MouseEventPtr event)
{
  static Vec2 lastPos = event->pos;
  Vec2 pos = event->pos - lastPos;
  camera->rotate(Vec3(-pos.y*0.5f, pos.x*0.5f, 0));
  lastPos = event->pos;
}

void BulletTest::resize(ResizeEventPtr event)
{
  camera->viewport(Rect(0,0,event->width,event->height));
}
