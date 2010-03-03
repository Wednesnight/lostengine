/**
 * $Id: meta_test.cpp 319 2009-06-08 22:17:47Z unwesen $
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

#include <vector>

#include <boost/noncopyable.hpp>

#include <cppunit/extensions/HelperMacros.h>

#include <fhtagn/meta/comparison.h>
#include <fhtagn/meta/for.h>

namespace {

static int test_sum = 0;

void
dynamic_for_functor1(int i)
{
  test_sum += i;
}


struct dynamic_for_functor2 : boost::noncopyable
{
  void operator()(int i)
  {
    test_sum += i;
  }
};


template <int CURRENT>
struct inc_twice : public fhtagn::meta::increment<CURRENT, 2>
{
};


template <int I>
struct static_functor
{
  void operator()()
  {
    test_sum += I;
  }


  void operator()(int factor)
  {
    test_sum += I * factor;
  }
};


} // anonymous namespace

class MetaTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(MetaTest);

      CPPUNIT_TEST(testComparison);
      CPPUNIT_TEST(testDynamicFor);
      CPPUNIT_TEST(testStaticFor);

    CPPUNIT_TEST_SUITE_END();

private:
    void testComparison()
    {
      namespace meta = fhtagn::meta;

      // equal
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::equal<42, 42>::value));
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::equal<42, 43>::value));

      // not_equal
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::not_equal<42, 42>::value));
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::not_equal<42, 43>::value));

      // greater
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::greater<42, 42>::value));
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::greater<42, 43>::value));
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::greater<43, 42>::value));

      // less
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::less<42, 42>::value));
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::less<42, 43>::value));
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::less<43, 42>::value));

      // greater_equal
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::greater_equal<42, 42>::value));
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::greater_equal<42, 43>::value));
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::greater_equal<43, 42>::value));

      // less_equal
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::less_equal<42, 42>::value));
      CPPUNIT_ASSERT_EQUAL(true,  bool(meta::less_equal<42, 43>::value));
      CPPUNIT_ASSERT_EQUAL(false, bool(meta::less_equal<43, 42>::value));
    }



    void testDynamicFor()
    {
      namespace meta = fhtagn::meta;

      // Test both functors with default increments, i.e. inc_once
      test_sum = 0;
      {
        meta::dynamic_for<0, 10>(&dynamic_for_functor1);
        CPPUNIT_ASSERT_EQUAL(45, test_sum);
      }

      test_sum = 0;
      {
        dynamic_for_functor2 f;
        meta::dynamic_for<0, 10>(f);
        CPPUNIT_ASSERT_EQUAL(45, test_sum);
      }

      // Test both functors with increments by two.
      test_sum = 0;
      {
        meta::dynamic_for<0, 10, inc_twice>(&dynamic_for_functor1);
        CPPUNIT_ASSERT_EQUAL(20, test_sum);
      }

      test_sum = 0;
      {
        dynamic_for_functor2 f;
        meta::dynamic_for<0, 10, inc_twice>(f);
        CPPUNIT_ASSERT_EQUAL(20, test_sum);
      }

      // Test both functor with a custom comparator.
      test_sum = 0;
      {
        meta::dynamic_for<0, 10, meta::less_equal>(&dynamic_for_functor1);
        CPPUNIT_ASSERT_EQUAL(55, test_sum);
      }

      test_sum = 0;
      {
        dynamic_for_functor2 f;
        meta::dynamic_for<0, 10, meta::less_equal>(f);
        CPPUNIT_ASSERT_EQUAL(55, test_sum);
      }
    }


    void testStaticFor()
    {
      namespace meta = fhtagn::meta;

      // *** Tests with default comparator
      // Test functor with default increments and no aditional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, static_functor>();
        CPPUNIT_ASSERT_EQUAL(45, test_sum);
      }

      // Test functor with increments by two and no additional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, inc_twice, static_functor>();
        CPPUNIT_ASSERT_EQUAL(20, test_sum);
      }

      // Test functor with default increments and an addtional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, static_functor>(3);
        CPPUNIT_ASSERT_EQUAL(45 * 3, test_sum);
      }

      // Test functor with increments by two and an additional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, inc_twice, static_functor>(3);
        CPPUNIT_ASSERT_EQUAL(20 * 3, test_sum);
      }

      // *** Tests with non-default comparator
      // Test functor with default increments and no aditional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, meta::less_equal, static_functor>();
        CPPUNIT_ASSERT_EQUAL(55, test_sum);
      }

      // Test functor with increments by two and no additional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, inc_twice, meta::less_equal, static_functor>();
        CPPUNIT_ASSERT_EQUAL(30, test_sum);
      }

      // Test functor with default increments and an addtional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, meta::less_equal, static_functor>(3);
        CPPUNIT_ASSERT_EQUAL(55 * 3, test_sum);
      }

      // Test functor with increments by two and an additional parameter
      test_sum = 0;
      {
        meta::static_for<0, 10, inc_twice, meta::less_equal, static_functor>(3);
        CPPUNIT_ASSERT_EQUAL(30 * 3, test_sum);
      }
    }
};


CPPUNIT_TEST_SUITE_REGISTRATION(MetaTest);
