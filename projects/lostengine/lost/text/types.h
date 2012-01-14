#ifndef LOST_TEXT_TYPES_H
#define LOST_TEXT_TYPES_H

namespace lost {
  namespace text {   
    typedef u8                              utf8_char;
    typedef lost::basic_string<utf8_char>   utf8_string;

    typedef u16                             utf16_char;
    typedef lost::basic_string<utf16_char>  utf16_string;

    typedef char32_t                        utf32_char;
    typedef lost::basic_string<utf32_char>  utf32_string;
  }
}

#endif