#include "lost/lua/bindings/LostMath.h"
#include "lost/lua/lua.h"
#include <luabind/operator.hpp>

#include "lost/math/Rect.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"
#include "lost/math/Matrix.h"

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
            .def(constructor<const Rect&>()) 
            .def(constructor<float, float, float, float>())
            .def("contains", &Rect::contains)
            .def("intersects", &Rect::intersects)
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

    void LostMathVec2(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("math")
        [
//        class_<Vec2, lost::shared_ptr<Vec2> >("Vec2")
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
            .def(self * float())
            .def(self * other<Vec3>())
            .def(self + other<Vec3>())
            .def(self - other<Vec3>())
            .def(self == other<Vec3>())
            .def(tostring(self)),

          def("len", (float(*)(const Vec3&))&len),
          def("squlen", (float(*)(const Vec3&))&squlen),
          def("normalise", (Vec3&(*)(Vec3&))&normalise),
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
          .def(const_self*Matrix()),
          class_<MatrixRotX, Matrix>("MatrixRotX")
            .def(constructor<float>()),
          class_<MatrixRotY, Matrix>("MatrixRotY")
            .def(constructor<float>()),
          class_<MatrixRotZ, Matrix>("MatrixRotZ")
            .def(constructor<float>()),
          class_<MatrixTranslation, Matrix>("MatrixTranslation")
            .def(constructor<const lost::math::Vec3&>())
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
    }

  }
}
