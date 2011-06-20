#ifndef LOST_APPLICATION_MOUSEBUTTON_H
#define LOST_APPLICATION_MOUSEBUTTON_H

// windows consts get in our way here...
#undef MB_RIGHT

namespace lost
{
  namespace application
  {

    enum MouseButton
    {
      MB_UNKNOWN = 0,

      MB_LEFT   = 1,
      MB_MIDDLE = 2,
      MB_RIGHT  = 4
    };

  }
}

#endif
