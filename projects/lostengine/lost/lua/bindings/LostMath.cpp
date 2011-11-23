#include "lost/math/io.h"
#include "lost/lua/bindings/LostMath.h"
#include "lost/lua/lua.h"

#include "lost/math/Rect.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"
#include "lost/math/Matrix.h"
#include "lost/math/AABB.h"
#include <float.h>

#include <slub/slub.h>

using namespace lost::math;
using namespace slub;

namespace slub {
  template<>
  struct converter<const Vec2&> {
    
    static bool check(lua_State* L, int index) {
      return converter<Vec2>::check(L, index);
    }
    
    static const Vec2& get(lua_State* L, int index) {
      if (registry::isRegisteredType<Vec2>()) {
        wrapper<const Vec2*>* w = static_cast<wrapper<const Vec2*>*>(converter<Vec2>::checkudata(L, index));
        return *w->ref;
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
    static int push(lua_State* L, const Vec2& value) {
      if (registry::isRegisteredType<Vec2>()) {
        wrapper<const Vec2*>* w = wrapper<const Vec2*>::create(L, typeid(value));
        w->ref = &value;
        w->gc = false;
        luaL_getmetatable(L, registry::getTypeName<Vec2*>().c_str());
        lua_setmetatable(L, -2);
        return 1;
      }
      return 0;
    }
    
  };
}

namespace lost
{
  namespace lua
  {

    void LostMathRect(lua_State* state)
    {
      package(state, "lost").package("math")
        .clazz<Rect>("Rect")
          .constructor()
          .constructor<float, float, float, float>()
          .method("contains", &Rect::contains)
          .method("intersects", &Rect::intersects)
          .method("clipTo", &Rect::clipTo)
          .method("maxX", &Rect::maxX)
          .method("maxY", &Rect::maxY)
          .field("x", &Rect::x)
          .field("y", &Rect::y)
          .field("width", &Rect::width)
          .field("height", &Rect::height)
          .eq()
          .tostring();
    }

    void LostMathAABB(lua_State* state)
    {
      package(state, "lost").package("math")
        .clazz<AABB>("AABB")
          .constructor<Vec3, Vec3>()
          .field("pos", &AABB::pos)
          .field("size", &AABB::size);
    }
    
    void LostMathVec2(lua_State* state)
    {
      package math = package(state, "lost").package("math");

      math.clazz<Vec2>("Vec2")
            .constructor()
            .constructor<const Vec2&>()
            .constructor<float, float>()
            .method("zero", (void(Vec2::*)()) &Vec2::zero)
            .field("x", &Vec2::x)
            .field("y", &Vec2::y)
            .field("width", &Vec2::width)
            .field("height", &Vec2::height)
            .mul<Vec2, float>()
            .mul<float, Vec2>()
            .add<Vec2, Vec2>()
            .sub<Vec2, Vec2>()
            .eq()
            .tostring();

      math
        .function("len", (float(*)(const Vec2&))&len)
        .function("squlen", (float(*)(const Vec2&))&squlen)
        .function("normalise", (Vec2&(*)(Vec2&))&normalise)
        .function("perpendicular", (Vec2(*)(Vec2&))&perpendicular)
        .function("angle", (float(*)(const Vec2&, const Vec2&))&angle)
        .function("compare", (bool(*)(const Vec2&, const Vec2&, float))&compare);
    }

    void LostMathVec3(lua_State* state)
    {
      package math = package(state, "lost").package("math");
      
      math.clazz<Vec3>("Vec3")
        .constructor()
        .constructor<const Vec3&>()
        .constructor<float, float, float>()
        .method("zero", (void(Vec3::*)()) &Vec3::zero)
        .field("x", &Vec3::x)
        .field("y", &Vec3::y)            
        .field("z", &Vec3::z)            
        .mul<Vec3, float>()
        .mul<float, Vec3>()
        .add<Vec3, Vec3>()
        .sub<Vec3, Vec3>()
        .eq()
      .tostring();

      math
        .function("len", (float(*)(const Vec3&))&len)
        .function("squlen", (float(*)(const Vec3&))&squlen)
        .function("normalise", (Vec3&(*)(Vec3&))&normalise)
        .function("cross", &cross)
        .function("angle", (float(*)(const Vec3&, const Vec3&))&angle)
        .function("compare", (bool(*)(const Vec3&, const Vec3&, float))&compare);
    }

    void LostMathVec4(lua_State* state)
    {
	      package(state, "lost").package("math")
        .clazz<Vec4>("Vec4")
            .constructor()
            .constructor<const Vec4&>()
            .constructor<float, float, float, float>()
            .method("clear", &Vec4::clear)
            .field("x", &Vec4::x)
            .field("y", &Vec4::y)            
            .field("z", &Vec4::z)            
            .field("w", &Vec4::w)            
            .mul<Vec4, float>()
            .mul<float, Vec4>()
            .add<Vec4, Vec4>()
            .sub<Vec4, Vec4>()
            .eq()
            .tostring();
    }

    void LostMathMatrix(lua_State* state)
    {
      package math = package(state, "lost").package("math");

      math.clazz<Matrix>("Matrix")
        .constructor()
        .constructor<const Matrix&>()
        .mul<Matrix, Matrix>()
        .mul<Vec3, Vec3>()
        .mul<Vec4, Vec4>()
        .method("transpose", &Matrix::transpose)
        .method("row", (void(Matrix::*)(long, const Vec4&))&Matrix::row);

      math.clazz<MatrixRotX>("MatrixRotX")
        .extends<Matrix>()
        .constructor<float>();

      math.clazz<MatrixRotY>("MatrixRotY")
        .extends<Matrix>()
        .constructor<float>();

      math.clazz<MatrixRotZ>("MatrixRotZ")
        .extends<Matrix>()
        .constructor<float>();

      math.clazz<MatrixTranslation>("MatrixTranslation")
        .extends<Matrix>()
        .constructor<const lost::math::Vec3&>();

      math.clazz<MatrixScaling>("MatrixScaling")
        .extends<Matrix>()
        .constructor<const lost::math::Vec3&>();

      math.clazz<MatrixLookAt>("MatrixLookAt")
        .extends<Matrix>()
        .constructor<const lost::math::Vec3&, const lost::math::Vec3&, const lost::math::Vec3&>();
    }

    void LostMath(lua_State* state)
    {
      LostMathRect(state);
      LostMathVec2(state);
      LostMathVec3(state);
      LostMathVec4(state);
      LostMathMatrix(state);
      LostMathAABB(state);

      // constants
#if !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE && !defined ANDROID
      package(state, "lost").package("math").constant("EPSILON", FLT_EPSILON);
#else
      package(state, "lost").package("math").constant("EPSILON", DBL_EPSILON);
#endif
    }

  }
}
