/**
 * $Id: restricted.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_RESTRICTED_H
#define FHTAGN_RESTRICTED_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <stdexcept>
#include <algorithm>

#include <boost/concept_check.hpp>

namespace fhtagn {

namespace concepts {

/**
 * RestrictionConcept enforces the interface of restrictions as used by the
 * restricted class template below.
 **/
template <
    typename valueT,
    typename restrictionT
>
struct RestrictionConcept
{
    void constraints()
    {
        valueT const & y = restrictionT::check(x);
        boost::ignore_unused_variable_warning(y);
    }

    valueT const x;
};

} // namespace concepts

namespace restrictions {

/**
 * In your own restrictions, please throw errors of this type when detecting a
 * violation of your restriction. That keeps the interface consistent.
 **/
typedef std::runtime_error violation_error;

/**
 * The 'none' restriction accepts only a single template parameter in order to
 * form the end of a restriction chain. Other restrictions should not do so.
 **/
template <typename T>
struct none
{
    static inline T const & check(T const & value)
    {
        return value;
    }
};


/**
 * The opposite of none<>, no value every passes the never<> restriction.
 **/
template <typename T>
struct never
{
    static inline T const & check(T const & value)
    {
        throw violation_error("No restriction was satisfied!");
        return value;
    }
};


/**
 * By writing restrictions such that their next restriction is invoked when the
 * passed value passes the current restriction, one effectively creates a chain
 * of ANDed restrictions - all of them must succeed.
 *
 * There may be situations in which ORing restrictions is more desirable, which
 * is what you can achieve using one_of<>. The template accepts up to 10
 * restrictions, which are checked in order, and if one of them is passed, the
 * whole chain is passed.
 *
 * The run-time equivalent would probably be that of a switch-statement. The
 * numeric restriction null_or_negative<> uses a one_of<> chain of
 * default_value<> and negative<>.
 */
template <
    typename valueT,
    typename restrict0T,
    typename restrict1T = never<valueT>,
    typename restrict2T = never<valueT>,
    typename restrict3T = never<valueT>,
    typename restrict4T = never<valueT>,
    typename restrict5T = never<valueT>,
    typename restrict6T = never<valueT>,
    typename restrict7T = never<valueT>,
    typename restrict8T = never<valueT>,
    typename restrict9T = never<valueT>
>
struct one_of
{
    static inline valueT const & check(valueT const & value)
    {
        boost::function_requires<concepts::RestrictionConcept<valueT, restrict0T> >();
        boost::function_requires<concepts::RestrictionConcept<valueT, restrict1T> >();
        boost::function_requires<concepts::RestrictionConcept<valueT, restrict2T> >();
        boost::function_requires<concepts::RestrictionConcept<valueT, restrict3T> >();
        boost::function_requires<concepts::RestrictionConcept<valueT, restrict4T> >();
        boost::function_requires<concepts::RestrictionConcept<valueT, restrict5T> >();
        boost::function_requires<concepts::RestrictionConcept<valueT, restrict6T> >();
        boost::function_requires<concepts::RestrictionConcept<valueT, restrict7T> >();
        boost::function_requires<concepts::RestrictionConcept<valueT, restrict8T> >();
        boost::function_requires<concepts::RestrictionConcept<valueT, restrict9T> >();

        try {
            return restrict0T::check(value);
        } catch (violation_error const &) {
            try {
                return restrict1T::check(value);
            } catch (violation_error const &) {
                try {
                    return restrict2T::check(value);
                } catch (violation_error const &) {
                    try {
                        return restrict3T::check(value);
                    } catch (violation_error const &) {
                        try {
                            return restrict4T::check(value);
                        } catch (violation_error const &) {
                            try {
                                return restrict5T::check(value);
                            } catch (violation_error const &) {
                                try {
                                    return restrict6T::check(value);
                                } catch (violation_error const &) {
                                    try {
                                        return restrict7T::check(value);
                                    } catch (violation_error const &) {
                                        try {
                                            return restrict8T::check(value);
                                        } catch (violation_error const &) {
                                            try {
                                                return restrict9T::check(value);
                                            } catch (violation_error const &) {
                                                return never<valueT>::check(value);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
};


} // namespace restrictions

/**
 * The restricted template class simulates a regular value type (as specified by
 * the wrappedT template parameter), but restricts the range of values you may
 * assign to it. If an invalid value is being assigned, an exception is thrown.
 *
 * The purpose of this class is to provide something similar to contract checks
 * in other languages, i.e. to enforce that the parameters to a function conform
 * to a certain pattern, as does the return value:
 *
 *    restricted<int, restrictions::range<int, 0, 20> >
 *    foo(restricted<std::string, restrictions::non_empty<std::string> > const & param)
 *    {
 *        // return 21; // would throw
 *        return 10; // doesn't throw
 *    }
 *
 *    foo(""); // would throw
 *
 * Of course, these type definitions are pretty ugly; it's probably best to
 * typedef them. See <fhtagn/restrictions.h> for some predefined restrictions,
 * but you can also roll your own.
 *
 * The actual restrictions imposed on the wrapped type aren't hardcoded into
 * restricted, but defined via a second template parameter, which defaults to
 * 'none' (i.e. no restrictions).
 *
 * Restrictions other than none should accept two template parameters, one being
 * the wrappedT to restrict, the other being a chained restriction type, which
 * should default to none.
 *
 * All restrictions must provide a static function check(wrappedT const &), which
 * throws when the passed wrappedT does not conform to whatever restrictions are
 * to be enforced. This interface is enforced by a concept check, and you should
 * use the same concept check in your user-defined restrictions.
 *
 * Predefined restrictions in this code all throw restrictions::violation_error,
 * and your own restrictions should do the same, for the sake of consistency.
 **/
template <
    /** The type to wrap */
    typename wrappedT,
    /** The restriction chain to apply */
    typename restrictionT = restrictions::none<wrappedT>
>
class restricted
{
public:
    /** Convenience */
    typedef restricted<wrappedT, restrictionT> own_type;

    /**
     * This constructor is the default ctor, and the only copy ctor restricted
     * allows - if you want to copy from one restricted to another, the cast
     * operator to wrappedT will be invoked automatically.
     **/
    restricted(wrappedT const & value = wrappedT())
        : m_value(restrictionT::check(value))
    {
        boost::function_requires<concepts::RestrictionConcept<wrappedT, restrictionT> >();
    }


    /**
     * An explicit copy constructor for restricted is required to make
     * restricted work in a return value specification. However, checking the
     * value passed in is unnecessary, as the 'other' restricted must already
     * have passed these checks during it's construction...
     **/
    restricted(restricted const & other)
        : m_value(other.m_value)
    {
    }

    /**
     * For similar reasons, and explicit assignment operator is a good idea.
     **/
    restricted & operator=(restricted const & other)
    {
        m_value = other.m_value;
        return *this;
    }


    /** Assignment operators */
    template <typename otherT>
    own_type & operator=(otherT const & other)
    {
        boost::function_requires<concepts::RestrictionConcept<wrappedT, restrictionT> >();
        m_value = restrictionT::check(other);
        return *this;
    }

    template <typename otherT>
    own_type & operator*=(otherT const & other)
    {
        boost::function_requires<concepts::RestrictionConcept<wrappedT, restrictionT> >();
        wrappedT tmp = m_value;
        tmp *= other;
        m_value = restrictionT::check(tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator/=(otherT const & other)
    {
        boost::function_requires<concepts::RestrictionConcept<wrappedT, restrictionT> >();
        wrappedT tmp = m_value;
        tmp /= other;
        m_value = restrictionT::check(tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator%=(otherT const & other)
    {
        boost::function_requires<concepts::RestrictionConcept<wrappedT, restrictionT> >();
        wrappedT tmp = m_value;
        tmp %= other;
        m_value = restrictionT::check(tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator+=(otherT const & other)
    {
        boost::function_requires<concepts::RestrictionConcept<wrappedT, restrictionT> >();
        wrappedT tmp = m_value;
        tmp += other;
        m_value = restrictionT::check(tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator-=(otherT const & other)
    {
        boost::function_requires<concepts::RestrictionConcept<wrappedT, restrictionT> >();
        wrappedT tmp = m_value;
        tmp -= other;
        m_value = restrictionT::check(tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator<<=(otherT const & other)
    {
        boost::function_requires<concepts::RestrictionConcept<wrappedT, restrictionT> >();
        wrappedT tmp = m_value;
        tmp <<= other;
        m_value = restrictionT::check(tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator>>=(otherT const & other)
    {
        boost::function_requires<concepts::RestrictionConcept<wrappedT, restrictionT> >();
        wrappedT tmp = m_value;
        tmp >>= other;
        m_value = restrictionT::check(tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator&=(otherT const & other)
    {
        boost::function_requires<concepts::RestrictionConcept<wrappedT, restrictionT> >();
        wrappedT tmp = m_value;
        tmp &= other;
        m_value = restrictionT::check(tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator^=(otherT const & other)
    {
        boost::function_requires<concepts::RestrictionConcept<wrappedT, restrictionT> >();
        wrappedT tmp = m_value;
        tmp ^= other;
        m_value = restrictionT::check(tmp);
        return *this;
    }

    template <typename otherT>
    own_type & operator|=(otherT const & other)
    {
        boost::function_requires<concepts::RestrictionConcept<wrappedT, restrictionT> >();
        wrappedT tmp = m_value;
        tmp |= other;
        m_value = restrictionT::check(tmp);
        return *this;
    }

    /** Arithmetic operators */

    // prefix operator ++
    own_type & operator++()
    {
        boost::function_requires<concepts::RestrictionConcept<wrappedT, restrictionT> >();
        wrappedT tmp = m_value;
        ++tmp;
        m_value = restrictionT::check(tmp);
        return *this;
    }

    // prefix operator --
    own_type & operator--()
    {
        boost::function_requires<concepts::RestrictionConcept<wrappedT, restrictionT> >();
        wrappedT tmp = m_value;
        --tmp;
        m_value = restrictionT::check(tmp);
        return *this;
    }


    /** Casting operator for the wrapped type. */
    operator wrappedT() const
    {
        return m_value;
    }

    /** Utility functions */
    void swap(own_type & other)
    {
        // Both this object and other must have passed restrictions to set their
        // m_value. And the restrictions are identical. So we can just swap
        // m_value.
        std::swap(m_value, other.m_value);
    }

private:
    /** wrapped value */
    wrappedT m_value;
};


} // namespace fhtagn


namespace std {

// Specialize std::swap for restricted types.
template <
    typename wrappedT,
    typename restrictionT
>
inline void swap(fhtagn::restricted<wrappedT, restrictionT> & first,
    fhtagn::restricted<wrappedT, restrictionT> & second)
{
  first.swap(second);
}

} // namespace std

#endif // guard
