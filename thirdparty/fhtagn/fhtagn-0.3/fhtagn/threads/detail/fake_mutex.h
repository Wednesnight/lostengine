/**
 * $Id: fake_mutex.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_THREADS_DETAIL_FAKE_MUTEX_H
#define FHTAGN_THREADS_DETAIL_FAKE_MUTEX_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <boost/noncopyable.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/locks.hpp>

namespace fhtagn {
namespace threads {

/**
 * Provides an interface like boost's mutexes, but does nothing at all. Since it
 * does nothing at all, it might as well provide all the lock types.
 *
 * Similar to recursive_mutex behaviour in a single thread.
 **/
class fake_mutex
    : private boost::noncopyable
{
public:
    typedef boost::unique_lock<fake_mutex> scoped_lock;
    typedef scoped_lock scoped_try_lock;
    typedef scoped_lock scoped_timed_lock;

    /*************************************************************************
     * Lockable implementation
     */
    void lock()
    {
    }


    bool try_lock()
    {
        return true;
    }

    void unlock()
    {
    }


    /*************************************************************************
     * TimedLockable implementation
     */
    bool timed_lock(boost::system_time const & abs_time)
    {
        return true;
    }


    template <typename time_durationT>
    bool timed_lock(time_durationT const & rel_time)
    {
        return true;
    }


    /*************************************************************************
     * SharedLockable implementation
     */
    void lock_shared()
    {
    }


    bool try_lock_shared()
    {
        return true;
    }


    void unlock_shared()
    {
    }


    bool timed_lock_shared(boost::system_time const & abs_time)
    {
        return true;
    }


    /*************************************************************************
     * UpgradeLockable implementation
     */
    void lock_upgrade()
    {
    }


    void unlock_upgrade()
    {
    }


    void unlock_upgrade_and_lock()
    {
    }


    void unlock_and_lock_upgrade()
    {
    }


    void unlock_upgrade_and_lock_shared()
    {
    }

};

}} // namespace fhtagn::threads

#endif // guard
