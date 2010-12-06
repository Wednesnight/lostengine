#ifndef BULLETTEST_H
#define BULLETTEST_H

#include "lost/application/Tasklet.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/camera/Camera3D.h"
#include "lost/math/Vec3.h"
#include "lost/mesh/forward.h"

#include <btBulletDynamicsCommon.h>
#include <list>

struct BulletTest : public lost::application::Tasklet
{
  BulletTest();

  void init();
  bool startup();
  bool update();
  bool shutdown();

  void keyDown(lost::application::KeyEventPtr event);
  void mouseMove(lost::application::MouseEventPtr event);
  void resize(lost::application::ResizeEventPtr event);

private:
  bool running;

  btBroadphaseInterface* broadphase;
  btDefaultCollisionConfiguration* collisionConfiguration;
  btCollisionDispatcher* dispatcher;
  btSequentialImpulseConstraintSolver* solver;
  btDiscreteDynamicsWorld* dynamicsWorld;

  btCollisionShape* groundShape;
  btDefaultMotionState* groundMotionState;
  btRigidBody* groundRigidBody;

  lost::camera::Camera3DPtr camera;

  lost::mesh::MeshPtr mesh;
  btCollisionShape* meshShape;

  btRigidBody* instantiateMesh(const lost::math::Vec3& pos);
  void instantiateMesh(const lost::math::Vec3& pos, const lost::math::Vec3& impulse);
};

#endif
