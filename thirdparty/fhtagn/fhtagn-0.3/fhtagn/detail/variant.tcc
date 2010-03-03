/**
 * $Id: variant.tcc 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_VARIANT_TCC
#define FHTAGN_VARIANT_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <assert.h>

/**
 * This particular error/error message is used in many places, so defined just
 * once.
 **/
#define FHTAGN_VARIANT_COMPARISON_ERROR \
    variant::error("fhtagn::variant: cannot compare to a value type other " \
            "than the one held in the variant.")

namespace fhtagn {

/**
 * The compare_throw comparison functor always throws.
 **/
template <typename T>
inline bool
variant::compare_throw<T>::operator()(T const & first, T const & second) const
{
    throw FHTAGN_VARIANT_COMPARISON_ERROR;
    return false;
}



/**
 * The data_base and data<T> structs fulfil pretty much the same purpose as
 * boost::any's nested classes, and in fact look remarkably similar ;)
 **/
struct variant::data_base
{
    virtual ~data_base()
    {
    }

    virtual std::type_info const & type() const = 0;
    virtual data_base * clone() const = 0;

    /**
     * The comparison operators need to be defined in data<T> as well, where
     * after a comparison of other.type() to our own held type, we can decide
     * whether it's safe to upcast data_base to data<T> or not.
     **/
    virtual bool operator==(data_base const & other) const = 0;
    virtual bool operator<(data_base const & other) const = 0;
    virtual bool operator<=(data_base const & other) const = 0;
    virtual bool operator>(data_base const & other) const = 0;
    virtual bool operator>=(data_base const & other) const = 0;
    virtual bool operator!=(data_base const & other) const = 0;
};

/**
 * By providing type-parameterized versions of type() and clone() to data_base,
 * we can determine whether the contents of two variants are of the same type
 * at run-time. We can also copy a variant's contents without knowing it's type.
 **/
template <typename valueT>
struct variant::data
    : public variant::data_base
{
    typedef typename specialization_traits<valueT>::holder_type holder_type;

    data()
        : m_value()
    {
    }

    data(holder_type const & val)
        : m_value(val)
    {
    }

    virtual ~data()
    {
    }

    virtual std::type_info const & type() const
    {
      return typeid(holder_type);
    }

    virtual data_base * clone() const
    {
      return new data<valueT>(m_value);
    }


    /**
     * Comparison operators
     **/
    virtual bool operator==(data_base const & other) const
    {
        if (other.type() != typeid(holder_type)) {
            throw FHTAGN_VARIANT_COMPARISON_ERROR;
        }
        return typename specialization_traits<holder_type>::equal_to()(m_value,
                reinterpret_cast<data<valueT> const *>(&other)->m_value);
    }


    virtual bool operator<(data_base const & other) const
    {
        if (other.type() != typeid(holder_type)) {
            throw FHTAGN_VARIANT_COMPARISON_ERROR;
        }
        return typename specialization_traits<holder_type>::less()(m_value,
                reinterpret_cast<data<valueT> const *>(&other)->m_value);
    }


    virtual bool operator<=(data_base const & other) const
    {
        if (other.type() != typeid(holder_type)) {
            throw FHTAGN_VARIANT_COMPARISON_ERROR;
        }
        return typename specialization_traits<holder_type>::less_equal()(m_value,
                reinterpret_cast<data<valueT> const *>(&other)->m_value);
    }


    virtual bool operator>(data_base const & other) const
    {
        if (other.type() != typeid(holder_type)) {
            throw FHTAGN_VARIANT_COMPARISON_ERROR;
        }
        return typename specialization_traits<holder_type>::greater()(m_value,
                reinterpret_cast<data<valueT> const *>(&other)->m_value);
    }


    virtual bool operator>=(data_base const & other) const
    {
        if (other.type() != typeid(holder_type)) {
            throw FHTAGN_VARIANT_COMPARISON_ERROR;
        }
        return typename specialization_traits<holder_type>::greater_equal()(m_value,
                reinterpret_cast<data<valueT> const *>(&other)->m_value);
    }


    virtual bool operator!=(data_base const & other) const
    {
        if (other.type() != typeid(holder_type)) {
            throw FHTAGN_VARIANT_COMPARISON_ERROR;
        }
        return typename specialization_traits<holder_type>::not_equal_to()(m_value,
                reinterpret_cast<data<valueT> const *>(&other)->m_value);
    }


    holder_type m_value;
};





template <typename T>
variant::variant(T const & other)
    : m_state(IS_VALUE)
    , m_data(0)
{
    instanciate_invalid_value();
    this->operator=(other);
}


template <typename T>
variant &
variant::operator=(T const & other)
{
    if (this == reinterpret_cast<variant const *>(&other)) {
        return *this;
    }

    switch (m_state) {
        case IS_INVALID:
            throw error("Cannot assign to the variant::invalid_value!");
            break;

        case IS_VALUE:
            delete m_data;
            // fall through
        case IS_EMPTY:
            // data<T> accepts a holder_type const & as it's parameter. If T and
            // the holder type are the same, that's great. If not, the coercion
            // is performed at this point.
            m_data = new data<T>(other);
            // we don't actually know if other is a value, but we hope it is.
            m_state = IS_VALUE;
            break;

        default:
            assert(0);
    }

    return *this;
}


template <typename T>
typename variant::specialization_traits<T>::holder_type &
variant::as()
{
    // We don't actually know if the reinterpret_cast is safe. It's left up to
    // the caller to precede calls to as() with calls to is().
    return reinterpret_cast<data<T> *>(m_data)->m_value;
}


template <typename T>
typename variant::specialization_traits<T>::holder_type const &
variant::as() const
{
    // We don't actually know if the reinterpret_cast is safe. It's left up to
    // the caller to precede calls to as() with calls to is().
    return reinterpret_cast<data<T> *>(m_data)->m_value;
}


template <typename T>
bool
variant::is() const
{
    return (m_data && typeid(T) == m_data->type());
}


inline bool
variant::is_valid() const
{
    return (m_state != IS_INVALID);
}


template <typename T>
inline bool
variant::check(variant const & var)
{
    try {
        return var.is<T>();
    } catch (error const &) {
        return false;
    }
}


inline bool
variant::exists(variant const & var)
{
    return var.is_valid();
}


template <typename T>
inline
typename variant::specialization_traits<T>::holder_type const &
variant::safe_get(variant const & var)
{
    if (!var.is<T>()) {
        throw variant::error("fhtagn::variant::safe_get: invalid type");
    }
    return var.as<T>();
}


template <typename T>
inline
typename variant::specialization_traits<T>::holder_type &
variant::safe_get(variant & var)
{
    if (!var.is<T>()) {
        throw variant::error("fhtagn::variant::safe_get: invalid type");
    }
    return var.as<T>();
}


template <typename T>
inline bool
variant::operator==(T const & other) const
{
    if (!m_data || m_data->type() != typeid(T)) {
        throw FHTAGN_VARIANT_COMPARISON_ERROR;
    }

    return typename specialization_traits<T>::equal_to()(reinterpret_cast<data<T> *>(m_data)->m_value,
        other);
}


template <typename T>
inline bool
variant::operator<(T const & other) const
{
    if (!m_data || m_data->type() != typeid(T)) {
        throw FHTAGN_VARIANT_COMPARISON_ERROR;
    }

    return typename specialization_traits<T>::less()(reinterpret_cast<data<T> *>(m_data)->m_value,
        other);
}


template <typename T>
inline bool
variant::operator<=(T const & other) const
{
    if (!m_data || m_data->type() != typeid(T)) {
        throw FHTAGN_VARIANT_COMPARISON_ERROR;
    }

    return typename specialization_traits<T>::less_equal()(reinterpret_cast<data<T> *>(m_data)->m_value,
        other);
}


template <typename T>
inline bool
variant::operator>(T const & other) const
{
    if (!m_data || m_data->type() != typeid(T)) {
        throw FHTAGN_VARIANT_COMPARISON_ERROR;
    }

    return typename specialization_traits<T>::greater()(reinterpret_cast<data<T> *>(m_data)->m_value,
        other);
}


template <typename T>
inline bool
variant::operator>=(T const & other) const
{
    if (!m_data || m_data->type() != typeid(T)) {
        throw FHTAGN_VARIANT_COMPARISON_ERROR;
    }

    return typename specialization_traits<T>::greater_equal()(reinterpret_cast<data<T> *>(m_data)->m_value,
        other);
}


template <typename T>
inline bool
variant::operator!=(T const & other) const
{
    if (!m_data || m_data->type() != typeid(T)) {
        throw FHTAGN_VARIANT_COMPARISON_ERROR;
    }

    return typename specialization_traits<T>::not_equal_to()(reinterpret_cast<data<T> *>(m_data)->m_value,
        other);
}


} // namespace fhtagn

// internal specialization for variant special types
FHTAGN_VARIANT_SPECIALIZE(fhtagn::variant::map_t)
FHTAGN_VARIANT_SPECIALIZE(fhtagn::variant::array_t)

#endif // guard
