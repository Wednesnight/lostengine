/**
 * $Id: encoders.cpp 319 2009-06-08 22:17:47Z unwesen $
 *
 * This file is part of the Fhtagn! C++ Library.
 * Copyright (C) 2009 Jens Finkhaeuser <unwesen@users.sourceforge.net>.
 *
 * Author: Jens Finkhaeuser <unwesen@users.sourceforge.net>.
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


universal_encoder::universal_encoder(char_encoding_type encoding /* = ASCII */)
    : m_encoding(encoding)
    , m_encoder(0)
{
    set_encoding(encoding);
}


universal_encoder::~universal_encoder()
{
    FHTAGN_TEXT_DESTROY(m_encoding, m_encoder, encoder);
}



void
universal_encoder::set_encoding(char_encoding_type new_encoding)
{
    FHTAGN_TEXT_DESTROY(m_encoding, m_encoder, encoder);
    m_encoding = new_encoding;
    FHTAGN_TEXT_CREATE(m_encoding, m_encoder, encoder);
}



universal_encoder::const_iterator
universal_encoder::begin() const
{
    FHTAGN_TEXT_CALL(m_encoding, m_encoder, encoder, begin, ());
    return const_iterator();
}


universal_encoder::const_iterator
universal_encoder::end() const
{
    FHTAGN_TEXT_CALL(m_encoding, m_encoder, encoder, end, ());
    return const_iterator();
}


bool
universal_encoder::encode(utf32_char_t ch)
{
    FHTAGN_TEXT_CALL(m_encoding, m_encoder, encoder, encode, (ch));
    return false;
}


bool
universal_encoder::use_replacement_char() const
{
    FHTAGN_TEXT_CALL(m_encoding, m_encoder, encoder, use_replacement_char, ());
    return false;
}


void
universal_encoder::use_replacement_char(bool new_value)
{
    FHTAGN_TEXT_CALL(m_encoding, m_encoder, encoder, use_replacement_char,
            (new_value));
}


utf32_char_t
universal_encoder::replacement_char() const
{
    FHTAGN_TEXT_CALL(m_encoding, m_encoder, encoder, replacement_char, ());
    return utf32_char_t();
}


void
universal_encoder::replacement_char(utf32_char_t new_value)
{
    FHTAGN_TEXT_CALL(m_encoding, m_encoder, encoder, replacement_char,
            (new_value));
}



}} // namespace fhtagn::text
