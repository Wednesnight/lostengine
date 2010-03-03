/**
 * $Id: mandatory.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MANDATORY_H
#define FHTAGN_MANDATORY_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <stdexcept>

#include <boost/noncopyable.hpp>

namespace fhtagn {

/**
 * An implementation of Andrei Alexandrescu's http://www.ddj.com/dept/cpp/184401917
 *
 * The throw_if_unchecked class template can be used to wrap around return
 * types of functions that you want the caller to examine:
 *
 *    throw_if_unchecked<int> foo() {...}
 *
 *    foo();                                    // throws
 *    throw_if_unchecked<int> ret = foo();      // throws
 *    int ret = foo();                          // doesn't throw, value is 'used'
 *    static_cast<ignore_return_value>(foo());  // doesn't throw, discarded
 *    ignore_return_value x = foo();            // doesn't throw, discarded
 *
 * The error thrown in the cases marked above are std::logic_error.
 **/

struct ignore_return_value {};

template <
    // the wrapped type
    typename wrappedT
>
class throw_if_unchecked
{
public:
    inline throw_if_unchecked(wrappedT const & retval)
        : m_throw(true)
        , m_retval(retval)
    {
    }

    inline throw_if_unchecked(throw_if_unchecked const & other)
        : m_throw(other.m_throw)
        , m_retval(other.m_retval)
    {
    }

    inline ~throw_if_unchecked()
    {
        if (m_throw) {
            throw std::logic_error("Ignored return value that must not be ignored!");
        }
    }

    inline operator wrappedT() const
    {
        m_throw = false;
        return m_retval;
    }

    inline operator ignore_return_value() const
    {
        m_throw = false;
        return ignore_return_value();
    }

private:
    // No default construction
    throw_if_unchecked();

    mutable bool m_throw;
    wrappedT     m_retval;
};


} // namespace fhtagn

#endif // guard
