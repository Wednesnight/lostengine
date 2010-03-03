/**
 * $Id: pointers_test.cpp 319 2009-06-08 22:17:47Z unwesen $
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

#include <fhtagn/pointers.h>

namespace {

struct pointer_test_struct
{
    pointer_test_struct()
        : m_counter(0)
    {
    }

    void inc() const
    {
        ++m_counter;
    }

    mutable int m_counter;
};

template <template <typename> class pointerT>
void pointer_tester(typename pointerT<pointer_test_struct>::type param)
{
    param->inc();
}


struct copy_policy_test_struct
{
    copy_policy_test_struct()
        : m_copied(false)
    {
    }

    copy_policy_test_struct(copy_policy_test_struct const & other)
        : m_copied(true)
    {
    }

    bool m_copied;
};

} // anonymous namespace

class PointerPolicyTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(PointerPolicyTest);

        CPPUNIT_TEST(testPointerTypes);
        CPPUNIT_TEST(testShallowCopyPolicy);
        CPPUNIT_TEST(testDeepCopyPolicy);

    CPPUNIT_TEST_SUITE_END();
private:

    void testPointerTypes()
    {
        // test that pointer_tester compiles and works on all types of pointers...
        pointer_test_struct x;

        pointer_tester<fhtagn::pointers::pointer>(&x);
        CPPUNIT_ASSERT_EQUAL(x.m_counter, 1);

        pointer_tester<fhtagn::pointers::const_pointer>(&x);
        CPPUNIT_ASSERT_EQUAL(x.m_counter, 2);

        pointer_tester<fhtagn::pointers::const_pointer_const>(&x);
        CPPUNIT_ASSERT_EQUAL(x.m_counter, 3);
    }


    void testShallowCopyPolicy()
    {
        using namespace fhtagn::pointers;

        copy_policy_test_struct * x = new copy_policy_test_struct();
        CPPUNIT_ASSERT_EQUAL(x->m_copied, false);

        shallow_copy<copy_policy_test_struct, pointer> first = x;
        CPPUNIT_ASSERT_EQUAL(first.m_pointer->m_copied, false);

        // after copying shallow_copy, it's embedded m_pointer must not have been
        // copy constructed.
        shallow_copy<copy_policy_test_struct, pointer> second = first;
        CPPUNIT_ASSERT_EQUAL(second.m_pointer->m_copied, false);

        delete x;
    }


    void testDeepCopyPolicy()
    {
        using namespace fhtagn::pointers;

        copy_policy_test_struct * x = new copy_policy_test_struct();
        CPPUNIT_ASSERT_EQUAL(x->m_copied, false);

        deep_copy<copy_policy_test_struct, pointer> first = x;
        CPPUNIT_ASSERT_EQUAL(first.m_pointer->m_copied, false);

        // after copying deep_copy, it's embedded m_pointer must have been
        // copy constructed.
        deep_copy<copy_policy_test_struct, pointer> second = first;
        CPPUNIT_ASSERT_EQUAL(second.m_pointer->m_copied, true);

        delete x;
    }
};


CPPUNIT_TEST_SUITE_REGISTRATION(PointerPolicyTest);
