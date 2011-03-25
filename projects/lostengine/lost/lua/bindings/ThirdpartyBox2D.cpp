#include "lost/lua/bindings/ThirdpartyBox2D.h"
#include "lost/lua/lua.h"
#include <Box2D/Box2D.h>

using namespace luabind;

namespace lost
{
  namespace lua
  {

    void ThirdpartyBox2Db2Vec2(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2Vec2>("b2Vec2")
          .def(constructor<>())
          .def(constructor<float32, float32>())
          .def("SetZero", &b2Vec2::SetZero)
          .def("Set", &b2Vec2::Set)
          .def_readwrite("x", &b2Vec2::x)
          .def_readwrite("y", &b2Vec2::y)
      ];
    }
    
    void ThirdpartyBox2Db2BodyDef(lua_State* state)
    {
      module(state, "box2d")
      [
        class_<b2BodyDef>("b2BodyDef")
        .def(constructor<>())
        .def_readwrite("position",&b2BodyDef::position)
        .def_readwrite("angle",&b2BodyDef::angle)
        .def_readwrite("linearVelocity",&b2BodyDef::linearVelocity)
        .def_readwrite("angularVelocity",&b2BodyDef::angularVelocity)
        .def_readwrite("linearDamping",&b2BodyDef::linearDamping)
        .def_readwrite("angularDamping",&b2BodyDef::angularDamping)
        .def_readwrite("allowSleep",&b2BodyDef::allowSleep)
        .def_readwrite("awake",&b2BodyDef::awake)
        .def_readwrite("fixedRotation",&b2BodyDef::fixedRotation)
        .def_readwrite("bullet",&b2BodyDef::bullet)
        .def_readwrite("active",&b2BodyDef::active)
      ];
    }

/*    void ThirdpartyBox2Db2Body(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2Body>("b2Body")
          .def("CreateShape", &b2Body::CreateShape)
          .def("DestroyShape", &b2Body::DestroyShape)
          .def("SetMass", &b2Body::SetMass)
          .def("SetMassFromShapes", &b2Body::SetMassFromShapes)
          .def("SetXForm", &b2Body::SetXForm)
          .def("GetXForm", &b2Body::GetXForm)
          .def("GetPosition", &b2Body::GetPosition)
          .def("GetAngle", &b2Body::GetAngle)
          .def("GetWorldCenter", &b2Body::GetWorldCenter)
          .def("GetLocalCenter", &b2Body::GetLocalCenter)
          .def("SetLinearVelocity", &b2Body::SetLinearVelocity)
          .def("GetLinearVelocity", &b2Body::GetLinearVelocity)
          .def("SetAngularVelocity", &b2Body::SetAngularVelocity)
          .def("GetAngularVelocity", &b2Body::GetAngularVelocity)
          .def("ApplyForce", &b2Body::ApplyForce)
          .def("ApplyTorque", &b2Body::ApplyTorque)
          .def("ApplyImpulse", &b2Body::ApplyImpulse)
          .def("GetMass", &b2Body::GetMass)
          .def("GetInertia", &b2Body::GetInertia)
          .def("GetWorldPoint", &b2Body::GetWorldPoint)
          .def("GetWorldVector", &b2Body::GetWorldVector)
          .def("GetLocalPoint", &b2Body::GetLocalPoint)
          .def("GetLocalVector", &b2Body::GetLocalVector)
          .def("GetLinearVelocityFromWorldPoint", &b2Body::GetLinearVelocityFromWorldPoint)
          .def("GetLinearVelocityFromLocalPoint", &b2Body::GetLinearVelocityFromLocalPoint)
          .def("IsBullet", &b2Body::IsBullet)
          .def("SetBullet", &b2Body::SetBullet)
          .def("IsStatic", &b2Body::IsStatic)
          .def("IsDynamic", &b2Body::IsDynamic)
          .def("IsFrozen", &b2Body::IsFrozen)
          .def("IsSleeping", &b2Body::IsSleeping)
          .def("AllowSleeping", &b2Body::AllowSleeping)
          .def("WakeUp", &b2Body::WakeUp)
          .def("PutToSleep", &b2Body::PutToSleep)
          .def("GetShapeList", &b2Body::GetShapeList)
          .def("GetJointList", &b2Body::GetJointList)
          .def("GetNext", &b2Body::GetNext)
          .def("GetUserData", &b2Body::GetUserData)
          .def("SetUserData", &b2Body::SetUserData)
          .def("GetWorld", &b2Body::GetWorld)
      ];
    }*/

    void ThirdpartyBox2Db2World(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2World>("b2World")
          .def(constructor<const b2Vec2&, bool>())
//          .def("CreateBody", &b2World::CreateBody)
          .def("Step", &b2World::Step)
      ];
    }
    
/*    void ThirdpartyBox2Db2AABB(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2AABB>("b2AABB")
          .def(constructor<>())
          .def("IsValid", &b2AABB::IsValid)
          .def_readwrite("lowerBound", &b2AABB::lowerBound)
          .def_readwrite("upperBound", &b2AABB::upperBound)
      ];
    }

    void ThirdpartyBox2Db2FilterData(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2FilterData>("b2FilterData")
          .def(constructor<>())
          .def_readwrite("categoryBits", &b2FilterData::categoryBits)
          .def_readwrite("maskBits", &b2FilterData::maskBits)
          .def_readwrite("groupIndex", &b2FilterData::groupIndex)
      ];
    }
    
    void ThirdpartyBox2Db2MassData(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2MassData>("b2MassData")
          .def(constructor<>())
          .def_readwrite("mass", &b2MassData::mass)
          .def_readwrite("center", &b2MassData::center)
          .def_readwrite("I", &b2MassData::I)
      ];
    }
    
    void ThirdpartyBox2Db2ShapeType(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2ShapeType>("b2ShapeType")
          .enum_("enum")
          [
            value("e_circleShape", e_circleShape),
            value("e_polygonShape", e_polygonShape)
          ]
      ];
    }
    
    void ThirdpartyBox2Db2ShapeDef(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2ShapeDef>("b2ShapeDef")
          .def(constructor<>())
          .def_readwrite("type", &b2ShapeDef::type)
          .def_readwrite("userData", &b2ShapeDef::userData)
          .def_readwrite("friction", &b2ShapeDef::friction)
          .def_readwrite("restitution", &b2ShapeDef::restitution)
          .def_readwrite("density", &b2ShapeDef::density)
          .def_readwrite("isSensor", &b2ShapeDef::isSensor)
          .def_readwrite("filter", &b2ShapeDef::filter)
      ];
    }
    
    void ThirdpartyBox2Db2Shape(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2Shape>("b2Shape")
          .def("GetType", &b2Shape::GetType)
          .def("IsSensor", &b2Shape::IsSensor)
          .def("SetFilterData", &b2Shape::SetFilterData)
          .def("GetFilterData", &b2Shape::GetFilterData)
          .def("GetBody", &b2Shape::GetBody)
          .def("GetNext", &b2Shape::GetNext)
          .def("GetUserData", &b2Shape::GetUserData)
          .def("SetUserData", &b2Shape::SetUserData)
          .def("TestPoint", &b2Shape::TestPoint)
          .def("TestSegment", &b2Shape::TestSegment)
          .def("ComputeAABB", &b2Shape::ComputeAABB)
          .def("ComputeSweptAABB", &b2Shape::ComputeSweptAABB)
          .def("ComputeMass", &b2Shape::ComputeMass)
          .def("GetSweepRadius", &b2Shape::GetSweepRadius)
          .def("GetFriction", &b2Shape::GetFriction)
          .def("GetRestitution", &b2Shape::GetRestitution)
      ];
    }
    
    void ThirdpartyBox2Db2PolygonDef(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2PolygonDef, b2ShapeDef>("b2PolygonDef")
          .def(constructor<>())
          .def("SetAsBox", (void(b2PolygonDef::*)(float32, float32)) &b2PolygonDef::SetAsBox)
          .def("SetAsBox", (void(b2PolygonDef::*)(float32, float32, const b2Vec2&, float32)) &b2PolygonDef::SetAsBox)
//          .def_readwrite("vertices", &b2PolygonDef::vertices)
          .def_readwrite("vertexCount", &b2PolygonDef::vertexCount)
      ];
    }
    
    void ThirdpartyBox2Db2PolygonShape(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2PolygonShape, b2Shape>("b2PolygonShape")
      ];
    }
    
    

*/
    void ThirdpartyBox2D(lua_State* state)
    {
      ThirdpartyBox2Db2Vec2(state);
      ThirdpartyBox2Db2BodyDef(state);
//      ThirdpartyBox2Db2Body(state);
      ThirdpartyBox2Db2World(state);
/*    
      ThirdpartyBox2Db2AABB(state);
      ThirdpartyBox2Db2FilterData(state);
      ThirdpartyBox2Db2MassData(state);
      ThirdpartyBox2Db2ShapeType(state);
      ThirdpartyBox2Db2ShapeDef(state);
      ThirdpartyBox2Db2Shape(state);
      ThirdpartyBox2Db2PolygonDef(state);
      ThirdpartyBox2Db2PolygonShape(state);
*/
    }

  }
}
