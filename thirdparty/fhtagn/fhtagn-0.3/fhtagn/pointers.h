/**
 * $Id: pointers.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_POINTERS_H
#define FHTAGN_POINTERS_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

namespace fhtagn {
namespace pointers {

/**
 * The pointer and const_pointer structs simplify writing template code that
 * should work both with pointers and const pointers. It avoids hardcoding the
 * type of pointer to use:
 *
 * template <typename T, template <typename> class pointerT>
 * void foo(pointerT<T>::type bar)
 * {
 *    bar->doSomething();
 * }
 *
 * int const * blargh = ...
 * foo<int, fhtagn::pointers::const_pointer>(blargh);
 *
 * You probably don't need this sort of thing unless you're writing nifty
 * template code like the pointer policies below...
 **/
template <typename T>
struct pointer
{
    typedef T * type;
};

template <typename T>
struct const_pointer
{
    typedef T const * type;
};

template <typename T>
struct const_pointer_const
{
    typedef T const * const type;
};

/**
 * Pointer policies solve a common problem, not by providing particularily
 * clever or complex code, but by giving it a name. There's very little magic
 * here...
 *
 * When handing any sort of object to a class's constructor, and that class
 * keeps a reference or pointer to that object, you always face the same
 * problems:
 *
 *  1) Who owns the object? (that's the obvious problem)
 *  2) What are the semantics for this object, if the enclosing class gets
 *     copy constructed or assigned?
 *
 * Pointer policies answer the second question, by giving different approaches
 * a different name. Some answers to the second question imply how the first
 * question is answered.
 *
 * It's still up to you to decide which approach to use.
 **/
template <
    typename dataT,
    template <typename> class pointerT = const_pointer
>
struct shallow_copy
{
    typename pointerT<dataT>::type m_pointer;

    shallow_copy(typename pointerT<dataT>::type pointer)
        : m_pointer(pointer)
    {
    }


    shallow_copy(shallow_copy const & other)
        : m_pointer(other.m_pointer)
    {
    }


    shallow_copy & operator=(shallow_copy const & other)
    {
        m_pointer = other.m_pointer;
        return *this;
    }


    typename pointerT<dataT>::type operator->() const
    {
        return m_pointer;
    }
};


template <
    typename dataT,
    template <typename> class pointerT = const_pointer
>
struct deep_copy
{
    typename pointerT<dataT>::type m_pointer;

    /**
     * Must distinguish between copy-constructed and non-copy-constructed
     * instances of deep_copy - copy-constructed instances must take ownership
     * of m_pointer, because they are allocated within one of the deep_copy
     * functions.
     **/
    bool m_owner;

    deep_copy(typename pointerT<dataT>::type pointer)
        : m_pointer(pointer)
        , m_owner(false)
    {
    }


    deep_copy(deep_copy const & other)
        : m_pointer(new dataT(*other.m_pointer))
        , m_owner(true)
    {
    }


    ~deep_copy()
    {
        if (m_owner) {
            delete m_pointer;
        }
    }


    deep_copy & operator=(deep_copy const & other)
    {
        if (m_owner) {
            delete m_pointer;
        }
        m_pointer = new dataT(*other.m_pointer);
        m_owner = true;
        return *this;
    }


    typename pointerT<dataT>::type operator->() const
    {
        return m_pointer;
    }
};



}} // namespace fhtagn::pointers

#endif // guard
