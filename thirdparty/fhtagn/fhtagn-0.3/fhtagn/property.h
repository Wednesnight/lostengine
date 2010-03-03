/**
 * $Id: property.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_PROPERTY_H
#define FHTAGN_PROPERTY_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <functional>

#include <boost/concept_check.hpp>

namespace fhtagn {

namespace concepts {

/**
 * The ReadPropertyTagConcept and WritePropertyTagConcept below are used to
 * provide compile-time checks on the property class further down in this file.
 **/
template <typename T>
struct ReadPropertyTagConcept
{
    void constraints()
    {
        T::read();
    }
};

template <typename T>
struct WritePropertyTagConcept
{
    void constraints()
    {
        T::write();
    }
};

} // namespace concepts

/** Conforms to the ReadPropertyTagConcept and the WritePropertyTagConcept */
struct read_write_property
{
    static inline void read() {}
    static inline void write() {}
};

/** Conforms to the ReadPropertyTagConcept */
struct read_only_property
{
    static inline void read() {}
};

/** Conforms to the WritePropertyTagConcept */
struct write_only_property
{
    static inline void write() {}
};




/**
 * Many programming languages support class properties - that is, class members
 * that behave like regular data members, but internally delegate to a getter
 * and setter method to retrieve/modify values.
 *
 * From a readability point of view, this certainly beats using getters and
 * setters manually, even though internally the code isn't vastly different.
 *
 * Using interceptable from <fhtagn/interceptable.h>, this class provides such a
 * convenience wrapper for creating class properties.
 *
 * The property template class is parametrized using two parameters: the first
 * is the regular data type to simulate, and the second the type of the class
 * holding the property:
 *
 *    struct X
 *    {
 *        property<int, X>    my_property;
 *
 *        X::X();
 *        int get() const;
 *        void set(int const &);
 *    };
 *
 * There is a third template parameter to the property class specifying whether
 * the property is a read-only, write-only or read-write property (the default
 * makes it a read-write property). If you attempt to assign a value to a
 * read-only property, or in fact if you attempt to pass a setter to the
 * constructor of a read-only property, the compilation will fail.
 *
 * The types you should use to define what access you want to allow on your
 * property are fhtagn::read_only_property, fhtagn::write_only_property and
 * fhtagn::read_write_property.
 *
 * The property class provides three constructors. One accepts a member function
 * pointer to a setter function, one accepts a member function pointer to a
 * getter function, and one accepts both. Using these constructors works only in
 * combination with the correct property access tag, i.e. a read_only_property
 * will only compile when you use the constructor that accepts a getter, but no
 * setter, etc.
 *
 * All constructors also require a pointer to parent_classT, in order to be able
 * to delegate to the correct parent object for access:
 *
 *    X::X()
 *        : my_property(this, &X::get, &X::set)
 *    {
 *    }
 *
 * Properties should be used just as a public data member of a class would be -
 * the only difference is that reading and writing the property is handled by
 * wrapped functions. To this end, the property class defines many operators.
 *
 *    X x;
 *    int y = x.my_property + 42; // works just as expected.
 *
 * The operators provided can be roughly divided into two classes:
 *  - Operators that modify the property value, such as assignment operators
 *  - Operators that return the property value, without modifications
 *
 * Explicitly not provided are operators that read the property value, modify
 * it, and return a temporary with the result, such as (for example) a simple
 * addition operator, because it makes no sense to do so for a property.
 *
 * Note: The property class can also be used to implement classes with e.g.
 *       read-only or write-only value semantics:
 *
 *          template <
 *            typename reprT
 *          >
 *          struct read_only
 *            : public property<
 *                reprT,
 *                read_only<reprT>,
 *                read_only_property
 *              >
 *          {
 *            inline read_only(reprT const & initial_value)
 *              : property<reprT, read_only<reprT>, read_only_property>(this,
 *                  &read_only<reprT>::get)
 *              , m_value(initial_value)
 *            {
 *            }
 *
 *            inline reprT get() const
 *            {
 *              return m_value;
 *            }
 *
 *            reprT m_value;
 *          };
 *
 *       A class that does just that can be found in threads/future.h
 **/
template <
    /** The type to "simulate" in the property */
    typename reprT,
    /** Parent class holding the property */
    typename parent_classT,
    /** Define the available access methods on the property */
    typename property_access_tagT = read_write_property
>
class property
{
public:
    /** convenience */
    typedef property<reprT, parent_classT, property_access_tagT> own_type;

    /**
     * std::mem_fun types are used, because they relieve us of also
     * holding a reference to the actual instance of holderT
     * whose property we are describing.
     **/
    typedef std::const_mem_fun_t<reprT, parent_classT>            getter_type;
    typedef std::mem_fun1_t<void, parent_classT, reprT const &>   setter_type;

    /**
     * On the other hand, the *_param_type typedefs allow us to use
     * construct a property class using simple pointer-to-member-functions
     * instead of mem_fun-type objects.
     **/
    typedef reprT (parent_classT::*getter_param_type)() const;
    typedef void (parent_classT::*setter_param_type)(reprT const &);


    /** Constructor for read-only properties */
    property(parent_classT * parent, getter_param_type getter)
        : m_parent(parent)
        , m_getter(getter)
        , m_setter(0)
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
    }


    /** Constructor for write-only properties */
    property(parent_classT * parent, setter_param_type setter)
        : m_parent(parent)
        , m_getter(0)
        , m_setter(setter)
    {
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
    }


    /** Constructor for read-write properties */
    property(parent_classT * parent, getter_param_type getter,
            setter_param_type setter)
        : m_parent(parent)
        , m_getter(getter)
        , m_setter(setter)
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
    }


    /**
     * Because properties require their parent pointer, they are not implicitly
     * copyable - you need to copy them explicitly in a copy ctor.
     **/
    property(parent_classT * parent, own_type const & other)
        : m_parent(parent)
        , m_getter(other.m_getter)
        , m_setter(other.m_setter)
    {
        // since this is copy-constructed from another property of exactly the
        // same type, and this other property must have been constructed in some
        // way, it's safe to assume that own_type satisfies the requirements
        // of the ReadProperty and WriteProperty concepts
    }


    /** Assignment operators */
    template <typename otherT>
    own_type & operator=(otherT const & other)
    {
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
        m_setter(m_parent, other);
        return *this;
    }

    template <typename otherT>
    own_type & operator*=(otherT const & other)
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
        reprT tmp = m_getter(m_parent);
        tmp *= other;
        m_setter(m_parent, tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator/=(otherT const & other)
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
        reprT tmp = m_getter(m_parent);
        tmp /= other;
        m_setter(m_parent, tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator%=(otherT const & other)
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
        reprT tmp = m_getter(m_parent);
        tmp %= other;
        m_setter(m_parent, tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator+=(otherT const & other)
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
        reprT tmp = m_getter(m_parent);
        tmp += other;
        m_setter(m_parent, tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator-=(otherT const & other)
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
        reprT tmp = m_getter(m_parent);
        tmp -= other;
        m_setter(m_parent, tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator<<=(otherT const & other)
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
        reprT tmp = m_getter(m_parent);
        tmp <<= other;
        m_setter(m_parent, tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator>>=(otherT const & other)
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
        reprT tmp = m_getter(m_parent);
        tmp >>= other;
        m_setter(m_parent, tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator&=(otherT const & other)
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
        reprT tmp = m_getter(m_parent);
        tmp &= other;
        m_setter(m_parent, tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator^=(otherT const & other)
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
        reprT tmp = m_getter(m_parent);
        tmp ^= other;
        m_setter(m_parent, tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator|=(otherT const & other)
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
        reprT tmp = m_getter(m_parent);
        tmp |= other;
        m_setter(m_parent, tmp);
        return *this;
    }

    /** Arithmetic operators */

    // prefix operator ++
    own_type & operator++()
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
        reprT tmp = m_getter(m_parent);
        ++tmp;
        m_setter(m_parent, tmp);
        return *this;
    }

    // prefix operator --
    own_type & operator--()
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        boost::function_requires<concepts::WritePropertyTagConcept<property_access_tagT> >();
        reprT tmp = m_getter(m_parent);
        --tmp;
        m_setter(m_parent, tmp);
        return *this;
    }

    /** Subscript operator */
    template <typename retT, typename otherT>
    retT & operator[](otherT const & other)
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        reprT & tmp = m_getter(m_parent);
        return tmp[other];
    }

    template <typename retT, typename otherT>
    retT const & operator[](otherT const & other) const
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        reprT const & tmp = m_getter(m_parent);
        return tmp[other];
    }

    /** Casting operator for the wrapped type. */
    operator reprT() const
    {
        boost::function_requires<concepts::ReadPropertyTagConcept<property_access_tagT> >();
        return m_getter(m_parent);
    }

private:
    /** Cannot be default-constructed */
    property();

    /**
     * Cannot be assigned. It makes no sense to assign one property to another
     * directly, as the semantics for assigning m_parent, m_getter and m_setter
     * are odd at best. Assinging the /value/ of one property to another property
     * works just fine by (implicitly) using the cast operator to reprT and the
     * assignment operator for any otherT that isn't a property.
     **/
    property & operator=(property const & other);

    /** Holds pointer to parent class */
    parent_classT * m_parent;

    /** Holds getter function/functor */
    getter_type m_getter;

    /** Holds setter function/functor */
    setter_type m_setter;
};


} // namespace fhtagn

#endif // guard
