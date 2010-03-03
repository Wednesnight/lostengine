/**
 * $Id: encoders.h 319 2009-06-08 22:17:47Z unwesen $
 *
 * This file is part of the Fhtagn! C++ Library.
 * Copyright (C) 2009 Jens Finkhaeuser <unwesen@users.sourceforge.net>.
 *
 * Author: Jens Finkhaeuser <unwesen@users.sourceforge.net>
 *
 * This program is licensed as free software for personal, educational or
 * other non-commerical uses: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Alternatively, licenses for commercial purposes are available as well.
 * Please send your enquiries to the copyright holder's address above.
 **/
#ifndef FHTAGN_TEXT_ENCODERS_H
#define FHTAGN_TEXT_ENCODERS_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <fhtagn/text/transcoding.h>

/**
 * The character value 159 is not a valid character in ASCII or ISO-8859-*
 * encodings.
 **/
#define FHTAGN_TEXT_INVALID_CHAR char(159)

namespace fhtagn {
namespace text {

/**
 * Encodes UTF-32 values to characters, without checking on whether the UTF-32
 * is valid or not. The encoder produces a 1:1 mapping, so any UTF-32 value
 * above 255 is treated as non-representable. This encoder defaults to not
 * producing replacement characters.
 *
 * This encoder may or may not be useful - but it provides a counterpart to the
 * raw_decoder.
 **/
struct raw_encoder
    : public transcoder_base
{
    typedef char const * const_iterator;

    /**
     * When encoding unknown character, the default is skip these characters.
     **/
    raw_encoder()
        : transcoder_base(true, '\0')
        , m_flag(false)
        , m_byte(0)
    {
    }

    const_iterator begin() const
    {
        if (m_flag) {
            return &m_byte;
        }
        return end();
    }

    const_iterator end() const
    {
        return &m_byte + 1;
    }

    bool encode(utf32_char_t ch)
    {
        if (0 <= ch && ch <= 255) {
            m_byte = static_cast<char>(ch);
            m_flag = true;
        } else {
            m_flag = false;
        }
        return m_flag;
    }

    bool m_flag;
    char m_byte;
};


/**
 * Encodes a UTF-32 character into a single ASCII byte. ASCII allows only 7 bit
 * values, i.e. all values above 127 are considered invalid.
 **/
struct ascii_encoder
    : public transcoder_base
{
    typedef char const * const_iterator;

    /**
     * When encoding unknown character, the default is skip these characters.
     **/
    ascii_encoder()
        : transcoder_base(true, '\0')
    {
    }

    const_iterator begin() const
    {
        // If m_byte has a valid value, return m_byte's address. Else return
        // end()
        return &m_byte + (m_byte == FHTAGN_TEXT_INVALID_CHAR ? 1 : 0);
    }

    const_iterator end() const
    {
        return &m_byte + 1;
    }

    bool encode(utf32_char_t ch)
    {
        // Only characters with values <= 127 can be encoded in ASCII - higher
        // value characters require a different encoding. Note that this also
        // implies that the default replacement character 0xfffd cannot be
        // encoded in ASCII.
        if (0 <= ch && ch <= 127) {
            m_byte = static_cast<char>(ch);
            return true;
        }
        m_byte = FHTAGN_TEXT_INVALID_CHAR; // signal empty buffer
        return false;
    }

    char m_byte;
};



/**
 * Base encoder for ISO-8859 encoders. Note that the IANA versions of the
 * standard is implemented, i.e. control characters in the C0 and C1 ranges
 * are interpreted as well.
 **/
struct iso8859_encoder_base
    : public transcoder_base
{
    typedef char const * const_iterator;

    /**
     * When encoding unknown characters, the default is to skip these characters.
     **/
    explicit iso8859_encoder_base(fhtagn::size_t subencoding)
        : transcoder_base(true, '\0')
        , m_subencoding(subencoding)
        , m_byte(0)
        , m_valid(true)
    {
    }

    const_iterator begin() const
    {
        // If m_byte has a valid value, return m_byte's address. Else return
        // end()
        return (m_valid ? &m_byte : end());
    }

    const_iterator end() const
    {
        return &m_byte + 1;
    }

    bool encode(utf32_char_t ch)
    {
        m_valid = true;

        // Characters with values <= 127 can be encoded in just like in ASCII
        // Characters with values in the range [128, 159] are technically only
        // valid after the 1987/1988/1989 extensions to the ISO 8859 standard.
        if (0 <= ch && ch <= 159) {
            m_byte = static_cast<char>(ch);
            return true;
        }

        if (m_subencoding == 1) {
            // in iso-8859-1, all characters correspond to unicode code points
            // of the same value.
            if (160 <= ch && ch <= 255) {
                m_byte = static_cast<char>(ch);
                return true;
            }
        } else {
            // compute offset into mapping table
            fhtagn::size_t offset = m_subencoding - 2;
            if (m_subencoding > 11) {
                --offset;
            }
            offset *= 96; // number of characters special to each subencoding

            for (fhtagn::size_t i = offset ; i < offset + 96 ; ++i) {
                if (detail::iso8859_mapping[i] == ch) {
                    m_byte = static_cast<char>(i - offset + 160);
                    return true;
                }
            }
        }

        // other bytes can't be encoded in any of the iso8859 encodings.
        m_valid = false;
        return false;
    }

    fhtagn::size_t  m_subencoding;
    char            m_byte;
    bool            m_valid;
};


#define FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(subencoding)       \
    struct iso8859_##subencoding##_encoder                    \
        : public iso8859_encoder_base                         \
    {                                                         \
        iso8859_##subencoding##_encoder()                     \
            : iso8859_encoder_base(subencoding)               \
        {                                                     \
        }                                                     \
    };

FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(1)
FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(2)
FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(3)
FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(4)
FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(5)
FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(6)
FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(7)
FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(8)
FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(9)
FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(10)
FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(11)
FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(13)
FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(14)
FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(15)
FHTAGN_TEXT_DEFINE_ISO8859_ENCODER(16)


/**
 * Encoder for CP 1252. That windows encoding is basically identical to
 * ISO-8859-1, except that the unused range of bytes between 0x80 and 0x9f
 * is mapped to characters, some of which are included in ISO-8859-15.
 **/
struct cp1252_encoder
    : public transcoder_base
{
    typedef char const * const_iterator;

    /**
     * When encoding unknown characters, the default is to skip these characters.
     **/
    explicit cp1252_encoder()
        : transcoder_base(true, '\0')
        , m_flag(false)
        , m_byte(0)
    {
    }

    const_iterator begin() const
    {
        if (m_flag) {
            return &m_byte;
        }
        return end();
    }

    const_iterator end() const
    {
        return &m_byte + 1;
    }

    bool encode(utf32_char_t ch)
    {
        // Characters with certain value ranges can be encoded in just like in
        // ASCII
        if ((0 <= ch && ch <= 127) || (160 <= ch && ch <= 255)) {
            m_byte = static_cast<char>(ch);
            m_flag = true;
            return true;
        }

        for (boost::uint32_t i = 0 ; i < 32 ; ++i) {
            if (detail::cp1252_mapping[i] == ch) {
                m_byte = static_cast<char>(i + 0x80);
                m_flag = true;
                return true;
            }
        }

        // other bytes can't be encoded in any of the iso8859 encodings.
        m_flag = false;
        return false;
    }

    bool  m_flag;
    char  m_byte;
};



/**
 * Encoder for Mac Roman. This encoding covers all possible 8 bit values.
 **/
struct mac_roman_encoder
    : public transcoder_base
{
    typedef char const * const_iterator;

    /**
     * When encoding unknown characters, the default is to skip these characters.
     **/
    explicit mac_roman_encoder()
        : transcoder_base(true, '\0')
        , m_flag(false)
        , m_byte(0)
    {
    }

    const_iterator begin() const
    {
        if (m_flag) {
            return &m_byte;
        }
        return end();
    }

    const_iterator end() const
    {
        return &m_byte + 1;
    }

    bool encode(utf32_char_t ch)
    {
        if (0 <= ch && ch <= 127) {
            m_byte = static_cast<char>(ch);
            m_flag = true;
        }

        for (boost::uint32_t i = 0 ; i < 128 ; ++i) {
            if (detail::mac_roman_mapping[i] == ch) {
                m_byte = static_cast<char>(i + 0x80);
                m_flag = true;
                return true;
            }
        }

        // other bytes can't be encoded
        m_flag = false;
        return false;
    }

    bool  m_flag;
    char  m_byte;
};



/**
 * UTF-8 encoder
 **/
struct utf8_encoder
    : public transcoder_base
{
    typedef char const * const_iterator;

    utf8_encoder()
        : transcoder_base()
        , m_end(m_buffer)
    {
    }

    const_iterator begin() const
    {
        return m_buffer;
    }

    const_iterator end() const
    {
        return m_end;
    }

    bool encode(utf32_char_t ch)
    {
        static utf32_char_t const modifier_table[] = {
            0x00, 0xc0, 0xe0, 0xf0,
        };

        boost::uint8_t size = 0;
        if (ch < 0x80) {
          size = 1;
        } else if (ch < 0x800) {
          size = 2;
        } else if (ch < 0x10000) {
          size = 3;
        } else if (ch < 0x110000) {
          size = 4;
        } else {
          // Character is outside of the scope that UTF-8 may represent
          return false;
        }

        char * offset = &m_buffer[size];
        m_end = offset;
        switch (size) {
          // everything falls through
          case 4:
            *--offset = static_cast<char>((ch | 0x80) & 0xbf);
            ch >>= 6;
          case 3:
            *--offset = static_cast<char>((ch | 0x80) & 0xbf);
            ch >>= 6;
          case 2:
            *--offset = static_cast<char>((ch | 0x80) & 0xbf);
            ch >>= 6;
          case 1:
            *--offset = static_cast<char>(ch | modifier_table[size - 1]);
        }
        return true;
    }

    char    m_buffer[4];
    char *  m_end;
};



/**
 * Base class for UTF-16 encoders. In contrast to decoding, we can't autodetect
 * the desired endianness of the output by parsing a BOM. Instead, we use the
 * host byte order by default.
 **/
struct utf16_encoder
    : public transcoder_base
{
    typedef char const * const_iterator;

    explicit utf16_encoder(byte_order::endian endian
            = static_cast<byte_order::endian>(byte_order::FHTAGN_BYTE_ORDER))
        : transcoder_base()
        , m_endian(endian)
        , m_end(reinterpret_cast<char *>(m_buffer))
    {
    }

    const_iterator begin() const
    {
        return reinterpret_cast<const_iterator>(m_buffer);
    }

    const_iterator end() const
    {
        return m_end;
    }

    bool encode(utf32_char_t ch)
    {
        if (ch <= 0xffff) {
            // characters in the basic multilingual plane are encoded as a single
            // word; however, they may not be values in the range indicating
            // surrogate pairs.
            if (0xd800 < ch && ch <= 0xdfff) {
                return false;
            }

            m_buffer[0] = static_cast<utf16_char_t>(ch);
            m_buffer[0] = byte_order::from_host(m_buffer[0], m_endian);

            m_end = reinterpret_cast<char *>(&m_buffer[1]);
            return true;
        }

        // characters outside the basic multilingual plane are encoded in two
        // words, but may still not exceed the maximum legal value for UTF-32
        // characters.
        if (ch > 0x0010ffffUL) {
            return false;
        }

        ch -= 0x00010000UL;
        m_buffer[0] = static_cast<utf16_char_t>((ch >> 10) + 0xd800);
        m_buffer[0] = byte_order::from_host(m_buffer[0], m_endian);
        m_buffer[1] = static_cast<utf16_char_t>((ch & 0x03ff) + 0xdc00);
        m_buffer[1] = byte_order::from_host(m_buffer[1], m_endian);

        m_end = reinterpret_cast<char *>(&m_buffer[2]);
        return true;
    }

    byte_order::endian  m_endian;
    utf16_char_t        m_buffer[2];
    char *              m_end;
};


struct utf16le_encoder
    : public utf16_encoder
{
    utf16le_encoder()
        : utf16_encoder(byte_order::FHTAGN_LITTLE_ENDIAN)
    {
    }
};


struct utf16be_encoder
    : public utf16_encoder
{
    utf16be_encoder()
        : utf16_encoder(byte_order::FHTAGN_BIG_ENDIAN)
    {
    }
};


/**
 * Base class for UTF-32 encoders. In contrast to decoding, we can't autodetect
 * the desired endianness of the output by parsing a BOM. Instead, we use the
 * host byte order by default.
 **/
struct utf32_encoder
    : public transcoder_base
{
    typedef char const * const_iterator;

    explicit utf32_encoder(byte_order::endian endian
            = static_cast<byte_order::endian>(byte_order::FHTAGN_BYTE_ORDER))
        : transcoder_base()
        , m_endian(endian)
    {
    }

    const_iterator begin() const
    {
        return reinterpret_cast<const_iterator>(&m_buffer);
    }

    const_iterator end() const
    {
        return reinterpret_cast<const_iterator>(&m_buffer) + 4;
    }

    bool encode(utf32_char_t ch)
    {
        m_buffer = byte_order::from_host(ch, m_endian);
        return true;
    }

    byte_order::endian  m_endian;
    utf32_char_t        m_buffer;
};


struct utf32le_encoder
    : public utf32_encoder
{
    utf32le_encoder()
        : utf32_encoder(byte_order::FHTAGN_LITTLE_ENDIAN)
    {
    }
};


struct utf32be_encoder
    : public utf32_encoder
{
    utf32be_encoder()
        : utf32_encoder(byte_order::FHTAGN_BIG_ENDIAN)
    {
    }
};


/**
 * The universal_encoder follows the same Concept as other encoders, but does
 * not encode one specific encoding. Instead, you can switch the encoding it is
 * meant to encode at run-time. Internally, the universal_encoder delegates to
 * a pimpl'd encoder instance of the required type.
 **/
struct universal_encoder
{
    /** transcoder_base interface **/
    bool use_replacement_char() const;
    void use_replacement_char(bool new_value);

    utf32_char_t replacement_char() const;
    void replacement_char(utf32_char_t new_value);

    /** Standard encoder interface **/
    typedef char const * const_iterator;

    explicit universal_encoder(char_encoding_type encoding = ASCII);
    ~universal_encoder();

    const_iterator begin() const;
    const_iterator end() const;

    bool encode(utf32_char_t ch);

    /**
     * Return the encoding the decoder instance is currently set to decode.
     **/
    char_encoding_type get_encoding() const
    {
        return m_encoding;
    }

    /**
     * Set a new encoding to decoder. This will reset() the decoder.
     **/
    void set_encoding(char_encoding_type new_encoding);

private:
    char_encoding_type  m_encoding;
    void *              m_encoder;
};


}} // namespace fhtagn::text

#endif //guard
