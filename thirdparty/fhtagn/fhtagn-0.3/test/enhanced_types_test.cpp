/**
 * $Id: enhanced_types_test.cpp 319 2009-06-08 22:17:47Z unwesen $
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

#include <cstdlib>

#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <cppunit/extensions/HelperMacros.h>

#include <fhtagn/property.h>
#include <fhtagn/mandatory.h>
#include <fhtagn/restrictions.h>

namespace {

struct rw_property_test
{
    rw_property_test()
        : the_property(this, &rw_property_test::get, &rw_property_test::set)
        , m_value(0)
    {
    }

    int get() const
    {
        return m_value;
    }

    void set(int const & new_value)
    {
        m_value = new_value;
    }

    fhtagn::property<int, rw_property_test> the_property;

    int m_value;
};


fhtagn::throw_if_unchecked<int> mandatory_function()
{
    return 42;
}


/** test a few restrictions, thereby proving the general mechanism */
fhtagn::restricted<int, fhtagn::restrictions::numeric::even<int> >
restricted_function(fhtagn::restricted<
        std::string,
        fhtagn::restrictions::container::non_empty<std::string>
    >, int return_value)
{
    return return_value;
}


} // anonymous namespace

class EnhancedTypesTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(EnhancedTypesTest);

        CPPUNIT_TEST(testProperty);
        CPPUNIT_TEST(testMandatory);
        CPPUNIT_TEST(testRestricted);
//        CPPUNIT_TEST(testRestrictedSpeed);
        CPPUNIT_TEST(testRestrictedORChain);

    CPPUNIT_TEST_SUITE_END();
private:

    void testProperty()
    {
        rw_property_test tester;
        tester.the_property = 123; // this line fails to compile for a read_only_property
        CPPUNIT_ASSERT_EQUAL(tester.m_value, 123);

        // here we're testing whether the overloaded assignment operators do
        // their job, by example of the /= operator.
        tester.the_property /= 10; // this line fails to compile for a write_only_property
        CPPUNIT_ASSERT_EQUAL(tester.m_value, 12);

        // whereas this test shows that casting a property to it's reprT, using
        // the result in a calculation, and assigning back to a property works
        // just as well.
        tester.the_property = tester.the_property * 10;
        CPPUNIT_ASSERT_EQUAL(tester.m_value, 120);
    }


    void testMandatory()
    {
        int ret = 0;
        CPPUNIT_ASSERT_THROW(mandatory_function(), std::logic_error);
        CPPUNIT_ASSERT_NO_THROW(static_cast<fhtagn::ignore_return_value>(mandatory_function()));
        CPPUNIT_ASSERT_NO_THROW(fhtagn::ignore_return_value x = mandatory_function());
        CPPUNIT_ASSERT_NO_THROW(ret = mandatory_function());

        ret = mandatory_function();
        CPPUNIT_ASSERT_EQUAL(ret, 42);
    }


    void testRestricted()
    {
        // doesn't throw, all restrictions are fulfilled
        CPPUNIT_ASSERT_NO_THROW(restricted_function(std::string("Hello, world!"), 10));

        // throws because std::string() produces a non-empty container
        CPPUNIT_ASSERT_THROW(restricted_function(std::string(), 10), fhtagn::restrictions::violation_error);

        // throws because 11 is not an even integer
        CPPUNIT_ASSERT_THROW(restricted_function(std::string("Hello, world!"), 11), fhtagn::restrictions::violation_error);

        // test a special numeric restriction, i.e. that a pointer is non-zero:
        typedef fhtagn::restricted<
            char const *,
            fhtagn::restrictions::numeric::non_default_value<char const *>
        > non_zero_char_pointer;
        CPPUNIT_ASSERT_THROW(non_zero_char_pointer p, fhtagn::restrictions::violation_error);
        CPPUNIT_ASSERT_NO_THROW(non_zero_char_pointer p = "Hello, world!");

        // Swap two values
        typedef fhtagn::restricted<int, fhtagn::restrictions::numeric::even<int> > even_int_t;
        even_int_t a = 2;
        CPPUNIT_ASSERT_THROW(a = 3, fhtagn::restrictions::violation_error);
        even_int_t b = 4;
        CPPUNIT_ASSERT_THROW(b = 3, fhtagn::restrictions::violation_error);
        CPPUNIT_ASSERT_NO_THROW(a.swap(b));
        CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(b));
        CPPUNIT_ASSERT_EQUAL(4, static_cast<int>(a));
        CPPUNIT_ASSERT_NO_THROW(std::swap(a, b));
        CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(a));
        CPPUNIT_ASSERT_EQUAL(4, static_cast<int>(b));

        // Silly thing, really... test that assigning to even_int_t returns a value.
        CPPUNIT_ASSERT(a = 6);
    }


    void testRestrictedSpeed()
    {
      // Test the overhead of a restricted int without restrictions. People
      // sometimes ask whether or not the overhead is worth adding such checks.

      namespace bpt = boost::posix_time;

      typedef fhtagn::restricted<int> no_restrict_int;

      // Takes ~0.1 sec on my machine
      const int amount = 10000000;

      // First, fill a vector of N regular ints.
      std::vector<int> vec1;
      vec1.reserve(amount);

      bpt::ptime plain_start = bpt::microsec_clock::universal_time();
      for (int i = 1 ; i <= amount ; ++i) {
        vec1.push_back(i);
      }
      bpt::ptime plain_end = bpt::microsec_clock::universal_time();

      // Then, do the same with a restricted int (with no restrictions, i.e.
      // no_restrict_int)
      std::vector<no_restrict_int> vec2;
      vec2.reserve(amount);

      bpt::ptime no_restrict_start = bpt::microsec_clock::universal_time();
      for (int i = 1 ; i <= amount ; ++i) {
        vec2.push_back(i);
      }
      bpt::ptime no_restrict_end = bpt::microsec_clock::universal_time();


      bpt::time_duration plain_time = plain_end - plain_start;
      bpt::time_duration no_restrict_time = no_restrict_end - no_restrict_start;

      // Because wall time sucks as a performance/overhead measurement, we'll
      // just make the simplifying assumption that a 20% difference between
      // both durations is a failure - there should be close to no overhead
      // after all.
      const double percentage = 0.2;

      bpt::time_duration::tick_type ms = std::max(plain_time, no_restrict_time)
        .total_microseconds();
      bpt::time_duration::tick_type diff_ms = std::labs(
          (plain_time - no_restrict_time).total_microseconds());
      CPPUNIT_ASSERT(diff_ms < (ms * percentage));

      // Just for the hell of it, add another test where the int is restricted
      // to be positive.

      typedef fhtagn::restricted<int,
              fhtagn::restrictions::numeric::positive<int> > restrict_int;

      CPPUNIT_ASSERT_THROW(restrict_int(-1),
          fhtagn::restrictions::violation_error);

      std::vector<restrict_int> vec3;
      vec3.reserve(amount);

      bpt::ptime restrict_start = bpt::microsec_clock::universal_time();
      for (int i = 1 ; i <= amount ; ++i) {
        vec3.push_back(i);
      }
      bpt::ptime restrict_end = bpt::microsec_clock::universal_time();

      bpt::time_duration restrict_time = restrict_end - restrict_start;

      // These checks can't be expensive.
      ms = std::max(plain_time, restrict_time).total_microseconds();
      diff_ms = std::labs((plain_time - restrict_time).total_microseconds());
      CPPUNIT_ASSERT(diff_ms < (ms * percentage));
    }



    void testRestrictedORChain()
    {
      // Test whether OR-chaining of restrictions works as intended.
      typedef fhtagn::restricted<int,
              fhtagn::restrictions::numeric::null_or_positive<int> > restrict_int;

      CPPUNIT_ASSERT_THROW(restrict_int(-1),
          fhtagn::restrictions::violation_error);

      CPPUNIT_ASSERT_NO_THROW(restrict_int(0));
      CPPUNIT_ASSERT_NO_THROW(restrict_int(1));
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(EnhancedTypesTest);
