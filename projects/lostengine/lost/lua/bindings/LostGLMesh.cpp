#include "lost/lua/lua.h"
#include "lost/gl/Mesh.h"

#include "lost/lua/bindings/LostGLMesh.h"

using namespace boost;
using namespace luabind;
using namespace lost::gl;

namespace lost
{
  namespace lua
  {
    void LostGLMesh(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<Mesh2D, shared_ptr<Mesh2D> >("Mesh2D")
            .def(constructor<uint32_t, uint32_t, uint32_t, shared_ptr<Texture> >())
            .def("setVertexCount", &Mesh2D::setVertexCount)
            .def("setVertex", &Mesh2D::setVertex)
            .def("setTexcoordCount", &Mesh2D::setTexcoordCount)
            .def("setTexcoord", &Mesh2D::setTexcoord)
            .def("setFaceCount", &Mesh2D::setFaceCount)
            .def("setFace", &Mesh2D::setFace),

          class_<Mesh3D, shared_ptr<Mesh3D> >("Mesh3D")
            .def(constructor<uint32_t, uint32_t, uint32_t, shared_ptr<Texture> >())
            .def("setVertexCount", &Mesh3D::setVertexCount)
            .def("setVertex", &Mesh3D::setVertex)
            .def("setTexcoordCount", &Mesh3D::setTexcoordCount)
            .def("setTexcoord", &Mesh3D::setTexcoord)
            .def("setFaceCount", &Mesh3D::setFaceCount)
            .def("setFace", &Mesh3D::setFace)
        ]
      ];
    }
  }
}
