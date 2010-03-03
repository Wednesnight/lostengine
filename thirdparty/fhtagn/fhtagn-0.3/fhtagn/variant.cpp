/**
 * $Id: variant.cpp 319 2009-06-08 22:17:47Z unwesen $
 *
 * This file is part of the Fhtagn! C++ Library.
 * Copyright (C) 2009 Jens Finkhaeuser <unwesen@users.sourceforge.net>.
 *
 * Author: Henning Pfeiffer <slashgod@users.sourceforge.net>
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

#include <fhtagn/variant.h>

namespace fhtagn {

fhtagn::shared_ptr<variant> variant::invalid_value;


void
variant::instanciate_invalid_value()
{
    if (!invalid_value) {
        invalid_value = fhtagn::shared_ptr<variant>(new variant(IS_INVALID));
    }
}


variant::variant(variant_state state)
    : m_state(state)
    , m_data(0)
{
    // only used for null value, so don't instanciate again.
}


variant::variant()
    : m_state(IS_EMPTY)
    , m_data(0)
{
    instanciate_invalid_value();
}


variant::variant(variant const & other)
    : m_state(other.m_state)
    , m_data(other.m_data ? other.m_data->clone() : 0)
{
    instanciate_invalid_value();
}


variant::~variant()
{
    delete m_data;
}


variant &
variant::operator=(variant const & other)
{
    // never ever change the state of the invalid value.
    if (m_state == IS_INVALID) {
      throw error("Cannot assign to the variant::invalid_value!");
    }

    if (this != &other) {
        m_state = other.m_state;
        delete m_data;
        m_data = other.m_data ? other.m_data->clone() : 0;
    }
    return *this;
}


variant &
variant::operator[](fhtagn::size_t index)
{
    switch (m_state) {
        case IS_INVALID:
        case IS_EMPTY:
            // Delay the reaction to this error by handing out the invalid value.
            // Since we also go to this branch if the [] operator is called on
            // the invalid value, only the final result of a chain of [] will
            // throw an error.
            return *invalid_value.get();
            break;

        case IS_VALUE:
            if (!is<array_t>() || index >= as<array_t>().size()) {
              // we can add a simple check for the validity of the index -
              // not because we want to fundamentally change how array_t
              // like containers are provided by the STL, but because we
              // can again delay reaction to erroneous input.
              return *invalid_value.get();
            }
            return as<array_t>()[index];
            break;

        default:
            assert(0);
    }
    return *invalid_value.get();
}


variant const &
variant::operator[](fhtagn::size_t index) const
{
    switch (m_state) {
        case IS_INVALID:
        case IS_EMPTY:
            // Delay the reaction to this error ...
            return *invalid_value.get();
            break;

        case IS_VALUE:
            if (!is<array_t>() || index >= as<array_t>().size()) {
              return *invalid_value.get();
            }
            return as<array_t>()[index];
            break;

        default:
            assert(0);
    }
    return *invalid_value.get();
}



variant &
variant::operator[](std::string const & key)
{
    switch (m_state) {
        case IS_INVALID:
            // Delay reaction to this error...
            return *invalid_value.get();
            break;

        case IS_EMPTY:
            // In this particular case, let's deviate from the theme of
            // returning the invalid value - the reason is that on
            // non-const maps the expected behaviour is to silently add
            // new kv-pairs. On const maps, that can't work...
            m_data = new data<map_t>();
            m_state = IS_VALUE;
            // fall through

        case IS_VALUE:
            if (is<map_t>()) {
                return as<map_t>()[key];
            }
            return *invalid_value.get();
            break;

        default:
            assert(0);
    }
    return *invalid_value.get();
}



variant const &
variant::operator[](std::string const & key) const
{
    switch (m_state) {
        case IS_INVALID:
        case IS_EMPTY:
            // Delay reaction...
            return *invalid_value.get();
            break;

        case IS_VALUE:
            if (is<map_t>()) {
                map_t::const_iterator iter = as<map_t>().find(key);
                if (iter == as<map_t>().end()) {
                  return *invalid_value.get();
                }
                return iter->second;
            }
            return *invalid_value.get();

        default:
            assert(0);
    }
    return *invalid_value.get();
}


bool
variant::operator==(variant const & other) const
{
    if (!m_data || !other.m_data) {
        throw FHTAGN_VARIANT_COMPARISON_ERROR;
    }
    return (*m_data == *other.m_data);
}


bool
variant::operator<(variant const & other) const
{
    if (!m_data || !other.m_data) {
        throw FHTAGN_VARIANT_COMPARISON_ERROR;
    }
    return (*m_data < *other.m_data);
}


bool
variant::operator<=(variant const & other) const
{
    if (!m_data || !other.m_data) {
        throw FHTAGN_VARIANT_COMPARISON_ERROR;
    }
    return (*m_data <= *other.m_data);
}


bool
variant::operator>(variant const & other) const
{
    if (!m_data || !other.m_data) {
        throw FHTAGN_VARIANT_COMPARISON_ERROR;
    }
    return (*m_data > *other.m_data);
}


bool
variant::operator>=(variant const & other) const
{
    if (!m_data || !other.m_data) {
        throw FHTAGN_VARIANT_COMPARISON_ERROR;
    }
    return (*m_data >= *other.m_data);
}


bool
variant::operator!=(variant const & other) const
{
    if (!m_data || !other.m_data) {
        throw FHTAGN_VARIANT_COMPARISON_ERROR;
    }
    return (*m_data != *other.m_data);
}



} // namespace fhtagn
