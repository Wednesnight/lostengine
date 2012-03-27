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

#include "lost/lua/bindings/LostCamera.h"
#include "lost/camera/Camera.h"
#include "lost/camera/Camera2D.h"
#include "lost/camera/Camera3D.h"

#include <slub/slub.h>

using namespace lost::camera;
using namespace lost::math;

namespace lost
{
  namespace lua
  {
    void LostCameraCamera(lua_State* state)
    {
      slub::package camera = slub::package(state, "lost").package("camera");

      camera.clazz<Camera>("Camera")
        .method("viewport", (Rect&(Camera::*)(const Rect&))&Camera::viewport)
        .method("viewport", (Rect&(Camera::*)())&Camera::viewport)
        .method("viewMatrix", &Camera::viewMatrix)
        .method("projectionMatrix", &Camera::projectionMatrix);

      camera.clazz<Camera2D>("Camera2D")
        .extends<Camera>()
        .function("create", &Camera2D::create);

      camera.clazz<Camera3D>("Camera3D")
        .extends<Camera>()
        .method("fovY", (float(Camera3D::*)()) &Camera3D::fovY)
        .method("fovY", (void(Camera3D::*)(const float)) &Camera3D::fovY)
        .method("position", (Vec3(Camera3D::*)()) &Camera3D::position)
        .method("position", (void(Camera3D::*)(const Vec3&)) &Camera3D::position)
        .method("direction", (Vec3(Camera3D::*)()) &Camera3D::direction)
        .method("direction", (void(Camera3D::*)(const Vec3&)) &Camera3D::direction)
        .method("stickToTarget", (bool(Camera3D::*)()) &Camera3D::stickToTarget)
        .method("stickToTarget", (void(Camera3D::*)(const bool)) &Camera3D::stickToTarget)
        .method("rotation", (Vec3(Camera3D::*)()) &Camera3D::rotation)
        .method("rotation", (void(Camera3D::*)(const Vec3&)) &Camera3D::rotation)
        .method("target", (Vec3(Camera3D::*)()) &Camera3D::target)
        .method("target", (void(Camera3D::*)(const Vec3&)) &Camera3D::target)
        .method("move", &Camera3D::move)
        .method("rotate", &Camera3D::rotate)
        .method("depth", (Vec2(Camera3D::*)()) &Camera3D::depth)
        .method("depth", (void(Camera3D::*)(const Vec2&)) &Camera3D::depth)
        .method("up", (Vec3(Camera3D::*)()) &Camera3D::up)
        .method("up", (void(Camera3D::*)(const Vec3&)) &Camera3D::up)
        .function("create", &Camera3D::create);
    }

    void LostCamera(lua_State* state)
    {
      LostCameraCamera(state);
    }

  }
}
