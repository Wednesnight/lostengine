#include "lost/math/io.h"
#include "lost/lua/bindings/LostMath.h"
#include "lost/lua/lua.h"
#include <luabind/operator.hpp>

#include "lost/math/Rect.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"
#include "lost/math/Matrix.h"
#include "lost/math/AABB.h"
#include <float.h>

using namespace luabind;
using namespace lost::math;

namespace lost
{
  namespace lua
  {

    void LostMathRect(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("math")
        [
          class_<Rect>("Rect")
            .def(constructor<>()) 
            .def(constructor<float, float, float, float>())
            .def("contains", &Rect::contains)
            .def("intersects", &Rect::intersects)
            .def("clipTo", &Rect::clipTo)
            .def(self == other<Rect>())
            .def(tostring(self))
            .def("maxX", &Rect::maxX)
            .def("maxY", &Rect::maxY)
            .def_readwrite("x", &Rect::x)
            .def_readwrite("y", &Rect::y)
            .def_readwrite("width", &Rect::width)
            .def_readwrite("height", &Rect::height)
        ]
      ];
    }

    void LostMathAABB(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("math")
        [
          class_<AABB>("AABB")
            .def(constructor<Vec3, Vec3>())
            .def_readwrite("pos", &AABB::pos)
            .def_readwrite("size", &AABB::size)
        ]
      ];
    }
    
    void LostMathVec2(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("math")
        [
          class_<Vec2>("Vec2")
            .def(constructor<>())
            .def(constructor<const Vec2&>())
            .def(constructor<float, float>())
            .def("zero", (void(Vec2::*)()) &Vec2::zero)
            .def_readwrite("x", &Vec2::x)
            .def_readwrite("y", &Vec2::y)
            .def_readwrite("width", &Vec2::width)
            .def_readwrite("height", &Vec2::height)
            .def(self * float())
            .def(float() * self)
            .def(self * other<Vec2>())
            .def(self + other<Vec2>())
            .def(self - other<Vec2>())
            .def(self == other<Vec2>())
            .def(tostring(self)),

          def("len", (float(*)(const Vec2&))&len),
          def("squlen", (float(*)(const Vec2&))&squlen),
          def("normalise", (Vec2&(*)(Vec2&))&normalise),
          def("perpendicular", (Vec2(*)(Vec2&))&perpendicular),
          def("angle", (float(*)(const Vec2&, const Vec2&))&angle),
          def("compare", (bool(*)(const Vec2&, const Vec2&, float))&compare)
        ]
      ];
    }

    void LostMathVec3(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("math")
        [
          class_<Vec3>("Vec3")
            .def(constructor<>())
            .def(constructor<const Vec3&>())
            .def(constructor<float, float, float>())
            .def("zero", (void(Vec3::*)()) &Vec3::zero)
            .def_readwrite("x", &Vec3::x)
            .def_readwrite("y", &Vec3::y)            
            .def_readwrite("z", &Vec3::z)            
            .def(self * float())
            .def(self * other<Vec3>())
            .def(self + other<Vec3>())
            .def(self - other<Vec3>())
            .def(self == other<Vec3>())
            .def(tostring(self)),

          def("len", (float(*)(const Vec3&))&len),
          def("squlen", (float(*)(const Vec3&))&squlen),
          def("normalise", (Vec3&(*)(Vec3&))&normalise),
          def("cross", &cross),
          def("angle", (float(*)(const Vec3&, const Vec3&))&angle),
          def("compare", (bool(*)(const Vec3&, const Vec3&, float))&compare)
        ]
      ];
    }

    void LostMathVec4(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("math")
        [
          class_<Vec4>("Vec4")
            .def(constructor<>())
            .def(constructor<const Vec4&>())
            .def(constructor<float, float, float, float>())
            .def_readwrite("x", &Vec4::x)
            .def_readwrite("y", &Vec4::y)            
            .def_readwrite("z", &Vec4::z)            
            .def_readwrite("w", &Vec4::w)            
            .def(self * float())
            .def(self * other<Vec4>())
            .def(self + other<Vec4>())
            .def(self - other<Vec4>())
            .def(self == other<Vec4>())
            .def(tostring(self))
            .def("clear", &Vec4::clear)
        ]
      ];
    }

    void LostMathMatrix(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("math")
        [
          class_<Matrix>("Matrix")
            .def(constructor<>())
            .def(constructor<const Matrix&>())
            .def(const_self * other<Matrix>())
            .def(const_self * other<Vec3>())
            .def(const_self * other<Vec4>())
            .def("transpose", &Matrix::transpose)
            .def("row", (void(Matrix::*)(long, const Vec4&))&Matrix::row),
          class_<MatrixRotX, Matrix>("MatrixRotX")
            .def(constructor<float>()),
          class_<MatrixRotY, Matrix>("MatrixRotY")
            .def(constructor<float>()),
          class_<MatrixRotZ, Matrix>("MatrixRotZ")
            .def(constructor<float>()),
          class_<MatrixTranslation, Matrix>("MatrixTranslation")
            .def(constructor<const lost::math::Vec3&>()),
          class_<MatrixScaling, Matrix>("MatrixScaling")
            .def(constructor<const lost::math::Vec3&>()),
          class_<MatrixLookAt, Matrix>("MatrixLookAt")
            .def(constructor<const lost::math::Vec3&, const lost::math::Vec3&, const lost::math::Vec3&>())
        ]
      ];
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
      globals(state)["lost"]["math"]["EPSILON"] = FLT_EPSILON;
#else
      globals(state)["lost"]["math"]["EPSILON"] = DBL_EPSILON;
#endif
    }

  }
}
