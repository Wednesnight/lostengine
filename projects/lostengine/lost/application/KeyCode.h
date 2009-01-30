#ifndef LOST_APPLICATION_KEYCODE_H
#define LOST_APPLICATION_KEYCODE_H

namespace lost
{
  namespace application
  {

    enum KeyCode
    {
      K_UNKNOWN = 0,

      K_BACKSPACE = 8,
      K_TAB = 9,
      K_ENTER     = 13,
      K_PAUSE     = 19,
      K_ESCAPE    = 27,
      K_SPACE     = 32,

      K_LEFT = 37,
      K_UP, K_RIGHT, K_DOWN,

      K_0 = 48,
      K_1, K_2, K_3, K_4, K_5, K_6, K_7,
      K_8, K_9,

      K_A = 65,
      K_B, K_C, K_D, K_E, K_F, K_G, K_H,
      K_I, K_J, K_K, K_L, K_M, K_N, K_O,
      K_P, K_Q, K_R, K_S, K_T, K_U, K_V,
      K_W, K_X, K_Y, K_Z,

      K_NUMPAD_0 = 96,
      K_NUMPAD_1, K_NUMPAD_2, K_NUMPAD_3, K_NUMPAD_4,
      K_NUMPAD_5, K_NUMPAD_6, K_NUMPAD_7, K_NUMPAD_8,
      K_NUMPAD_9,

      K_NUMPAD_PLUS  = 107,
      K_NUMPAD_MINUS = 109,

      K_F1 = 112,
      K_F2, K_F3, K_F4, K_F5, K_F6, K_F7,
      K_F8, K_F9, K_F10, K_F11, K_F12,
      K_DELETE		= 127,

      K_INSERT		= 277,

      K_HOME		= 278,
      K_END		= 279,
      K_PAGEUP		= 280,
      K_PAGEDOWN		= 281,

      K_RSHIFT		= 303,
      K_LSHIFT		= 304,
      K_RCTRL		= 305,
      K_LCTRL		= 306
    };

  }
}

#endif
