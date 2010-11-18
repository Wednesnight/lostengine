#ifndef LOST_FONT_TEXTFORWARD_H
#define LOST_FONT_TEXTFORWARD_H

namespace fhtagn {
namespace text {

typedef char            utf8_char_t;
typedef std::basic_string<utf8_char_t>  utf8_string;
typedef uint16_t utf16_char_t;
typedef std::basic_string<utf16_char_t> utf16_string;
typedef uint32_t utf32_char_t;
typedef std::basic_string<utf32_char_t> utf32_string;

}
}

#endif