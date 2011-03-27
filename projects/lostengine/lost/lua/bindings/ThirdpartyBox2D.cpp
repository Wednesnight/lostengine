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
          .def("Set",&b2Vec2::Set)
          .def_readwrite("x", &b2Vec2::x)
          .def_readwrite("y", &b2Vec2::y)
      ];
    }

    void ThirdpartyBox2Db2AABB(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2AABB>("b2AABB")
          .def(constructor<>())
          .def("IsValid", &b2AABB::IsValid)
          .def("GetCenter", &b2AABB::GetCenter)
          .def("GetExtents", &b2AABB::GetExtents)
          .def("Combine", &b2AABB::Combine)
          .def("Contains", &b2AABB::Contains)
          .def_readwrite("lowerBound", &b2AABB::lowerBound)
          .def_readwrite("upperBound", &b2AABB::upperBound)
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

    void ThirdpartyBox2Db2Transform(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2Transform>("b2Transform")
          .def(constructor<>())
          .def("SetIdentity",&b2Transform::SetIdentity)
          .def("Set",&b2Transform::Set)
          .def("GetAngle",&b2Transform::GetAngle)
          .def_readwrite("position", &b2Transform::position)
      ];
    }
    
    void ThirdpartyBox2Db2BodyDef(lua_State* state)
    {
      module(state, "box2d")
      [
        class_<b2BodyDef>("b2BodyDef")
        .def(constructor<>())
        .def_readwrite("type",&b2BodyDef::type) // use the constants provided below
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
      luabind::globals(state)["box2d"]["b2_staticBody"] = b2_staticBody;
      luabind::globals(state)["box2d"]["b2_kinematicBody"] = b2_kinematicBody;
      luabind::globals(state)["box2d"]["b2_dynamicBody"] = b2_dynamicBody;
    }

    void ThirdpartyBox2Db2Shape(lua_State* state)
    {
      module(state,"box2d")
      [
        class_<b2Shape>("b2Shape")
        .enum_("enum")
        [
          value("e_unknown",b2Shape::e_unknown),
          value("e_circle",b2Shape::e_circle),
          value("e_polygon",b2Shape::e_polygon),
          value("e_typeCount",b2Shape::e_typeCount)
        ]
//        .def(constructor<>())
        .def("GetType",&b2Shape::GetType)
      ];
    }


    void ThirdpartyBox2Db2World(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2World>("b2World")
          .def(constructor<const b2Vec2&, bool>())
          .def("CreateBody", &b2World::CreateBody)
          .def("ClearForces", &b2World::ClearForces)
          .def("Step", &b2World::Step)
      ];
    }
    
    void ThirdpartyBox2Db2Filter(lua_State* state)
    {
      module(state, "box2d")
      [
        class_<b2Filter>("b2Filter")
        .def_readwrite("categoryBits",&b2Filter::categoryBits)
        .def_readwrite("maskBits",&b2Filter::maskBits)
        .def_readwrite("groupIndex",&b2Filter::groupIndex)
      ];
    }

    void shape_setter(b2FixtureDef* fd, b2Shape* sh)
    {
      fd->shape = sh;
    }

    void ThirdpartyBox2Db2FixtureDef(lua_State* state)
    {
      module(state, "box2d")
      [
        class_<b2FixtureDef>("b2FixtureDef")
        .def(constructor<>())
        .def("setShape",&shape_setter)
        .def_readwrite("shape",&b2FixtureDef::shape)
        .def_readwrite("userData",&b2FixtureDef::userData)
        .def_readwrite("friction",&b2FixtureDef::friction)
        .def_readwrite("restitution",&b2FixtureDef::restitution)
        .def_readwrite("density",&b2FixtureDef::density)
        .def_readwrite("isSensor",&b2FixtureDef::isSensor)
        .def_readwrite("filter",&b2FixtureDef::filter)
      ];
    }
    
    void ThirdpartyBox2Db2Body(lua_State* state)
    {
      module(state, "box2d")
      [
        class_<b2Body>("b2Body")
        .def("CreateFixture",(b2Fixture*(b2Body::*)(const b2FixtureDef*)) &b2Body::CreateFixture)
        .def("CreateFixture",(b2Fixture*(b2Body::*)(const b2Shape*,float32)) &b2Body::CreateFixture)
        .def("SetTransform",&b2Body::SetTransform)
        .def("GetTransform",&b2Body::GetTransform)
        .def("GetPosition",&b2Body::GetPosition)
        .def("GetAngle",&b2Body::GetAngle)
        .def("GetWorldCenter",&b2Body::GetWorldCenter)
        .def("GetLocalCenter",&b2Body::GetLocalCenter)
        .def("SetLinearVelocity",&b2Body::SetLinearVelocity)
        .def("GetLinearVelocity",&b2Body::GetLinearVelocity)
        .def("SetAngularVelocity",&b2Body::SetAngularVelocity)
        .def("GetAngularVelocity",&b2Body::GetAngularVelocity)
        .def("ApplyForce",&b2Body::ApplyForce)
        .def("ApplyTorque",&b2Body::ApplyTorque)
        .def("ApplyLinearImpulse",&b2Body::ApplyLinearImpulse)
        .def("ApplyAngularImpulse",&b2Body::ApplyAngularImpulse)
        .def("GetMass",&b2Body::GetMass)
        .def("GetInertia",&b2Body::GetInertia)
        .def("GetMassData",&b2Body::GetMassData)
        .def("SetMassData",&b2Body::SetMassData)
        .def("ResetMassData",&b2Body::ResetMassData)
        .def("GetWorldPoint",&b2Body::GetWorldPoint)
        .def("GetWorldVector",&b2Body::GetWorldVector)
        .def("GetLocalPoint",&b2Body::GetLocalPoint)
        .def("GetLocalVector",&b2Body::GetLocalVector)
        .def("GetLinearVelocityFromWorldPoint",&b2Body::GetLinearVelocityFromWorldPoint)
        .def("GetLinearVelocityFromLocalPoint",&b2Body::GetLinearVelocityFromLocalPoint)
        .def("GetLinearDamping",&b2Body::GetLinearDamping)
        .def("SetLinearDamping",&b2Body::SetLinearDamping)
        .def("GetAngularDamping",&b2Body::GetAngularDamping)
        .def("SetAngularDamping",&b2Body::SetAngularDamping)
        .def("SetType",&b2Body::SetType)
        .def("GetType",&b2Body::GetType)
        .def("SetBullet",&b2Body::SetBullet)
        .def("IsBullet",&b2Body::IsBullet)
        .def("SetSleepingAllowed",&b2Body::SetSleepingAllowed)
        .def("IsSleepingAllowed",&b2Body::IsSleepingAllowed)
        .def("SetAwake",&b2Body::SetAwake)
        .def("IsAwake",&b2Body::IsAwake)
        .def("SetActive",&b2Body::SetActive)
        .def("IsActive",&b2Body::IsActive)
        .def("SetFixedRotation",&b2Body::SetFixedRotation)
        .def("IsFixedRotation",&b2Body::IsFixedRotation)
      ];
    }  
    
    void ThirdpartyBox2Db2Fixture(lua_State* state)
    {
      module(state, "box2d")
      [
        class_<b2Fixture>("b2Fixture")
        .def("GetType",&b2Fixture::GetType)
//        .def("GetShape",&b2Fixture::GetShape)
        .def("SetSensor",&b2Fixture::SetSensor)
        .def("IsSensor",&b2Fixture::IsSensor)
        .def("SetFilterData",&b2Fixture::SetFilterData)
        .def("GetFilterData",&b2Fixture::GetFilterData)
//        .def("GetBody",&b2Fixture::GetBody)
//        .def("GetNext",&b2Fixture::GetNext)
        .def("GetUserData",&b2Fixture::GetUserData)
        .def("SetUserData",&b2Fixture::SetUserData)
        .def("TestPoint",&b2Fixture::TestPoint)
        .def("SetDensity",&b2Fixture::SetDensity)
        .def("GetDensity",&b2Fixture::GetDensity)
        .def("GetFriction",&b2Fixture::GetFriction)
        .def("SetFriction",&b2Fixture::SetFriction)
        .def("GetRestitution",&b2Fixture::GetRestitution)
        .def("SetRestitution",&b2Fixture::SetRestitution)
        .def("GetAABB",&b2Fixture::GetAABB)
      ];
    }

    void ThirdpartyBox2Db2PolygonShape(lua_State* state) {
      module(state, "box2d")
      [
        class_<b2PolygonShape, b2Shape>("b2PolygonShape")
        .def(constructor<>())
        .def("SetAsBox",(void(b2PolygonShape::*)(float,float)) &b2PolygonShape::SetAsBox)
      ];
    }
    
/*    
    
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
    
    
    

*/
    void ThirdpartyBox2D(lua_State* state)
    {
      ThirdpartyBox2Db2Vec2(state);
      ThirdpartyBox2Db2AABB(state);
      ThirdpartyBox2Db2MassData(state);
      ThirdpartyBox2Db2Transform(state);
      ThirdpartyBox2Db2BodyDef(state);
      ThirdpartyBox2Db2Shape(state);
      ThirdpartyBox2Db2Filter(state);
      ThirdpartyBox2Db2FixtureDef(state);
      ThirdpartyBox2Db2Fixture(state);
      ThirdpartyBox2Db2Body(state);
      ThirdpartyBox2Db2World(state);
      ThirdpartyBox2Db2PolygonShape(state);
/*    
      ThirdpartyBox2Db2ShapeDef(state);
      ThirdpartyBox2Db2PolygonDef(state);
*/
    }

  }
}
