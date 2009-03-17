#include "lost/camera/Camera3D.h"
#include "lost/lgl/lglu.h"

namespace lost
{
  namespace camera
  {
    
    const math::Vec3 positionBase(0.0f, 0.0f, 0.0f);
    const math::Vec3 directionBase(0.0f, 0.0f, -1.0f);
    const math::Vec3 rotationBase(0.0f, 0.0f, 0.0f);
    const math::Vec3 upBase(0.0f, 1.0f, 0.0f);
    
    Camera3D::Camera3D(const boost::shared_ptr<gl::Context>& inContext, const lost::math::Rect& inViewport)
    : Camera::Camera(inContext, inViewport),
      mPosition(positionBase),
      mDirection(directionBase),
      mRotation(rotationBase),
      mPlane(inViewport.width, inViewport.height, 100.0f),
      mFovY(120.0f),
      mStickToTarget(false)
    {
      DOUT("Camera3D::Camera3D()");
      plane(mPlane);
    }
    
    void Camera3D::plane(const math::Vec3& inPlane)
    {
      mPlane = inPlane;
      double radFovY = tan((double)mPlane.z / (double)(mPlane.y / 2));
      mFovY = math::rad2deg(radFovY);
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

    math::Vec3& Camera3D::rotation()
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
      return rotZ * rotY * rotX * upBase;
    }
    
    void Camera3D::position(const math::Vec3& newPosition)
    {
      mPosition = newPosition;
    }
    
    void Camera3D::direction(const math::Vec3& newDirection)
    {
      mDirection = newDirection;
    }

    void Camera3D::stickToTarget(const bool newStickToTarget)
    {
      mStickToTarget = newStickToTarget;
    }

    void Camera3D::rotation(const math::Vec3& newRotation)
    {
      mRotation = newRotation;
    }
    
    void Camera3D::target(const math::Vec3& newTarget)
    {
      mDirection = newTarget - mPosition;
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
    }

    void Camera3D::apply()
    {
      Camera::apply();

      double aspectRatio = (double)mPlane.x / (double)mPlane.y;
      math::Vec3 currentEye = position();
      math::Vec3 currentTarget = target();
      math::Vec3 currentUp = up();

      lgluPerspective(mFovY, aspectRatio, 0.1f, mPlane.z);
      lgluLookAt(currentEye.x,    currentEye.y,    currentEye.z,
                 currentTarget.x, currentTarget.y, currentTarget.z,
                 currentUp.x,     currentUp.y,     currentUp.z);
    }

  }
}
