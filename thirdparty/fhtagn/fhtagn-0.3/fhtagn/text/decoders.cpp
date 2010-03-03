/**
 * $Id: decoders.cpp 319 2009-06-08 22:17:47Z unwesen $
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
#include <fhtagn/text/transcoding.h>
#include <fhtagn/text/detail/universal.h>


namespace fhtagn {
namespace text {


universal_decoder::universal_decoder(char_encoding_type encoding /* = ASCII */)
    : m_encoding(encoding)
    , m_decoder(0)
{
    set_encoding(encoding);
}


universal_decoder::~universal_decoder()
{
    FHTAGN_TEXT_DESTROY(m_encoding, m_decoder, decoder);
}



void
universal_decoder::set_encoding(char_encoding_type new_encoding)
{
    FHTAGN_TEXT_DESTROY(m_encoding, m_decoder, decoder);
    m_encoding = new_encoding;
    FHTAGN_TEXT_CREATE(m_encoding, m_decoder, decoder);
}



void
universal_decoder::reset()
{
    FHTAGN_TEXT_CALL(m_encoding, m_decoder, decoder, reset, ());
}


bool
universal_decoder::append(unsigned char byte)
{
    FHTAGN_TEXT_CALL(m_encoding, m_decoder, decoder, append, (byte));
    return false;
}


bool
universal_decoder::have_full_sequence() const
{
    FHTAGN_TEXT_CALL(m_encoding, m_decoder, decoder, have_full_sequence, ());
    return false;
}


utf32_char_t
universal_decoder::to_utf32() const
{
    FHTAGN_TEXT_CALL(m_encoding, m_decoder, decoder, to_utf32, ());
    return utf32_char_t();
}


bool
universal_decoder::use_replacement_char() const
{
    FHTAGN_TEXT_CALL(m_encoding, m_decoder, decoder, use_replacement_char, ());
    return false;
}


void
universal_decoder::use_replacement_char(bool new_value)
{
    FHTAGN_TEXT_CALL(m_encoding, m_decoder, decoder, use_replacement_char,
            (new_value));
}


utf32_char_t
universal_decoder::replacement_char() const
{
    FHTAGN_TEXT_CALL(m_encoding, m_decoder, decoder, replacement_char, ());
    return utf32_char_t();
}


void
universal_decoder::replacement_char(utf32_char_t new_value)
{
    FHTAGN_TEXT_CALL(m_encoding, m_decoder, decoder, replacement_char,
            (new_value));
}



}} // namespace fhtagn::text
