/**
 * $Id: numeric.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_DETAIL_NUMERIC_RESTRICTIONS_H
#define FHTAGN_DETAIL_NUMERIC_RESTRICTIONS_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

namespace fhtagn {
namespace restrictions {
namespace numeric {

/**
 * Ensures that a numeric value lies within a range of other values of the same
 * type. MIN and MAX themselves are valid values of the range.
 **/
template <
    typename valueT,
    valueT   MIN,
    valueT   MAX,
    typename next_restrictionT = none<valueT>
>
struct range
{
    static inline valueT const & check(valueT const & value)
    {
        boost::function_requires<concepts::RestrictionConcept<valueT, next_restrictionT> >();

        if (value < MIN || MAX < value) {
            throw violation_error("fhtagn::restrictions::range failed!");
        }
        return next_restrictionT::check(value);
    }
};


/**
 * Ensures that a numeric value is divisible withour remainder by a specified
 * (long) integer value.
 **/
template <
    typename valueT,
    long DIVISOR,
    typename next_restrictionT = none<valueT>
>
struct divisible
{
    static inline valueT const & check(valueT const & value)
    {
        boost::function_requires<concepts::RestrictionConcept<valueT, next_restrictionT> >();

        if (value % DIVISOR) {
            throw violation_error("fhtagn::restrictions::divisible failed!");
        }
        return next_restrictionT::check(value);
    }
};

/**
 * The even policy checks that the passed value is even, i.e. divisible by 2
 * without remainder. This makes use of the divisible restriction above.
 **/
template <
    typename valueT,
    typename next_restrictionT = none<valueT>
>
struct even
{
    static inline valueT const & check(valueT const & value)
    {
        boost::function_requires<concepts::RestrictionConcept<valueT, next_restrictionT> >();

        try {
            divisible<valueT, 2>::check(value);
        } catch (violation_error const &) {
            // catch exception to transform it to something more specific
            throw violation_error("fhtagn::restrictions::even failed!");
        }
        return next_restrictionT::check(value);
    }
};


/**
 * The odd policy checks that the passed value is odd, i.e. divisible by 2
 * with a remainder. This makes use of the divisible restriction above.
 **/
template <
    typename valueT,
    typename next_restrictionT = none<valueT>
>
struct odd
{
    static inline valueT const & check(valueT const & value)
    {
        boost::function_requires<concepts::RestrictionConcept<valueT, next_restrictionT> >();

        try {
            divisible<valueT, 2>::check(value);
            // if divisible does not throw, we have an even number
            throw violation_error("fhtagn::restrictions::odd failed!");
        } catch (violation_error const & ex) {
            // ignore errors by divisible
        }
        return next_restrictionT::check(value);
    }
};


/**
 * The equal policy checks that a valueT is equal to another valueT.
 **/
template <
    typename valueT,
    valueT OTHER,
    typename next_restrictionT = none<valueT>
>
struct equal
{
    static inline valueT const & check(valueT const & value)
    {
        boost::function_requires<concepts::RestrictionConcept<valueT, next_restrictionT> >();

        if (value != OTHER) {
            throw violation_error("fhtagn::restrictions::equal failed!");
        }
        return next_restrictionT::check(value);
    }
};


/**
 * The non_equal policy checks that a valueT is equal to another valueT.
 **/
template <
    typename valueT,
    valueT OTHER,
    typename next_restrictionT = none<valueT>
>
struct non_equal
{
    static inline valueT const & check(valueT const & value)
    {
        boost::function_requires<concepts::RestrictionConcept<valueT, next_restrictionT> >();

        if (value == OTHER) {
            throw violation_error("fhtagn::restrictions::non_equal failed!");
        }
        return next_restrictionT::check(value);
    }
};


/**
 * The default_value policy is a refinement of equal, in that it checks that a
 * value is equal to the value constructed via the type's default constructor.
 * For numeric values this is usually zero.
 **/
template <
    typename valueT,
    typename next_restrictionT = none<valueT>
>
struct default_value
{
    static inline valueT const & check(valueT const & value)
    {
        boost::function_requires<concepts::RestrictionConcept<valueT, next_restrictionT> >();

        if (value != valueT()) {
            throw violation_error("fhtagn::restrictions::default_value failed!");
        }
        return next_restrictionT::check(value);
    }
};


/**
 * The non_default_value policy is a refinement of equal, in that it checks that
 * a value is not equal to the value constructed via the type's default
 * constructor. For numeric values this is usually zero.
 *
 * You can use this restriction to check for non-zero pointers as well.
 **/
template <
    typename valueT,
    typename next_restrictionT = none<valueT>
>
struct non_default_value
{
    static inline valueT const & check(valueT const & value)
    {
        boost::function_requires<concepts::RestrictionConcept<valueT, next_restrictionT> >();

        if (value == valueT()) {
            throw violation_error("fhtagn::restrictions::non_default_value failed!");
        }
        return next_restrictionT::check(value);
    }
};


/**
 * Check whether the value is positive. The check relies on the assumption that
 * a default-constructed valueT is semantically equivalent to zero, which is the
 * case for all built in numeric types.
 **/
template <
    typename valueT,
    typename next_restrictionT = none<valueT>
>
struct positive
{
    static inline valueT const & check(valueT const & value)
    {
        boost::function_requires<concepts::RestrictionConcept<valueT, next_restrictionT> >();

        if (value <= valueT()) {
            throw violation_error("fhtagn::restrictions::positive failed!");
        }
        return next_restrictionT::check(value);
    }
};


/**
 * Check whether the value is negative. The check relies on the assumption that
 * a default-constructed valueT is semantically equivalent to zero, which is the
 * case for all built in numeric types.
 **/
template <
    typename valueT,
    typename next_restrictionT = none<valueT>
>
struct negative
{
    static inline valueT const & check(valueT const & value)
    {
        boost::function_requires<concepts::RestrictionConcept<valueT, next_restrictionT> >();

        if (value >= valueT()) {
            throw violation_error("fhtagn::restrictions::negative failed!");
        }
        return next_restrictionT::check(value);
    }
};


/**
 * Combines the default_value restriction with the positive restriction such
 * that either must be satisfied to satisfy null_or_positive.
 **/
template <
    typename valueT,
    typename next_restrictionT = none<valueT>
>
struct null_or_positive
{
    static inline valueT const & check(valueT const & value)
    {
        boost::function_requires<concepts::RestrictionConcept<valueT, next_restrictionT> >();
        return next_restrictionT::check(one_of<
                valueT,
                default_value<valueT>,
                positive<valueT>
            >::check(value));
    }
};


/**
 * Combines the default_value restriction with the negative restriction such
 * that either must be satisfied to satisfy null_or_negative.
 **/
template <
    typename valueT,
    typename next_restrictionT = none<valueT>
>
struct null_or_negative
{
    static inline valueT const & check(valueT const & value)
    {
        boost::function_requires<concepts::RestrictionConcept<valueT, next_restrictionT> >();
        return next_restrictionT::check(one_of<valueT,
                default_value<valueT>,
                negative<valueT>
            >::check(value));
    }
};


}}} // namespace fthagn::restrictions::numeric

#endif // guard
