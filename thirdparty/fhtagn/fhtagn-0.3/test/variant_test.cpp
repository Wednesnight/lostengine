/**
 * $Id: variant_test.cpp 319 2009-06-08 22:17:47Z unwesen $
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

#include <cppunit/extensions/HelperMacros.h>

#include <fhtagn/stdvariant.h>

namespace {

struct test_type
{
  test_type(int x = 0)
    : m_x(x)
  {}
  ~test_type() {}

  test_type(test_type const & other)
    : m_x(0)
  {
    // Copying may occur exactly once, that is, during the first assignment
    // to the variant.
    CPPUNIT_ASSERT_MESSAGE("Copying shouldn't occur!", ++sm_copy_count <= 1);
    m_x = other.m_x;
  }

  test_type & operator=(test_type const &)
  {
    CPPUNIT_ASSERT_MESSAGE("Assignment shouldn't occur!", false);
    return *this;
  }

  int m_x;
  static int sm_copy_count;

  bool operator==(test_type const & other) const
  {
    return m_x == other.m_x;
  }
};

int test_type::sm_copy_count = 0;

} // anonymous namespace
FHTAGN_VARIANT_SPECIALIZE(test_type)
FHTAGN_VARIANT_SPECIALIZE_COMPARE(test_type, not_equal_to,
        fhtagn::variant::compare_throw<test_type>)
FHTAGN_VARIANT_SPECIALIZE_COMPARE(test_type, less,
        fhtagn::variant::compare_throw<test_type>)
FHTAGN_VARIANT_SPECIALIZE_COMPARE(test_type, less_equal,
        fhtagn::variant::compare_throw<test_type>)
FHTAGN_VARIANT_SPECIALIZE_COMPARE(test_type, greater,
        fhtagn::variant::compare_throw<test_type>)
FHTAGN_VARIANT_SPECIALIZE_COMPARE(test_type, greater_equal,
        fhtagn::variant::compare_throw<test_type>)

class VariantTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(VariantTest);

      CPPUNIT_TEST(testSimple);
      CPPUNIT_TEST(testNoCopy);
      CPPUNIT_TEST(testArray);
      CPPUNIT_TEST(testMap);
      CPPUNIT_TEST(testString);
      CPPUNIT_TEST(testCheck);
      CPPUNIT_TEST(testExists);
      CPPUNIT_TEST(testSafeGet);
      CPPUNIT_TEST(testOperators);

    CPPUNIT_TEST_SUITE_END();
private:
    void testSimple()
    {
        fhtagn::variant b = true;
        CPPUNIT_ASSERT(b.is<bool>());
        CPPUNIT_ASSERT(b.as<bool>() == true);
        fhtagn::variant x = 42;
        CPPUNIT_ASSERT(x.is<int>());
        x.as<int>() += 3;
        CPPUNIT_ASSERT_EQUAL(45, x.as<int>());

        x = 3.141592;
        CPPUNIT_ASSERT(x.is<double>());
        CPPUNIT_ASSERT(!x.is<int>());

        // copy the variant, make sure the copy behaves the same
        fhtagn::variant y;
        y = x;
        CPPUNIT_ASSERT(y.is<double>());
        CPPUNIT_ASSERT(!y.is<int>());
    }

    void testNoCopy()
    {
        // Ensures that no (technically: only one) copy of a value is made
        // when assigning to a variant, in cases where the value's type is
        // identical to it's holder's type (see variant::specialization_traits).
        test_type x = 42;
        fhtagn::variant b = x;
        CPPUNIT_ASSERT(b.is<test_type>());
        CPPUNIT_ASSERT(x == b.as<test_type>());

        b.as<test_type>().m_x = 666;
        CPPUNIT_ASSERT(666 == b.as<test_type>().m_x);
    }

    void testArray()
    {
        fhtagn::variant y = fhtagn::variant::array_t(1);
        CPPUNIT_ASSERT(y.is<fhtagn::variant::array_t>());
        y[0] = 42;
        y[0].as<int>() += 1234;
        CPPUNIT_ASSERT_EQUAL(1276, y[0].as<int>());

        // treating y[0] as a container must fail
        CPPUNIT_ASSERT_THROW(y[0]["foo"] = 123, fhtagn::variant::error);

        // automatically promoting an undefined variant to array must fail
        fhtagn::variant x;
        CPPUNIT_ASSERT_THROW(x[0] = 42, fhtagn::variant::error);

        // try using a vector method...
        fhtagn::variant z = fhtagn::variant::array_t();
        z.as<fhtagn::variant::array_t>().push_back(12);
    }


    void testMap()
    {
        // promoting an undefined variable to map (or array) must succeed
        fhtagn::variant x;
        CPPUNIT_ASSERT_NO_THROW(x["foo"] = 123);
        CPPUNIT_ASSERT(x.is<fhtagn::variant::map_t>());
        CPPUNIT_ASSERT(x["foo"].is<int>());

        // even to more than one level
        fhtagn::variant y;
        CPPUNIT_ASSERT_NO_THROW(y["foo"]["bar"]["baz"] = 123);
        CPPUNIT_ASSERT(y.is<fhtagn::variant::map_t>());
        CPPUNIT_ASSERT(y["foo"].is<fhtagn::variant::map_t>());
        CPPUNIT_ASSERT(y["foo"]["bar"].is<fhtagn::variant::map_t>());
        CPPUNIT_ASSERT(y["foo"]["bar"]["baz"].is<int>());

        // again, as with arrays, can't change a map to another type implicitly
        CPPUNIT_ASSERT_THROW(y["foo"][0] = 123, fhtagn::variant::error);

        // but can do so explicitly
        CPPUNIT_ASSERT_NO_THROW(y["foo"] = 123);

        // Force a throw. First, we assign a variant a non-map value. Then we try
        // to use that variant as a map, which should return an invalid_value.
        fhtagn::variant z = 123;
        CPPUNIT_ASSERT_NO_THROW(z["foo"]);
        CPPUNIT_ASSERT_THROW(z["foo"] = 123, fhtagn::variant::error);
    }


    void testString()
    {
        fhtagn::variant x;
        CPPUNIT_ASSERT_NO_THROW(x = std::string("hello"));
        CPPUNIT_ASSERT(x.is<std::string>());

        // different holder type from value type
        char const * foo = "world";
        CPPUNIT_ASSERT_NO_THROW(x = foo);
        CPPUNIT_ASSERT(x.is<std::string>());

        // works on some GCCs at least
        CPPUNIT_ASSERT_NO_THROW(x = "baz");
    }

    void testCheck()
    {
        fhtagn::variant x;
        CPPUNIT_ASSERT_EQUAL(false, fhtagn::variant::check<int>(x));
        CPPUNIT_ASSERT_EQUAL(false, fhtagn::variant::check<int>(x["foo"]["bar"]));

        x["foo"]["bar"] = 3.141592;
        CPPUNIT_ASSERT_EQUAL(false, fhtagn::variant::check<int>(x["foo"]["bar"]));

        x["foo"]["bar"] = 3;
        CPPUNIT_ASSERT_EQUAL(true, fhtagn::variant::check<int>(x["foo"]["bar"]));

        // If x is a non-const reference, unfortunately checking for
        // x["baz"]["quux"] would create ["baz"] as an array_t. Let's test with
        // a const ref first, because that should just return false.
        fhtagn::variant const & y = x;
        CPPUNIT_ASSERT_EQUAL(false, fhtagn::variant::check<int>(y["baz"]["quux"]));
        CPPUNIT_ASSERT_EQUAL(false, fhtagn::variant::check<fhtagn::variant::array_t>(y["baz"]));

        // x being a non-const reference, "baz" gets added as a map_t
        CPPUNIT_ASSERT_EQUAL(false, fhtagn::variant::check<int>(x["baz"]["quux"]));
        CPPUNIT_ASSERT_EQUAL(true, fhtagn::variant::check<fhtagn::variant::map_t>(x["baz"]));

        // let's try if passing a non-const variable to the FHTAGN_VARIANT_CHECK
        // macro is safer:
        fhtagn::variant z;
        CPPUNIT_ASSERT_EQUAL(false, FHTAGN_VARIANT_CHECK(int, z, ["baz"]["quux"]));
        CPPUNIT_ASSERT_EQUAL(false, FHTAGN_VARIANT_CHECK(fhtagn::variant::map_t, z, ["baz"]));
    }

    void testExists()
    {
        fhtagn::variant x;
        CPPUNIT_ASSERT_EQUAL(true, fhtagn::variant::exists(x["foo"]["bar"]));

        x["foo"]["bar"] = 3.141592;
        CPPUNIT_ASSERT_EQUAL(true, fhtagn::variant::exists(x["foo"]["bar"]));

        x["foo"]["bar"] = fhtagn::variant::array_t();
        x["foo"]["bar"].as<fhtagn::variant::array_t>().push_back(123);
        CPPUNIT_ASSERT_EQUAL(true, fhtagn::variant::exists(x["foo"]["bar"][0]));

        // If x is a non-const reference, unfortunately checking for
        // x["baz"]["quux"] would create ["baz"] as an array_t. Let's test with
        // a const ref first, because that should just return false.
        fhtagn::variant const & y = x;
        CPPUNIT_ASSERT_EQUAL(false, fhtagn::variant::exists(y["baz"]["quux"]));
        CPPUNIT_ASSERT_EQUAL(false, fhtagn::variant::exists(y["baz"]));

        // x being a non-const reference, "baz" gets added as a map_t
        CPPUNIT_ASSERT_EQUAL(true, fhtagn::variant::exists(x["baz"]["quux"]));
        CPPUNIT_ASSERT_EQUAL(true, fhtagn::variant::exists(x["baz"]));

        // let's try if passing a non-const variable to the FHTAGN_VARIANT_EXISTS
        // macro is safer:
        fhtagn::variant z;
        CPPUNIT_ASSERT_EQUAL(false, FHTAGN_VARIANT_EXISTS(z, ["baz"]["quux"]));
        CPPUNIT_ASSERT_EQUAL(false, FHTAGN_VARIANT_EXISTS(z, ["baz"]));
    }

    void testSafeGet()
    {
        fhtagn::variant x;
        CPPUNIT_ASSERT_THROW(fhtagn::variant::safe_get<int>(x), fhtagn::variant::error);
        CPPUNIT_ASSERT_THROW(fhtagn::variant::safe_get<int>(x["foo"]["bar"]), fhtagn::variant::error);

        x["foo"]["bar"] = 3.141592;
        CPPUNIT_ASSERT_THROW(fhtagn::variant::safe_get<int>(x["foo"]["bar"]), fhtagn::variant::error);

        x["foo"]["bar"] = 3;
        CPPUNIT_ASSERT_NO_THROW(fhtagn::variant::safe_get<int>(x["foo"]["bar"]));
        CPPUNIT_ASSERT_EQUAL(3, fhtagn::variant::safe_get<int>(x["foo"]["bar"]));

        // similar to the problems with fhtagn::variant::check, test the macros.
        fhtagn::variant z;
        CPPUNIT_ASSERT_THROW(FHTAGN_VARIANT_SAFE_GET(int, z, ["baz"]["quux"]), fhtagn::variant::error);
        CPPUNIT_ASSERT_EQUAL(false, FHTAGN_VARIANT_CHECK(fhtagn::variant::map_t, z, ["baz"]));
    }


    void testOperators()
    {
        fhtagn::variant x = 42;

        // Test positives
        CPPUNIT_ASSERT(x == 42);
        CPPUNIT_ASSERT(x < 43);
        CPPUNIT_ASSERT(x <= 42);
        CPPUNIT_ASSERT(x > 41);
        CPPUNIT_ASSERT(x >= 42);
        CPPUNIT_ASSERT(x != 0);

        // Test negatives
        CPPUNIT_ASSERT(!(x == 43));
        CPPUNIT_ASSERT(!(x < 41));
        CPPUNIT_ASSERT(!(x <= 41));
        CPPUNIT_ASSERT(!(x > 43));
        CPPUNIT_ASSERT(!(x >= 43));
        CPPUNIT_ASSERT(!(x != 42));

        // Test other type
        CPPUNIT_ASSERT_THROW(x == 42.0, fhtagn::variant::error);
        CPPUNIT_ASSERT_THROW(x < 43.0, fhtagn::variant::error);
        CPPUNIT_ASSERT_THROW(x <= 42.0, fhtagn::variant::error);
        CPPUNIT_ASSERT_THROW(x > 41.0, fhtagn::variant::error);
        CPPUNIT_ASSERT_THROW(x >= 42.0, fhtagn::variant::error);
        CPPUNIT_ASSERT_THROW(x != 0.0, fhtagn::variant::error);

        // Test positives with other variants
        CPPUNIT_ASSERT(x == fhtagn::variant(42));
        CPPUNIT_ASSERT(x < fhtagn::variant(43));
        CPPUNIT_ASSERT(x <= fhtagn::variant(42));
        CPPUNIT_ASSERT(x > fhtagn::variant(41));
        CPPUNIT_ASSERT(x >= fhtagn::variant(42));
        CPPUNIT_ASSERT(x != fhtagn::variant(0));

        // Test negatives with other variants
        CPPUNIT_ASSERT(!(x == fhtagn::variant(43)));
        CPPUNIT_ASSERT(!(x < fhtagn::variant(41)));
        CPPUNIT_ASSERT(!(x <= fhtagn::variant(41)));
        CPPUNIT_ASSERT(!(x > fhtagn::variant(43)));
        CPPUNIT_ASSERT(!(x >= fhtagn::variant(43)));
        CPPUNIT_ASSERT(!(x != fhtagn::variant(42)));

        // Test other types with other variants
        CPPUNIT_ASSERT_THROW(x == fhtagn::variant(42.0), fhtagn::variant::error);
        CPPUNIT_ASSERT_THROW(x < fhtagn::variant(43.0), fhtagn::variant::error);
        CPPUNIT_ASSERT_THROW(x <= fhtagn::variant(42.0), fhtagn::variant::error);
        CPPUNIT_ASSERT_THROW(x > fhtagn::variant(41.0), fhtagn::variant::error);
        CPPUNIT_ASSERT_THROW(x >= fhtagn::variant(42.0), fhtagn::variant::error);
        CPPUNIT_ASSERT_THROW(x != fhtagn::variant(0.0), fhtagn::variant::error);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(VariantTest);
