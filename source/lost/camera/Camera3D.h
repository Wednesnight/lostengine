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

#ifndef LOST_CAMERA_CAMERA3D_H
#define LOST_CAMERA_CAMERA3D_H

#include "lost/camera/Camera.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Matrix.h"

namespace lost
{
  namespace camera
  {
    struct Camera3D;
    typedef lost::shared_ptr<Camera3D> Camera3DPtr;

    struct Camera3D : public Camera
    {
    private:
      math::Vec3 mPosition;
      math::Vec3 mDirection;
      math::Vec3 mRotation;
      float      mFovY;
      bool       mStickToTarget;

    protected:
      void update();

    public:
      // constructor
      Camera3D(const lost::math::Rect& inViewport);
      static Camera3DPtr create(const lost::math::Rect& inViewport);
      // getters
      float fovY();
      math::Vec3 position();
      math::Vec3 direction();
      math::Vec3 rotation();
      bool stickToTarget();
      math::Vec3 target();
      math::Vec3 up();

      // setters
      void plane(const math::Vec3& inPlane);
      void fovY(const float inFovY);
      void position(const math::Vec3& newPosition);
      void direction(const math::Vec3& newDirection);
      void stickToTarget(const bool newStickToTarget);
      void rotation(const math::Vec3& newRotation);
      void target(const math::Vec3& newTarget);

      // methods
      void move(const math::Vec3& deltaMove);
      void rotate(const math::Vec3& deltaRotate);
    };

  }
}

#endif
