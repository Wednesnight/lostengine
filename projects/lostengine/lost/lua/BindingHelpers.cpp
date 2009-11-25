#include "lost/resource/DefaultLoader.h"
#include "lost/platform/shared_ptr.h"
#include "lost/application/AccelerometerEvent.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/Tasklet.h"
#include "lost/application/TouchEvent.h"
#include "lost/gl/Context.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/camera/Camera.h"
#include "lost/common/Color.h"
#include "lost/event/Event.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/gl/RenderBuffer.h"
#include "lost/gl/Texture.h"
#include "lost/lsystem/LSystem.h"
#include "lost/lua/State.h"
#include "lost/math/Rect.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"
#include "lost/resource/File.h"
#include "lost/resource/Loader.h"

#define GET_POINTER(name) template name* get_pointer(lost::shared_ptr<name>&);\
                          template name const* get_pointer(lost::shared_ptr<name const>&);

// this block is essential for correct luabind handling of lost::shared_ptr
namespace luabind
{
  template<class T>
  T* get_pointer(lost::shared_ptr<T>& p)
  {
    return p.get();
  }
  
  GET_POINTER(lost::application::AccelerometerEvent);
  GET_POINTER(lost::application::Application);
  GET_POINTER(lost::application::ApplicationEvent);
  GET_POINTER(lost::application::KeyEvent);
  GET_POINTER(lost::application::MouseEvent);
  GET_POINTER(lost::application::ResizeEvent);
  GET_POINTER(lost::application::Tasklet);
  GET_POINTER(lost::application::TouchEvent);
  GET_POINTER(lost::application::TouchEvent::Touch);
  GET_POINTER(lost::bitmap::Bitmap);
  GET_POINTER(lost::camera::Camera);
  GET_POINTER(lost::common::Color);
  GET_POINTER(lost::event::Event);
  GET_POINTER(lost::gl::Context);
  GET_POINTER(lost::gl::FrameBuffer);
  GET_POINTER(lost::gl::RenderBuffer);
  GET_POINTER(lost::gl::Texture);
  GET_POINTER(lost::lsystem::LSystem);
  GET_POINTER(lost::lsystem::LSystemState);
  GET_POINTER(lost::lua::State);
  GET_POINTER(lost::math::Rect);
  GET_POINTER(lost::math::Vec2);
  GET_POINTER(lost::math::Vec3);
  GET_POINTER(lost::math::Vec4);
  GET_POINTER(lost::resource::File);
  GET_POINTER(lost::resource::Loader);
}