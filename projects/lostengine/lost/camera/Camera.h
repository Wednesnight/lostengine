#ifndef LOST_CAMERA_CAMERA_H
#define LOST_CAMERA_CAMERA_H

#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Matrix.h"

namespace lost
{
  namespace camera
  {

    static const math::Vec3 positionBase(0.0f, 0.0f, 0.0f);
    static const math::Vec3 directionBase(0.0f, 0.0f, -1.0f);
    static const math::Vec3 rotationBase(0.0f, 0.0f, 0.0f);
    static const float      fovYBase(120.0f);
    static const math::Vec2 depthBase(0.1f, 100.0f);
    static const math::Vec3 upBase(0.0f, 1.0f, 0.0f);

    struct Camera
    {
    private:
      math::Vec3 mPosition;
      math::Vec3 mDirection;
      math::Vec3 mRotation;
      float      mFovY;
      math::Vec2 mDepth;
    public:
      Camera()
      : mPosition(positionBase),
        mDirection(directionBase),
        mRotation(rotationBase),
        mFovY(fovYBase),
        mDepth(depthBase)
      {
      }

      math::Vec2& depth()
      {
        return mDepth;
      }

      float& fovY()
      {
        return mFovY;
      }

      math::Vec3& position()
      {
        return mPosition;
      }

      math::Vec3 direction()
      {
        math::MatrixRotX rotX(mRotation.x);
        math::MatrixRotY rotY(mRotation.y);
        math::MatrixRotZ rotZ(mRotation.z);
        return rotZ * rotY * rotX * mDirection;
      }

      math::Vec3& rotation()
      {
        return mRotation;
      }

      math::Vec3 target()
      {
        return mPosition + direction();
      }

      math::Vec3 up()
      {
        math::MatrixRotX rotX(mRotation.x);
        math::MatrixRotY rotY(mRotation.y);
        math::MatrixRotZ rotZ(mRotation.z);
        return rotZ * rotY * rotX * upBase;
      }

      void fovY(const float newFovY)
      {
        mFovY = newFovY;
      }

      void depth(const math::Vec2& newDepth)
      {
        mDepth = newDepth;
      }

      void position(const math::Vec3& newPosition)
      {
        mPosition = newPosition;
      }

      void direction(const math::Vec3& newDirection)
      {
        mDirection = newDirection;
      }

      void rotation(const math::Vec3& newRotation)
      {
        mRotation = newRotation;
      }

      void target(const math::Vec3& newTarget)
      {
        mDirection = newTarget - mPosition;
      }

      void move(const math::Vec3& deltaMove)
      {
        math::MatrixRotX        rotX(mRotation.x);
        math::MatrixRotY        rotY(mRotation.y);
        math::MatrixRotZ        rotZ(mRotation.z);
        math::Vec3              newDelta(rotZ * rotY * rotX * deltaMove);
        math::MatrixTranslation translationMatrix(newDelta);
        position(translationMatrix * mPosition);
      }

      void rotate(const math::Vec3& deltaRotate)
      {
        math::Vec3 newRotation = mRotation + deltaRotate;
        if (newRotation.x < 0) newRotation.x += 360;
          else if (newRotation.x > 360) newRotation.x -= 360;
        if (newRotation.y < 0) newRotation.y += 360;
          else if (newRotation.y > 360) newRotation.y -= 360;
        if (newRotation.z < 0) newRotation.z += 360;
          else if (newRotation.z > 360) newRotation.z -= 360;
        mRotation = newRotation;
      }
    };

  }
}

#endif
