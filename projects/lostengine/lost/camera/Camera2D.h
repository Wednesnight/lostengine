#ifndef LOST_CAMERA_CAMERA2D_H
#define LOST_CAMERA_CAMERA2D_H

namespace boost
{
  template<class T> class shared_ptr;
}

namespace lost
{
  namespace common
  {
    struct DisplayAttributes;
  }
}

namespace lost
{
  namespace camera
  {

    struct Camera2D
    {
      Camera2D(boost::shared_ptr<lost::common::DisplayAttributes> inDisplayAttributes);
      ~Camera2D();
    };

  }
}

#endif
