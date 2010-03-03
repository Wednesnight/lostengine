/**
 * $Id: lock_policy.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_THREADS_LOCK_POLICY_H
#define FHTAGN_THREADS_LOCK_POLICY_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

/**
 * Lock policies are classes injected into an algorithm, container or other
 * piece of code at compile time that the code delegates to in order to perform
 * mutual exclusion. The most prominent reason for factoring out the policy is
 * to avoid incurring the overhead of locking or unlocking mutexes when the code
 * in question is intended to be used in a single-threaded environment.
 *
 * An example of such code making use of lock policies might look like this:
 *
 *    template <typename lock_policyT>
 *    void foo(...)
 *    {
 *        // code before critical section
 *
 *        // enter critical section
 *        lock_policyT::lock();
 *
 *        // code in critical section
 *
 *        // leave critical section
 *        lock_policyT::unlock();
 *
 *        // code after critical section
 *    }
 *
 * It's up to the lock_policyT to determine whether or not the lock()/unlock()
 * functions perform any mutex locking, or are no-ops.
 *
 * Fortunately, boost::thread already provides a nicely unified interface for
 * the mutexes and locks it contains. The basic pattern is this:
 *
 *    * Each mutex class contains at least an embedded type scoped_lock
 *    * Those mutex classes that support lock types that may fail at locking
 *      support additional embedded types, which are scoped_try_lock and
 *      scoped_timed_lock at the time of writing.
 *
 * In essence, the above code snippet can easily rewritten to accomodate either
 * recursive or non-recursive boost mutexes in the following manner:
 *
 *    template <typename mutexT>
 *    void foo(...)
 *    {
 *        // code before critical section
 *
 *        // enter critical section
 *        {
 *            static mutexT m;
 *            typename mutexT::scoped_lock l(m);
 *
 *            // code in critical section
 *
 *            // leave critical section
 *        }
 *
 *        // code after critical section
 *    }
 *
 * In other words, boost's mutex interface is almost good enough to implement
 * code with lock policies, except for two things:
 *
 *    1. Boost does *not* provide a mutex type that does nothing, i.e. all of
 *       it's lock types incur some overhead. We remedy that by providing a
 *       mutex types with the same interface as boost's mutexes.
 *
 *       Actually, we provide more than one mutex type. The first one,
 *       fake_mutex, really does nothing at all. It implements the interfaces of
 *       Lockable, TimedLockable, SharedLockable and UpgradeLockable, but all of
 *       it's functions will *always* succeed.
 *
 *       That is probably what is best when designing an algorithm to work both
 *       in a threaded and non-threaded environment.
 **/
#include <fhtagn/threads/detail/fake_mutex.h>
/**
 *       On the other hand, code that relies on specific *behaviour* of a mutex
 *       class, such as try_lock() succeeding or failing depending on whether
 *       the mutex is already locked, would break.
 *
 *       To deal with those situations, we provide pseudo_mutex. It implements
 *       the Lockable, TimedLockable and SharedLockable interfaces, and
 *       actually flags whether or not it's locked. But it's not thread-safe,
 *       and will pretty much only work in a single thread.
 *
 **/
#include <fhtagn/threads/detail/pseudo_mutex.h>
 /**
  *      Another way of thinking of these mutexes is that in a single thread,
  *      fake_mutex exhibits the same behaviour as a recursive_mutex, whereas
  *      pseudo_mutex exhibits that of a mutex.
  *
  *      XXX None of the mutexes simulate the UpgradeLockable behaviour.
  **/

/**
 *    2. We'd like to know as early as possible and as quickly as possible
 *       whether or not our selected mutex type works with the expectations of
 *       the algorithm making use of it. In particular, we'd like to know
 *       whether the mutex type provides the scoped_try_lock or
 *       scoped_timed_lock types. To help out in that area, we provide a few
 *       pre-defined boost::concept definitions that one can check for in code
 *       making use of lock policies.
 **/
#include <fhtagn/threads/detail/mutex_concepts.h>


#endif // guard
