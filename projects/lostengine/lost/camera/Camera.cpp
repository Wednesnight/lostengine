#include "lost/camera/Camera.h"

namespace lost
{
  namespace camera
  {
    
    const math::Vec3 positionBase(0.0f, 0.0f, 0.0f);
    const math::Vec3 directionBase(0.0f, 0.0f, -1.0f);
    const math::Vec3 rotationBase(0.0f, 0.0f, 0.0f);
    const float      fovYBase(120.0f);
    const math::Vec2 depthBase(0.1f, 100.0f);
    const math::Vec3 upBase(0.0f, 1.0f, 0.0f);
    
    Camera::Camera()
    : mPosition(positionBase),
      mDirection(directionBase),
      mRotation(rotationBase),
      mFovY(fovYBase),
      mDepth(depthBase)
    {
    }
    
    math::Vec2& Camera::depth()
    {
      return mDepth;
    }
    
    float& Camera::fovY()
    {
      return mFovY;
    }
    
    math::Vec3& Camera::position()
    {
      return mPosition;
    }
    
    math::Vec3 Camera::direction()
    {
      math::MatrixRotX rotX(mRotation.x);
      math::MatrixRotY rotY(mRotation.y);
      math::MatrixRotZ rotZ(mRotation.z);
      return rotZ * rotY * rotX * mDirection;
    }
    
    math::Vec3& Camera::rotation()
    {
      return mRotation;
    }
    
    math::Vec3 Camera::target()
    {
      return mPosition + direction();
    }
    
    math::Vec3 Camera::up()
    {
      math::MatrixRotX rotX(mRotation.x);
      math::MatrixRotY rotY(mRotation.y);
      math::MatrixRotZ rotZ(mRotation.z);
      return rotZ * rotY * rotX * upBase;
    }
    
    void Camera::fovY(const float newFovY)
    {
      mFovY = newFovY;
    }
    
    void Camera::depth(const math::Vec2& newDepth)
    {
      mDepth = newDepth;
    }
    
    void Camera::position(const math::Vec3& newPosition)
    {
      mPosition = newPosition;
    }
    
    void Camera::direction(const math::Vec3& newDirection)
    {
      mDirection = newDirection;
    }
    
    void Camera::rotation(const math::Vec3& newRotation)
    {
      mRotation = newRotation;
    }
    
    void Camera::target(const math::Vec3& newTarget)
    {
      mDirection = newTarget - mPosition;
    }
    
    void Camera::move(const math::Vec3& deltaMove)
    {
      math::MatrixRotX        rotX(mRotation.x);
      math::MatrixRotY        rotY(mRotation.y);
      math::MatrixRotZ        rotZ(mRotation.z);
      math::Vec3              newDelta(rotZ * rotY * rotX * deltaMove);
      math::MatrixTranslation translationMatrix(newDelta);
      position(translationMatrix * mPosition);
    }
    
    void Camera::rotate(const math::Vec3& deltaRotate)
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
    
  }
}
