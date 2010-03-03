/**
 * $Id: variant.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_VARIANT_H
#define FHTAGN_VARIANT_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include <functional>

#include <boost/static_assert.hpp>

#include <fhtagn/shared_ptr.h>

/**
 * Call this macro to specialize the variant for use with the specified type.
 * Must be called in the global namespace.
 **/
#define FHTAGN_VARIANT_SPECIALIZE(_type)                                      \
    namespace fhtagn {                                                        \
        template <>                                                           \
        struct variant::is_specialized<_type>                                 \
        {                                                                     \
        };                                                                    \
    } // namespace

/**
 * Call this macro to specialize the variant for use with the specified
 * value type, but store values internally using the specified holder type.
 * Must be called in the global namespace.
 **/
#define FHTAGN_VARIANT_SPECIALIZE_HOLDER(_value_type, _holder_type)           \
    FHTAGN_VARIANT_SPECIALIZE(_value_type)                                    \
    namespace fhtagn {                                                        \
        template <>                                                           \
        struct variant::specialization_traits<_value_type>                    \
        {                                                                     \
            typedef _holder_type holder_type;                                 \
            /** specialize comparison functors as well **/                    \
            typedef comp_equal_to<holder_type>::type        equal_to;         \
            typedef comp_not_equal_to<holder_type>::type    not_equal_to;     \
            typedef comp_less<holder_type>::type            less;             \
            typedef comp_less_equal<holder_type>::type      less_equal;       \
            typedef comp_greater<holder_type>::type         greater;          \
            typedef comp_greater_equal<holder_type>::type   greater_equal;    \
        };                                                                    \
    } // namespace

/**
 * Call these macros for specializing the comparison functor to use when
 * comparing two variants holding the same value type. Note that the comparison
 * functor must be specialized on the holder type, not the value type itself.
 **/
#define FHTAGN_VARIANT_SPECIALIZE_COMPARE(_value_type, _operator, _functor)   \
    namespace fhtagn {                                                        \
        template <>                                                           \
        struct variant::comp_##_operator<_value_type>                         \
        {                                                                     \
            typedef _functor type;                                            \
        };                                                                    \
    } // namespace

/**
 * Safe version of fhtagn::variant::check - calls fhtagn::variant::check but
 * casts the variable passed as the second parameter to a const reference
 * to ensure that operator[] does not modify the variable at all.
 *   "unsafe": fhtagn::variant::check<int>(x["foo"])
 *   safe:     FHTAGN_VARIANT_CHECK(int, x, ["foo"])
 **/
#define FHTAGN_VARIANT_CHECK(_value_type, _variable, _index_chain)            \
    fhtagn::variant::check<_value_type>(                                      \
        const_cast<fhtagn::variant const &>(_variable)_index_chain)           \

/**
 * Safe version of fhtagn::variant::exists - calls fhtagn::variant::exists but
 * casts the variable passed as the second parameter to a const reference
 * to ensure that operator[] does not modify the variable at all.
 *   "unsafe": fhtagn::variant::exists(x["foo"])
 *   safe:     FHTAGN_VARIANT_EXISTS(x, ["foo"])
 **/
#define FHTAGN_VARIANT_EXISTS(_variable, _index_chain)                         \
    fhtagn::variant::exists(                                                  \
        const_cast<fhtagn::variant const &>(_variable)_index_chain)           \

/**
 * Safe version of fhtagn::variant::safe_get - calls fhtagn::variant::safe_get
 * but casts the variable passed as the second parameter to a const reference
 * to ensure that operator[] does not modify the variable at all.
 *   "unsafe": fhtagn::variant::safe_get<int>(x["foo"])
 *   safe:     FHTAGN_VARIANT_SAFE_GET(int, x, ["foo"])
 **/
#define FHTAGN_VARIANT_SAFE_GET(_value_type, _variable, _index_chain)         \
    fhtagn::variant::safe_get<_value_type>(                                   \
        const_cast<fhtagn::variant const &>(_variable)_index_chain)           \

namespace fhtagn {

/**
 * The variant class serves the purpose of holding a variety of values, the
 * exact data type of which are determined at run time. The concept is very
 * similar to "any" data types, and in fact variant used to use boost::any
 * internally.
 *
 * In contrast to "any", however, you can determine at compile time what data
 * types may be stored in the variant. Assigning a differently typed value to
 * the variant will not compile.
 *
 * Assume you specialized the variant class to store some unsigned integer
 * types:
 *    FHTAGN_VARIANT_SPECIALIZE(uint8_t);
 *    FHTAGN_VARIANT_SPECIALIZE(uint16_t);
 *    FHTAGN_VARIANT_SPECIALIZE(uint32_t);
 *    FHTAGN_VARIANT_SPECIALIZE(uint64_t);
 *
 * The following code would work under these circumstances:
 *    variant x = 12;
 *    uint16_t y = 123;
 *    variant z = y;
 *
 * Assigning a signed integer value or from a signed integer variable, or from
 * a float, would not work:
 *    variant x = -12; // compile error
 *    int16_t y = 123;
 *    variant z = y; // compile error
 *
 *    variant x = 1.0; // compile error
 *
 * These properties make the variant useful for all cases where an "any" data
 * type might provide more flexibility than wanted.
 *
 * Another use-case for a variant comes to mind, however, and that is to write
 * code more closely resembling what weakly-typed langauges, or strongly-typed
 * languages with a lot of automatic type conversion provide:
 *
 *    variant x = 12;
 *    x = std::string("Hello, world!");
 *
 * Both assignments compile just fine, as long as your variant is specialized both
 * for an integer type and std::string.
 *
 * Things become even neater, because this code also works:
 *
 *    variant x;
 *    x["foo"]["bar"] = 12;
 *    x["foo"] = 34;
 *
 * So what happens here?
 *
 * First you should know that in order to emulate the type system that scripting
 * languages commonly supply, a specialization of a variant that holds a map of
 * strings to variants always exist. Another such specialization is one that
 * emulates an array of variants). You can instanciate objects of these special
 * types using the variant::array_t and variant::map_t types respectively.
 *
 * In the above code snippet, we first create a variant named x, which we assign
 * no value. Variants that have never been assigned a value are special, in that
 * they can adopt any type (for which specializations exist) implicitly. It is
 * equally possible to assign such a variant a simle value, as it is to use them
 * as if they were a variant::map_t. More explictly, the example above should
 * have been started like this:
 *
 *    variant x = variant::map_t();
 *    x["foo"] = variant::map_t();
 *    x["foo"]["bar"] = 12;
 *
 * But why go to that effort if you can have it easier?
 *
 * Note that the same trick does not work for variant::array_t - the array_t is
 * modelled after the STL's Sequence concept (it is, in fact, a
 * std::vector<variant>), and thus new elements need to be appended or prepended
 * explicitly. This would work:
 *
 *    variant x = variant::array_t();
 *    x.as<variant::array_t>().push_back(12);
 *
 *    variant z = variant::array_t(1);
 *    z[0] = 12;
 *
 * So what's with that weird syntax above, this as() function call?
 *
 * Variants can't and won't change the fact that C++ is a strongly typed
 * language. There are more reasons to /want/ strongly typed languages (at least
 * in this author's opinion) than there is to want weakly typed languages. All
 * that variant does, therefore, is to save you some of the hassle of casting
 * back and forth between void * and the type you want to use.
 *
 * If you assign an integer value to a variant, the variant will remember that
 * you assigned it an integer value, and you need to extract that integer
 * explictly before you can modify it:
 *
 *    variant x = 12;
 *    x.as<int>() += 3;
 *    assert(x.as<int>() == 15);
 *
 * The as() function requires a template parameter for the type you expect to be
 * stored in the variant, casts it's internal void pointer to the type you
 * specify, and returns a reference to it.
 *
 * If you your variant did not contain an int, however, as() would return a
 * reference to an int at address 0x0 - you would very definitely encounter a
 * segmentation fault.
 *
 * It pays to be sure, therefore, what type your variant contains. You can find
 * that out prior to calling as() by calling is() - is() works in a nearly
 * identical fashion internally, but returns true if the specified type matched
 * the one stored in the variant, and false otherwise:
 *
 *    if (x.is<int>()) {
 *        x.as<int>() += 234; // perfectly safe code.
 *    }
 *
 * And that's how you would normally use a variant. The variant::map_t and
 * variant::array_t types are treated a little more specially, however. The
 * subscript operator for variant that accepts integer values assumes that
 * the held data type is a variant::array_t. Similarily, the subscript
 * operator that accepts strings, assumes that there is a variant::map_t.
 *
 * Because this sort of shortcut must be hardcoded, it's only provided for
 * variant::map_t and variant::array_t - that does not, however, stop you from
 * storing any other container in a variant, it just means that access will be
 * slightly more complex.
 *
 * This variant implementation provides another feature, which makes it
 * particularily useful for storing strings, although the feature can be used
 * for any other data types as well. It might be that you want to write code
 * such as this:
 *
 *    char * foo = "Hello, world!";
 *    variant x = foo;
 *
 * For the sake of the argument, we're assuming that you don't want to store
 * a char * inside the variant. More likely, you want to store the string
 * "Hello, world!" in the variant, that is, you want the variant to behave much
 * like a std::string.
 *
 * In order to try and avoid too many special cases (such as with variant::map_t
 * and variant::array_t), you can instead specialize the variant to use a
 * different data type for the internal holder variable than the data type
 * used in the assignment:
 *
 *    FHTAGN_VARIANT_SPECIALIZE_HOLDER(char *, std::string);
 *    x = foo;
 *    assert(x.as<std::string>() == "Hello, world");
 *
 * Note that all those specialization macros, i.e. FHTAGN_VARIANT_SPECIALIZE
 * and FHTAGN_VARIANT_SPECIALIZE_HOLDER provide compile time checks for
 * assignemnt. That is, if in a given piece of code you assign an integer value
 * to a variant, and somewhere previous to that line (through #includes or
 * directly), no specialization macro for integers appeared, the compile will
 * fail.
 *
 * While that may at first seem cumbersome, it's actually a great feature. It
 * allows you to use variant in several places in your code, and each time you
 * can decide anew which data types you want to support. Ideally, build yourself
 * an internal header file for each use case, and #include that wherever
 * appropriate. The included header file <fhtagn/stdvariant.h> may be a good
 * starting point.
 *
 * Note that if you use any value type that does not have comparison oerators
 * defined, you will either have to define those comparison operators, or
 * explicitly tell variant what functor to use for comparisons via the
 * FHTAGN_VARIANT_SPECIALIZE_COMPARE macro.
 **/
class variant
{
public:
    /**
     * See class documentation. Specialized map type that maps std::strings to
     * variants.
     **/
    typedef std::map<std::string, variant> map_t;

    /**
     * See class documentation. Specialized array of variants.
     **/
    typedef std::vector<variant> array_t;

    /**
     * Variants throw error on some occasion, which is derived from
     * std::logic_error.
     **/
    typedef std::logic_error error;

    variant();
    ~variant();

    /**
     * Variants are CopyConstructible and Assignable from other variants.
     **/
    variant(variant const & other);
    variant & operator=(variant const & other);

    /**
     * Variants are also CopyConstructible and Assignable from any other type -
     * at least in theory. In practice, these functions use
     * specialization_traits<T> internally (see below), which may not be defined
     * for any T. FHTAGN_VARIANT_SPECIALIZE* creates such definitions for you.
     **/
    template <typename T>
    variant(T const & other);
    template <typename T>
    variant & operator=(T const & other);

    /**
     * Part of the variant's specialization magic. If this structure is defined
     * for a valueT, assigning a valueT to the variant will compile... See
     * specialization_traits for details.
     **/
    template <typename valueT>
    struct is_specialized;

    /**
     * For each of the comparison operators, a typedef in a specialized struct
     * is declared, which is then used in specialization_traits. The reason is
     * that for most types, the predefined comparison functors from <functional>
     * are likely what you want to compare variant's contents with; however
     * some value types may not define all operators, and you may wish to
     * explicitly specialize which functor to use instead.
     **/
    template <typename valueT>
    struct comp_equal_to
    {
        typedef std::equal_to<valueT> type;
    };

    template <typename valueT>
    struct comp_not_equal_to
    {
        typedef std::not_equal_to<valueT> type;
    };

    template <typename valueT>
    struct comp_less
    {
        typedef std::less<valueT> type;
    };

    template <typename valueT>
    struct comp_less_equal
    {
        typedef std::less_equal<valueT> type;
    };

    template <typename valueT>
    struct comp_greater
    {
        typedef std::greater<valueT> type;
    };

    template <typename valueT>
    struct comp_greater_equal
    {
        typedef std::greater_equal<valueT> type;
    };


    /**
     * Comparison functor to use whenever you want a comparison to throw an
     * exception. That is likely to be the case whenever the corresponding
     * operator for your value type does not exist.
     **/
    template <typename T>
    struct compare_throw
    {
        bool operator()(T const & first, T const & second) const;
    };


    /**
     * Using a traits structure for valueT enables us to do two things:
     * 1. easily define at compile time, whether a different holder_type is to
     *    be used from the valueT specified. By default, holder_type and valueT
     *    are identical.
     * 2. Throw compile time assertions if is_specialized<valueT> is not defined.
     **/
    template <typename valueT>
    struct specialization_traits
    {
        typedef valueT holder_type;

        /**
         * Comparator typedefs - each of them can be specialized separately, if
         * desired.
         **/
        typedef typename comp_equal_to<valueT>::type        equal_to;
        typedef typename comp_not_equal_to<valueT>::type    not_equal_to;
        typedef typename comp_less<valueT>::type            less;
        typedef typename comp_less_equal<valueT>::type      less_equal;
        typedef typename comp_greater<valueT>::type         greater;
        typedef typename comp_greater_equal<valueT>::type   greater_equal;

        BOOST_STATIC_ASSERT(sizeof(is_specialized<valueT>) != 0);
    };


    /**
     * Return the held variable in the format (data type) defined by the
     * specialization_traits, as a reference.
     **/
    template <typename T>
    typename specialization_traits<T>::holder_type &
    as();

    /**
     * Return the held variable in the format (data type) defined by the
     * specialization_traits, as a const reference.
     **/
    template <typename T>
    typename specialization_traits<T>::holder_type const &
    as() const;

    /**
     * Returns true if this variant holds a value of type T, else false.
     **/
    template <typename T>
    bool is() const;

    /**
     * Variants may be invalid. Invalid variants can be returned from the
     * subscript operators, if the variant you called the subcript operator
     * on was not subscriptable. If you further use an invalid variant, that
     * will result in a variant::error  exception. Using this function, you
     * can explicitly check if a variant is invalid.
     **/
    bool is_valid() const;

    /**
     * Assume that the variant holds a map_t or an array_t, and try to access
     * the element specified by the index/key. If the variant does not hold a
     * container, or (depending on the container semantics) the element does
     * not exist, an invalid variant is returned.
     **/
    variant & operator[](fhtagn::size_t index);
    variant & operator[](std::string const & key);

    variant const & operator[](fhtagn::size_t index) const;
    variant const & operator[](std::string const & key) const;

    /**
     * Use this function to simplify checking of the type of a nested variant:
     *    variant::check<int>(x["foo"]["bar"])
     * returns true if x["foo"]["bar"] exists and contains an int.
     *
     * The code only works if (in the example above) x is a const variable or
     * holds a value other than a map. Because variant maps try to behave as
     * std::map does, and std::map is expected to silently add keys that don't
     * yet exists (and we want that, otherwise x["foo"] = 123 would NEVER work),
     * x may get converted to a map_t with a key "foo"...
     * In order to be really safe, use the FHTAGN_VARIANT_CHECK() macro above.
     **/
    template <typename T>
    static bool check(variant const & var);

    /**
     * Use this function to simplify checking for existence of a key/element in a nested variant:
     *    variant::exists(x["foo"][1]["bar"][2]))
     * returns true if x["foo"][1]["bar"][2] exists.
     *
     * See the above check() function for a reasoning why using the macro
     * FHTAGN_VARIANT_EXISTS() might be a better idea in a lot of cases.
     **/
    static bool exists(variant const & var);

    /**
     * The two static versions of safe_get() below can be used in much the same
     * way as check() above. They either return the specified type, or, if that's
     * not possible, throw an error.
     *
     * See the above check() function for a reasoning why using the macro
     * FHTAGN_VARIANT_SAFE_GET() might be a better idea in a lot of cases.
     **/
    template <typename T>
    static typename specialization_traits<T>::holder_type const &
    safe_get(variant const & var);

    template <typename T>
    static typename specialization_traits<T>::holder_type &
    safe_get(variant & var);

    /**
     * Comparison operators. All of these will throw a variant::error if T
     * isn't the same as the held type. The comparison operators use the
     * comparison functions from <functional> (i.e. std::equal_to, etc.), so
     * even if your held type does not define comparison operators, you can
     * produce specializations of these functions, and variant's comparisons
     * will work.
     **/
    template <typename T>
    bool operator==(T const & other) const;
    template <typename T>
    bool operator<(T const & other) const;
    template <typename T>
    bool operator<=(T const & other) const;
    template <typename T>
    bool operator>(T const & other) const;
    template <typename T>
    bool operator>=(T const & other) const;
    template <typename T>
    bool operator!=(T const & other) const;

    /**
     * Special overloads for variant, behave much the same way. Note that two
     * empty or two invalid variants will always throw, even with operator==
     * or operator!=, where we could technically return false.
     **/
    bool operator==(variant const & other) const;
    bool operator<(variant const & other) const;
    bool operator<=(variant const & other) const;
    bool operator>(variant const & other) const;
    bool operator>=(variant const & other) const;
    bool operator!=(variant const & other) const;

private:
    /**
     * Internally, variants can be in three states:
     * - Empty variants can be treated as any value, even containers. However,
     *   is() will always return false, and as() will always return a refernce
     *   to an object at 0x0. Once the variant has been assigned a value, or
     *   been treated as a variant::map_t, it's state will be changed to
     *   IS_VALUE.
     * - IS_VALUE specifies variants that have been assigned a value - it does
     *   not matter whether that value is a simple value or a container.
     * - IS_INVALID cannot be constructed explicitly, rather, it's used
     *   internally when the user has tried to perform an illegal operation
     *   on the variant. Using a variant in the IS_INVALID state will result
     *   in an variant::error exception.
     **/
    enum variant_state
    {
        IS_EMPTY   = 0,
        IS_VALUE   = 1,
        IS_INVALID = 2
    };

    /**
     * The default constructor sets the state to IS_EMPTY, copy constructors set
     * the stte to IS_VALUE. In order to construct an invalid value, we need to
     * pass it an explicit state - however, invalid values need never be
     * constructed by the user, therefore this constructor is private.
     **/
    variant(variant_state state);

    /** The state of this variant **/
    variant_state  m_state;
    /** The data (if any) this variant holds **/
    struct data_base;
    data_base * m_data;

    template <typename T>
    struct data;

    /**
     * There is little need creating hundres of invalid variants, where one
     * suffices. Thefore variant has one static variant member that is
     * constructed to be invalid, which is returned whenever one of variant's
     * functions returns an invalid value.
     **/
    static void instanciate_invalid_value();
    static fhtagn::shared_ptr<variant> invalid_value;
};


} // namespace fhtagn

#include <fhtagn/detail/variant.tcc>

#endif // guard
