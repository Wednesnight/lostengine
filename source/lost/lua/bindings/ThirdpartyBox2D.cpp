/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/lua/bindings/ThirdpartyBox2D.h"
#include "lost/lua/lostlua.h"
#include <Box2D/Box2D.h>

#include <slub/slub.h>

namespace slub {

  template<>
  struct converter<b2BodyType> {
    
    static bool check(lua_State* L, int index) {
      return converter<int>::check(L, index);
    }
    
    static b2BodyType get(lua_State* L, int index) {
      return (b2BodyType) luaL_checkinteger(L, index);
    }
    
    static int push(lua_State* L, b2BodyType t) {
      lua_pushinteger(L, t);
      return 1;
    }
    
  };  

}

namespace lost
{
  namespace lua
  {

    void ThirdpartyBox2Db2Vec2(lua_State* state) {
      slub::package(state, "box2d")
        .clazz<b2Vec2>("b2Vec2")
          .constructor()
          .constructor<float32, float32>()
          .method("SetZero", &b2Vec2::SetZero)
          .method("Set",&b2Vec2::Set)
          .field("x", &b2Vec2::x)
          .field("y", &b2Vec2::y);
    }

    void ThirdpartyBox2Db2AABB(lua_State* state) {
      slub::package(state, "box2d")
        .clazz<b2AABB>("b2AABB")
          .constructor()
          .method("IsValid", &b2AABB::IsValid)
          .method("GetCenter", &b2AABB::GetCenter)
          .method("GetExtents", &b2AABB::GetExtents)
          .method("Combine", &b2AABB::Combine)
          .method("Contains", &b2AABB::Contains)
          .field("lowerBound", &b2AABB::lowerBound)
          .field("upperBound", &b2AABB::upperBound);
    }    
    
    void ThirdpartyBox2Db2MassData(lua_State* state) {
      slub::package(state, "box2d")
        .clazz<b2MassData>("b2MassData")
          .constructor()
          .field("mass", &b2MassData::mass)
          .field("center", &b2MassData::center)
          .field("I", &b2MassData::I);
    }

    void ThirdpartyBox2Db2Transform(lua_State* state) {
      slub::package(state, "box2d")
        .clazz<b2Transform>("b2Transform")
          .constructor()
          .method("SetIdentity",&b2Transform::SetIdentity)
          .method("Set",&b2Transform::Set)
          .method("GetAngle",&b2Transform::GetAngle)
          .field("position", &b2Transform::position);
    }
    
    void ThirdpartyBox2Db2BodyDef(lua_State* state)
    {
      slub::package box2d(state, "box2d");

      box2d.clazz<b2BodyDef>("b2BodyDef")
        .constructor()
        .field("type",&b2BodyDef::type) // use the constants provided below
        .field("position",&b2BodyDef::position)
        .field("angle",&b2BodyDef::angle)
        .field("linearVelocity",&b2BodyDef::linearVelocity)
        .field("angularVelocity",&b2BodyDef::angularVelocity)
        .field("linearDamping",&b2BodyDef::linearDamping)
        .field("angularDamping",&b2BodyDef::angularDamping)
        .field("allowSleep",&b2BodyDef::allowSleep)
        .field("awake",&b2BodyDef::awake)
        .field("fixedRotation",&b2BodyDef::fixedRotation)
        .field("bullet",&b2BodyDef::bullet)
        .field("active",&b2BodyDef::active);

      box2d.enumerated("b2_staticBody", b2_staticBody);
      box2d.enumerated("b2_kinematicBody", b2_kinematicBody);
      box2d.enumerated("b2_dynamicBody", b2_dynamicBody);
    }

    void ThirdpartyBox2Db2Shape(lua_State* state)
    {
      slub::package(state,"box2d")
        .clazz<b2Shape>("b2Shape")
          .enumerated("e_unknown", b2Shape::e_unknown)
          .enumerated("e_circle", b2Shape::e_circle)
          .enumerated("e_polygon", b2Shape::e_polygon)
          .enumerated("e_typeCount", b2Shape::e_typeCount)
          .method("GetType",&b2Shape::GetType);
    }


    void ThirdpartyBox2Db2World(lua_State* state) {
      slub::package box2d(state, "box2d");

      box2d.clazz<b2DebugDraw>("b2DebugDraw");

      box2d.clazz<b2World>("b2World")
        .constructor<const b2Vec2&, bool>()
        .method("CreateBody", &b2World::CreateBody)
        .method("ClearForces", &b2World::ClearForces)
        .method("Step", &b2World::Step)
        .method("SetDebugDraw", &b2World::SetDebugDraw)
        .method("DrawDebugData", &b2World::DrawDebugData);
    }
    
    void ThirdpartyBox2Db2Filter(lua_State* state)
    {
      slub::package(state, "box2d")
        .clazz<b2Filter>("b2Filter")
          .field("categoryBits",&b2Filter::categoryBits)
          .field("maskBits",&b2Filter::maskBits)
          .field("groupIndex",&b2Filter::groupIndex);
    }

    void shape_setter(b2FixtureDef* fd, b2Shape* sh)
    {
      fd->shape = sh;
    }

    void ThirdpartyBox2Db2FixtureDef(lua_State* state)
    {
      slub::package(state, "box2d")
        .clazz<b2FixtureDef>("b2FixtureDef")
          .constructor()
          .method("setShape",&shape_setter)
          .field("shape",&b2FixtureDef::shape)
          .field("userData",&b2FixtureDef::userData)
          .field("friction",&b2FixtureDef::friction)
          .field("restitution",&b2FixtureDef::restitution)
          .field("density",&b2FixtureDef::density)
          .field("isSensor",&b2FixtureDef::isSensor)
          .field("filter",&b2FixtureDef::filter);
    }
    
    void ThirdpartyBox2Db2Body(lua_State* state)
    {
      slub::package(state, "box2d")
        .clazz<b2Body, slub::null_deleter>("b2Body")
          .method("CreateFixture",(b2Fixture*(b2Body::*)(const b2FixtureDef*)) &b2Body::CreateFixture)
          .method("CreateFixture",(b2Fixture*(b2Body::*)(const b2Shape*,float32)) &b2Body::CreateFixture)
          .method("SetTransform",&b2Body::SetTransform)
          .method("GetTransform",&b2Body::GetTransform)
          .method("GetPosition",&b2Body::GetPosition)
          .method("GetAngle",&b2Body::GetAngle)
          .method("GetWorldCenter",&b2Body::GetWorldCenter)
          .method("GetLocalCenter",&b2Body::GetLocalCenter)
          .method("SetLinearVelocity",&b2Body::SetLinearVelocity)
          .method("GetLinearVelocity",&b2Body::GetLinearVelocity)
          .method("SetAngularVelocity",&b2Body::SetAngularVelocity)
          .method("GetAngularVelocity",&b2Body::GetAngularVelocity)
          .method("ApplyForce",&b2Body::ApplyForce)
          .method("ApplyTorque",&b2Body::ApplyTorque)
          .method("ApplyLinearImpulse",&b2Body::ApplyLinearImpulse)
          .method("ApplyAngularImpulse",&b2Body::ApplyAngularImpulse)
          .method("GetMass",&b2Body::GetMass)
          .method("GetInertia",&b2Body::GetInertia)
          .method("GetMassData",&b2Body::GetMassData)
          .method("SetMassData",&b2Body::SetMassData)
          .method("ResetMassData",&b2Body::ResetMassData)
          .method("GetWorldPoint",&b2Body::GetWorldPoint)
          .method("GetWorldVector",&b2Body::GetWorldVector)
          .method("GetLocalPoint",&b2Body::GetLocalPoint)
          .method("GetLocalVector",&b2Body::GetLocalVector)
          .method("GetLinearVelocityFromWorldPoint",&b2Body::GetLinearVelocityFromWorldPoint)
          .method("GetLinearVelocityFromLocalPoint",&b2Body::GetLinearVelocityFromLocalPoint)
          .method("GetLinearDamping",&b2Body::GetLinearDamping)
          .method("SetLinearDamping",&b2Body::SetLinearDamping)
          .method("GetAngularDamping",&b2Body::GetAngularDamping)
          .method("SetAngularDamping",&b2Body::SetAngularDamping)
          .method("SetType",&b2Body::SetType)
          .method("GetType",&b2Body::GetType)
          .method("SetBullet",&b2Body::SetBullet)
          .method("IsBullet",&b2Body::IsBullet)
          .method("SetSleepingAllowed",&b2Body::SetSleepingAllowed)
          .method("IsSleepingAllowed",&b2Body::IsSleepingAllowed)
          .method("SetAwake",&b2Body::SetAwake)
          .method("IsAwake",&b2Body::IsAwake)
          .method("SetActive",&b2Body::SetActive)
          .method("IsActive",&b2Body::IsActive)
          .method("SetFixedRotation",&b2Body::SetFixedRotation)
          .method("IsFixedRotation",&b2Body::IsFixedRotation);
    }  
    
    void ThirdpartyBox2Db2Fixture(lua_State* state)
    {
      slub::package(state, "box2d")
        .clazz<b2Fixture, slub::null_deleter>("b2Fixture")
          .method("GetType",&b2Fixture::GetType)
//          .method("GetShape",&b2Fixture::GetShape)
          .method("SetSensor",&b2Fixture::SetSensor)
          .method("IsSensor",&b2Fixture::IsSensor)
          .method("SetFilterData",&b2Fixture::SetFilterData)
          .method("GetFilterData",&b2Fixture::GetFilterData)
//          .method("GetBody",&b2Fixture::GetBody)
//          .method("GetNext",&b2Fixture::GetNext)
          .method("GetUserData",&b2Fixture::GetUserData)
          .method("SetUserData",&b2Fixture::SetUserData)
          .method("TestPoint",&b2Fixture::TestPoint)
          .method("SetDensity",&b2Fixture::SetDensity)
          .method("GetDensity",&b2Fixture::GetDensity)
          .method("GetFriction",&b2Fixture::GetFriction)
          .method("SetFriction",&b2Fixture::SetFriction)
          .method("GetRestitution",&b2Fixture::GetRestitution)
          .method("SetRestitution",&b2Fixture::SetRestitution)
          .method("GetAABB",&b2Fixture::GetAABB);
    }

    void ThirdpartyBox2Db2PolygonShape(lua_State* state) {
      slub::package(state, "box2d")
        .clazz<b2PolygonShape>("b2PolygonShape")
          .extends<b2Shape>()
          .constructor()
          .method("SetAsBox",(void(b2PolygonShape::*)(float,float)) &b2PolygonShape::SetAsBox);
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
