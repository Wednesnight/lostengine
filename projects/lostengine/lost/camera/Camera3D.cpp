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

#include "lost/camera/Camera3D.h"

namespace lost
{
  namespace camera
  {
    
    Camera3D::Camera3D(const lost::math::Rect& inViewport)
    : Camera(inViewport),
      mPosition(0.0f, 0.0f, 0.0f),
      mDirection(0.0f, 0.0f, -1.0f),
      mRotation(0.0f, 0.0f, 0.0f),
      mFovY(45.0f),
      mStickToTarget(false)
    {
      mDepth = lost::math::Vec2(1.0f, 100.0f);
    }

    Camera3DPtr Camera3D::create(const lost::math::Rect& inViewport)
    {
      return Camera3DPtr(new Camera3D(inViewport));
    }

    math::Vec3 Camera3D::position()
    {
      math::Vec3 result = mPosition;
      if (stickToTarget())
      {
        math::MatrixRotX rotX(mRotation.x);
        math::MatrixRotY rotY(mRotation.y);
        math::MatrixRotZ rotZ(mRotation.z);
        result = target() + (rotZ * rotY * rotX * (mDirection * -1.0f));
      }
      return result;
    }
    
    math::Vec3 Camera3D::direction()
    {
      math::Vec3 result = mDirection;
      if (!stickToTarget())
      {
        math::MatrixRotX rotX(mRotation.x);
        math::MatrixRotY rotY(mRotation.y);
        math::MatrixRotZ rotZ(mRotation.z);
        result = rotZ * rotY * rotX * result;
      }
      return result;
    }

    bool Camera3D::stickToTarget()
    {
      return mStickToTarget;
    }

    math::Vec3 Camera3D::rotation()
    {
      return mRotation;
    }
    
    math::Vec3 Camera3D::target()
    {
      return mPosition + direction();
    }
    
    math::Vec3 Camera3D::up()
    {
      math::MatrixRotX rotX(mRotation.x);
      math::MatrixRotY rotY(mRotation.y);
      math::MatrixRotZ rotZ(mRotation.z);
      return rotZ * rotY * rotX * math::Vec3(0.0f, 1.0f, 0.0f);
    }
    
    float Camera3D::fovY()
    {
      return mFovY;
    }
    
    void Camera3D::position(const math::Vec3& newPosition)
    {
      mPosition = newPosition;
      needsUpdate = true;
    }
    
    void Camera3D::direction(const math::Vec3& newDirection)
    {
      mDirection = newDirection;
      needsUpdate = true;
    }

    void Camera3D::stickToTarget(const bool newStickToTarget)
    {
      mStickToTarget = newStickToTarget;
      needsUpdate = true;
    }

    void Camera3D::rotation(const math::Vec3& newRotation)
    {
      mRotation = newRotation;
      needsUpdate = true;
    }
    
    void Camera3D::target(const math::Vec3& newTarget)
    {
      mDirection = newTarget - mPosition;
      needsUpdate = true;
    }
    
    void Camera3D::move(const math::Vec3& deltaMove)
    {
      if (stickToTarget())
      {
        math::MatrixTranslation translationMatrix(math::Vec3(0.0f, 0.0f, deltaMove.z));
        math::Vec3 currentTarget(target());
        position(translationMatrix * mPosition);
        target(currentTarget);
      }
      else
      {
        math::MatrixRotX        rotX(mRotation.x);
        math::MatrixRotY        rotY(mRotation.y);
        math::MatrixRotZ        rotZ(mRotation.z);
        math::Vec3              newDelta(rotZ * rotY * rotX * deltaMove);
        math::MatrixTranslation translationMatrix(newDelta);
        position(translationMatrix * mPosition);
      }
      needsUpdate = true;
    }
    
    void Camera3D::fovY(const float inFovY)
    {
      mFovY = inFovY;
      needsUpdate = true;
    }
    
    void Camera3D::rotate(const math::Vec3& deltaRotate)
    {
      math::Vec3 newRotation = mRotation + deltaRotate;
      if (newRotation.x < 0) newRotation.x += 360;
      else if (newRotation.x > 360) newRotation.x -= 360;
      if (newRotation.y < 0) newRotation.y += 360;
      else if (newRotation.y > 360) newRotation.y -= 360;
      if (newRotation.z < 0) newRotation.z += 360;
      else if (newRotation.z > 360) newRotation.z -= 360;
      mRotation = newRotation;
      needsUpdate = true;
    }

    void Camera3D::update()
    {
      if (needsUpdate)
      {
        mViewMatrix = lost::math::MatrixLookAt(position(), target(), up());
        double aspectRatio = (double)mViewport.width / (double)mViewport.height;
        mProjectionMatrix = lost::math::MatrixPerspective(mFovY, (float)aspectRatio, mDepth);
        needsUpdate = false;
      }
    }

  }
}
