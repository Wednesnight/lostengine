/**
 * $Id: threads_test.cpp 319 2009-06-08 22:17:47Z unwesen $
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

#include <boost/bind.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <cppunit/extensions/HelperMacros.h>

#include <fhtagn/threads/tasklet.h>
#include <fhtagn/threads/lock_policy.h>
#include <fhtagn/threads/future.h>

namespace {

bool free_done = false;
void free_thread_func(fhtagn::threads::tasklet & t)
{
    free_done = true;
}


void throwing_free_func(fhtagn::threads::tasklet & t)
{
    throw std::runtime_error("test_error");
}


bool free_error_handled = false;
void error_handler(fhtagn::threads::tasklet & t, std::exception const & ex)
{
    CPPUNIT_ASSERT_EQUAL(std::string("test_error"), std::string(ex.what()));
    free_error_handled = true;
}


struct bind_test
{
    bind_test()
        : done(false)
    {
    }


    void simple_member(fhtagn::threads::tasklet & t)
    {
        namespace th = fhtagn::threads;

        // These asserts lead to a premature end of the thread, and prevent
        // the done flag from being set.
        CPPUNIT_ASSERT(t.alive());

        // Signal that the function is finished.
        done = true;
    }


    void sleep_halfsec(fhtagn::threads::tasklet & t)
    {
        namespace th = fhtagn::threads;
        t.sleep(500000);
        done = true;
    }


    void looping_member(fhtagn::threads::tasklet & t)
    {
        namespace th = fhtagn::threads;

        // This isn't terribly efficient, that's what sleep() exists for...
        while (t.get_state() == th::tasklet::RUNNING
                || t.get_state() == th::tasklet::SLEEPING)
        {
            // tum-tee-tum.
        }

        // Signal that the function is finished.
        done = true;
    }


    void sleeping_member(fhtagn::threads::tasklet & t)
    {
        // It's much more efficient to sleep until woken up.
        while (!t.sleep()) {
            // tum-tee-tum.
        }

        // Signal that the function is finished.
        done = true;
    }

    bool     done;
};



template <typename mutexT>
struct mutex_test
{
    mutexT m_mutex;

    void test_lock_guard()
    {
        // Lockable via lock_guard
        {
            boost::lock_guard<mutexT> l1(m_mutex);
        }

        // Unlockable via lock_guard
        m_mutex.lock();
        {
            boost::lock_guard<mutexT> l2(m_mutex, boost::adopt_lock_t());
        }

        // ... make sure lock_guard dropped the lock
        boost::unique_lock<mutexT> l3(m_mutex);
    }


    void test_unique_lock_nonrecursive()
    {
        {
            boost::unique_lock<mutexT> l1(m_mutex);
            CPPUNIT_ASSERT_EQUAL(true, l1.owns_lock());
            CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l1));

            boost::unique_lock<mutexT> l2(m_mutex, boost::try_to_lock_t());
            CPPUNIT_ASSERT_EQUAL(false, l2.owns_lock());
            CPPUNIT_ASSERT_EQUAL(false, static_cast<bool>(l2));

            l1.unlock();
            CPPUNIT_ASSERT_EQUAL(false, l1.owns_lock());
            CPPUNIT_ASSERT_EQUAL(false, static_cast<bool>(l1));

            boost::unique_lock<mutexT> l3(m_mutex, boost::try_to_lock_t());
            CPPUNIT_ASSERT_EQUAL(true, l3.owns_lock());
            CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l3));
        }

        m_mutex.lock();
        {
            boost::unique_lock<mutexT> l4(m_mutex, boost::adopt_lock_t());
            CPPUNIT_ASSERT_EQUAL(true, l4.owns_lock());
            CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l4));
        }

        boost::unique_lock<mutexT> l5(m_mutex, boost::defer_lock_t());
        CPPUNIT_ASSERT_EQUAL(false, l5.owns_lock());
        CPPUNIT_ASSERT_EQUAL(false, static_cast<bool>(l5));

        l5.lock();
        CPPUNIT_ASSERT_EQUAL(true, l5.owns_lock());
        CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l5));
    }


    void test_unique_lock_recursive()
    {
        {
            boost::unique_lock<mutexT> l1(m_mutex);
            CPPUNIT_ASSERT_EQUAL(true, l1.owns_lock());
            CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l1));

            boost::unique_lock<mutexT> l2(m_mutex, boost::try_to_lock_t());
            CPPUNIT_ASSERT_EQUAL(true, l2.owns_lock());
            CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l2));
        }

        m_mutex.lock();
        {
            boost::unique_lock<mutexT> l3(m_mutex, boost::adopt_lock_t());
            CPPUNIT_ASSERT_EQUAL(true, l3.owns_lock());
            CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l3));
        }

        boost::unique_lock<mutexT> l4(m_mutex, boost::defer_lock_t());
        CPPUNIT_ASSERT_EQUAL(false, l4.owns_lock());
        CPPUNIT_ASSERT_EQUAL(false, static_cast<bool>(l4));

        l4.lock();
        CPPUNIT_ASSERT_EQUAL(true, l4.owns_lock());
        CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l4));
    }


    void test_timed_lock_nonrecursive()
    {
        boost::unique_lock<mutexT> l1(m_mutex, boost::get_system_time());
        CPPUNIT_ASSERT_EQUAL(true, l1.owns_lock());
        CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l1));

        boost::unique_lock<mutexT> l2(m_mutex, boost::get_system_time());
        CPPUNIT_ASSERT_EQUAL(false, l2.owns_lock());
        CPPUNIT_ASSERT_EQUAL(false, static_cast<bool>(l2));
    }


    void test_timed_lock_recursive()
    {
        boost::unique_lock<mutexT> l1(m_mutex, boost::get_system_time());
        CPPUNIT_ASSERT_EQUAL(true, l1.owns_lock());
        CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l1));

        boost::unique_lock<mutexT> l2(m_mutex, boost::get_system_time());
        CPPUNIT_ASSERT_EQUAL(true, l2.owns_lock());
        CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l2));
    }


    void lock_shared_lock()
    {
        boost::shared_lock<mutexT> l(m_mutex);
        CPPUNIT_ASSERT_EQUAL(true, l.owns_lock());
        CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l));
    }


    void test_shared_lock()
    {
        boost::shared_lock<mutexT> l(m_mutex);
        CPPUNIT_ASSERT_EQUAL(true, l.owns_lock());
        CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l));

        // Works like a recursive mutex...
        boost::shared_lock<mutexT> l2(m_mutex);
        CPPUNIT_ASSERT_EQUAL(true, l2.owns_lock());
        CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l2));

        // ... but also across threads
        namespace th = fhtagn::threads;
        th::tasklet t(boost::bind(&mutex_test<mutexT>::lock_shared_lock,
              this));
        CPPUNIT_ASSERT(t.start());
        CPPUNIT_ASSERT(t.wait());
    }


    void test_upgrade_lock()
    {
        boost::shared_lock<mutexT> l(m_mutex);
        CPPUNIT_ASSERT_EQUAL(true, l.owns_lock());
        CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l));

        // Works like a recursive mutex with shared_locks...
        boost::upgrade_lock<mutexT> l2(m_mutex);
        CPPUNIT_ASSERT_EQUAL(true, l2.owns_lock());
        CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l2));

        // ... so we can have even more shared locks. (but only one upgrade_lock
        // per mutex).
        boost::shared_lock<mutexT> l3(m_mutex);
        CPPUNIT_ASSERT_EQUAL(true, l3.owns_lock());
        CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(l3));

        // ... once the shared_locks are unlocked, we can upgrade the
        // upgrade_lock.
        l.unlock();
        l3.unlock();

        {
            boost::upgrade_to_unique_lock<mutexT> l4(l2);

            // ... at which point the shared_locks can't be locked any longer.
            CPPUNIT_ASSERT_EQUAL(false, l.try_lock());
            CPPUNIT_ASSERT_EQUAL(false, l3.try_lock());
        }

        // At least until the upgrade is reversed.
        CPPUNIT_ASSERT_EQUAL(true, l.try_lock());
        CPPUNIT_ASSERT_EQUAL(true, l3.try_lock());
    }
};



fhtagn::size_t future_func()
{
  return 42;
}

fhtagn::size_t throwing_future_func()
{
  throw std::runtime_error("This is expected.");
  return 42;
}


} // anonymous namespace

class ThreadsTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(ThreadsTest);

        CPPUNIT_TEST(testTaskletSleep);
        CPPUNIT_TEST(testTaskletMemFun);
        CPPUNIT_TEST(testTaskletFreeFun);
        CPPUNIT_TEST(testTaskletError);
        CPPUNIT_TEST(testTaskletScope);

        CPPUNIT_TEST(testMutexConcepts);
        CPPUNIT_TEST(testMutexes);

        CPPUNIT_TEST(testFutures);

    CPPUNIT_TEST_SUITE_END();
private:


    void testTaskletSleep()
    {
        namespace th = fhtagn::threads;

        // By testing the msec-based version of sleep(), both functions are
        // tested nicely. It might be prudent to test them separately at some
        // point though.

        // Check whether sleep() correctly handles 0 msecs (no sleeping)
        {
            bind_test bt;
            th::tasklet task(boost::bind(&bind_test::sleeping_member,
                        boost::ref(bt), _1));

            boost::xtime t1;
            boost::xtime t2;

            boost::xtime_get(&t1, boost::TIME_UTC);
            CPPUNIT_ASSERT(task.start());
            CPPUNIT_ASSERT(task.stop());
            CPPUNIT_ASSERT(task.wait());
            boost::xtime_get(&t2, boost::TIME_UTC);

            boost::uint64_t st1 = (t1.sec * 1000000000) + t1.nsec;
            boost::uint64_t st2 = (t2.sec * 1000000000) + t2.nsec;

            // The time difference must always be lower than the 1000 msecs
            // (1000000 nsecs) we specified as the sleep time.
            CPPUNIT_ASSERT((st2 - st1) < 1000000000);
        }

        // Same test, but with a half-second sleep. Now the elapsed time must
        // be larger than half a second.
        {
            bind_test bt;
            th::tasklet task(boost::bind(&bind_test::sleep_halfsec,
                        boost::ref(bt), _1));

            boost::xtime t1;
            boost::xtime t2;

            boost::xtime_get(&t1, boost::TIME_UTC);
            CPPUNIT_ASSERT(task.start());
            CPPUNIT_ASSERT(task.wait());
            boost::xtime_get(&t2, boost::TIME_UTC);

            boost::uint64_t st1 = (t1.sec * 1000000000) + t1.nsec;
            boost::uint64_t st2 = (t2.sec * 1000000000) + t2.nsec;

            // The time difference must always be larger than the sleep time of
            // 500 msec
            CPPUNIT_ASSERT((st2 - st1) >= 500000000);
        }

    }


    void testTaskletMemFun()
    {
        namespace th = fhtagn::threads;

        // Bind a simple member function that terminates immediately.
        {
            bind_test bt;
            th::tasklet task(boost::bind(&bind_test::simple_member,
                        boost::ref(bt), _1));

            CPPUNIT_ASSERT(task.start());
            CPPUNIT_ASSERT(task.get_state() == th::tasklet::RUNNING
                    || task.get_state() == th::tasklet::FINISHED);

            CPPUNIT_ASSERT(task.wait());

            CPPUNIT_ASSERT(bt.done && "An assertion failure here may indicate "
                    "an assertion failure in the bound function.");
        }

        // Bind a looping member function that (inefficiently) polls stopped()
        {
            bind_test bt;
            th::tasklet task(boost::bind(&bind_test::looping_member,
                        boost::ref(bt), _1));

            CPPUNIT_ASSERT(task.start());
            CPPUNIT_ASSERT(task.get_state() == th::tasklet::RUNNING);

            // must stop the thread before waiting.
            CPPUNIT_ASSERT(task.stop());
            CPPUNIT_ASSERT(task.wait());

            CPPUNIT_ASSERT(bt.done && "An assertion failure here may indicate "
                    "an assertion failure in the bound function.");
        }

        // Bind a member function that uses sleep() to wait until it's stopped.
        {
            bind_test bt;
            th::tasklet task(boost::bind(&bind_test::sleeping_member,
                        boost::ref(bt), _1));

            CPPUNIT_ASSERT(task.start());
            CPPUNIT_ASSERT(task.get_state() == th::tasklet::RUNNING
                    || task.get_state() == th::tasklet::SLEEPING);

            // must stop the thread before waiting.
            CPPUNIT_ASSERT(task.stop());
            CPPUNIT_ASSERT(task.wait());

            CPPUNIT_ASSERT(bt.done && "An assertion failure here may indicate "
                    "an assertion failure in the bound function.");
        }
    }


    void testTaskletFreeFun()
    {
        namespace th = fhtagn::threads;

        // Test whether a free function can be bound the same as a member
        // function.
        free_done = false;
        th::tasklet task(boost::bind(&free_thread_func, _1));

        CPPUNIT_ASSERT(task.start());
        CPPUNIT_ASSERT(task.get_state() == th::tasklet::RUNNING
                || task.get_state() == th::tasklet::FINISHED);

        CPPUNIT_ASSERT(task.wait());

        CPPUNIT_ASSERT(free_done && "An assertion failure here may indicate "
                    "an assertion failure in the bound function.");
    }


    void testTaskletError()
    {
        namespace th = fhtagn::threads;

        // Test that upon errors the task's state is ABORTED
        th::tasklet task(boost::bind(&throwing_free_func, _1));

        CPPUNIT_ASSERT(task.start());
        // The thread may already be ABORTED, but never FINISHED.
        CPPUNIT_ASSERT(task.get_state() == th::tasklet::RUNNING
                || task.get_state() == th::tasklet::ABORTED);

        CPPUNIT_ASSERT(task.stop());
        CPPUNIT_ASSERT(task.wait());
        CPPUNIT_ASSERT_EQUAL((int) th::tasklet::ABORTED, (int) task.get_state());

        CPPUNIT_ASSERT_EQUAL(false, free_error_handled);

        CPPUNIT_ASSERT(task.reset());

        // Same test again, but first we register the error handler.
        task.add_error_handler(boost::bind(&error_handler, _1, _2));

        CPPUNIT_ASSERT(task.start());
        // The thread may already be ABORTED, but never FINISHED.
        CPPUNIT_ASSERT(task.get_state() == th::tasklet::RUNNING
                || task.get_state() == th::tasklet::ABORTED);

        CPPUNIT_ASSERT(task.stop());
        CPPUNIT_ASSERT(task.wait());
        CPPUNIT_ASSERT_EQUAL((int) th::tasklet::ABORTED, (int) task.get_state());

        CPPUNIT_ASSERT_EQUAL(true, free_error_handled);
    }


    void testTaskletScope()
    {
        namespace th = fhtagn::threads;

        // Checks to determine whether tasklets that are destroyed before being
        // started, stopped or waited upon cause ugliness. These tests basically
        // only have to not segfault...

        // Unused task
        {
            th::tasklet task(boost::bind(&free_thread_func, _1));
        }

        // started task
        {
            th::tasklet task(boost::bind(&free_thread_func, _1));
            task.start();
        }

        // started & stopped task
        {
            th::tasklet task(boost::bind(&free_thread_func, _1));
            task.start();
            task.stop();
        }
    }


    void testMutexConcepts()
    {
        namespace th = fhtagn::threads;

        // Just need to compile...

        // Lockable concept
        boost::function_requires<th::concepts::Lockable<boost::mutex> >();
        boost::function_requires<th::concepts::Lockable<boost::try_mutex> >();
        boost::function_requires<th::concepts::Lockable<boost::timed_mutex> >();
        boost::function_requires<th::concepts::Lockable<boost::recursive_mutex> >();
        boost::function_requires<th::concepts::Lockable<boost::recursive_try_mutex> >();
        boost::function_requires<th::concepts::Lockable<boost::recursive_timed_mutex> >();
        boost::function_requires<th::concepts::Lockable<boost::shared_mutex> >();

        // TimedLockable concept
        boost::function_requires<th::concepts::TimedLockable<boost::timed_mutex> >();
        boost::function_requires<th::concepts::TimedLockable<boost::recursive_timed_mutex> >();
        // XXX shared_mutex supports absolute timed locking, but not relative
        // timed locking. Ugh.
        // boost::function_requires<th::concepts::TimedLockable<boost::shared_mutex> >();

        // SharedLockable concept
        boost::function_requires<th::concepts::SharedLockable<boost::shared_mutex> >();

        // UpgradeLockable concept
        boost::function_requires<th::concepts::UpgradeLockable<boost::shared_mutex> >();
    }


    void testMutexes()
    {
        namespace th = fhtagn::threads;

        // Needs to conform to all mutex concepts
        boost::function_requires<th::concepts::Lockable<th::pseudo_mutex> >();
        boost::function_requires<th::concepts::TimedLockable<th::pseudo_mutex> >();
        boost::function_requires<th::concepts::SharedLockable<th::pseudo_mutex> >();

        boost::function_requires<th::concepts::Lockable<th::fake_mutex> >();
        boost::function_requires<th::concepts::TimedLockable<th::fake_mutex> >();
        boost::function_requires<th::concepts::SharedLockable<th::fake_mutex> >();
        boost::function_requires<th::concepts::UpgradeLockable<th::fake_mutex> >();


        // Test test code first, by trying it out on boost's mutexes...
        {
          mutex_test<boost::mutex> m;
          m.test_lock_guard();
          m.test_unique_lock_nonrecursive();
        }

        {
          mutex_test<boost::try_mutex> m;
          m.test_lock_guard();
          m.test_unique_lock_nonrecursive();
        }

        {
          mutex_test<boost::timed_mutex> m;
          m.test_lock_guard();
          m.test_unique_lock_nonrecursive();
          m.test_timed_lock_nonrecursive();
        }

        {
          mutex_test<boost::recursive_mutex> m;
          m.test_lock_guard();
          m.test_unique_lock_recursive();
        }

        {
          mutex_test<boost::recursive_try_mutex> m;
          m.test_lock_guard();
          m.test_unique_lock_recursive();
        }

        {
          mutex_test<boost::recursive_timed_mutex> m;
          m.test_lock_guard();
          m.test_unique_lock_recursive();
          m.test_timed_lock_recursive();
        }

        {
          mutex_test<boost::shared_mutex> m;
          m.test_lock_guard();
          m.test_unique_lock_nonrecursive();
          m.test_timed_lock_nonrecursive();
          m.test_shared_lock();
          m.test_upgrade_lock();
        }

        // pseudo_mutex should survive most of non-recursive tests
        {
          mutex_test<th::pseudo_mutex> m;
          m.test_lock_guard();
          m.test_unique_lock_nonrecursive();
          m.test_timed_lock_nonrecursive();
          m.test_shared_lock();
        }

        // fake_mutex can only survive recursive tests
        {
          mutex_test<th::fake_mutex> m;
          m.test_lock_guard();
          m.test_unique_lock_recursive();
          m.test_timed_lock_recursive();
          m.test_shared_lock();
        }
    }


    void testFutures()
    {
      namespace th = fhtagn::threads;

      // immediate evaluation
      {
        th::future<fhtagn::size_t> f(&future_func);
        fhtagn::size_t x = f;
        CPPUNIT_ASSERT_EQUAL(fhtagn::size_t(42), x);
      }

      // lazy evaluation
      {
        th::future<fhtagn::size_t> f(&future_func, th::futures::lazy_evaluate());
        fhtagn::size_t x = f;
        CPPUNIT_ASSERT_EQUAL(fhtagn::size_t(42), x);
      }

      // future and exception
      {
        th::future<fhtagn::size_t> f(&throwing_future_func);
        fhtagn::size_t x = 0;
        CPPUNIT_ASSERT_THROW(x = f, th::futures::exception);
      }

      // copy futures
      {
        th::future<fhtagn::size_t> f1(&future_func);
        th::future<fhtagn::size_t> f2 = f1;
        fhtagn::size_t x = f1;
        CPPUNIT_ASSERT_EQUAL(fhtagn::size_t(42), x);
        CPPUNIT_ASSERT_EQUAL(f1, f2);
      }
    }
};


CPPUNIT_TEST_SUITE_REGISTRATION(ThreadsTest);
