/**
 * $Id: pseudo_mutex.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_THREADS_DETAIL_PSEUDO_MUTEX_H
#define FHTAGN_THREADS_DETAIL_PSEUDO_MUTEX_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <boost/noncopyable.hpp>
#include <boost/thread/xtime.hpp>

namespace fhtagn {
namespace threads {

/**
 * Provides an interface like boost's mutexes, but does next to nothing.
 *
 * Similar to mutex behaviour in a single thread.
 **/
class pseudo_mutex
    : private boost::noncopyable
{
public:
    typedef boost::unique_lock<pseudo_mutex> scoped_lock;
    typedef scoped_lock scoped_try_lock;
    typedef scoped_lock scoped_timed_lock;


    pseudo_mutex()
        : m_locked(false)
    {
    }


    /*************************************************************************
     * Lockable implementation
     */
    void lock()
    {
        m_locked = true;
    }


    bool try_lock()
    {
        if (m_locked) {
            return false;
        }

        m_locked = true;
        return true;
    }

    void unlock()
    {
        m_locked = false;
    }


    /*************************************************************************
     * TimedLockable implementation
     */
    bool timed_lock(boost::system_time const & abs_time)
    {
        return try_lock();
    }


    template <typename time_durationT>
    bool timed_lock(time_durationT const & rel_time)
    {
        return try_lock();
    }


    /*************************************************************************
     * SharedLockable implementation
     */
    void lock_shared()
    {
        lock();
    }


    bool try_lock_shared()
    {
        return try_lock();
    }


    void unlock_shared()
    {
        unlock();
    }


    bool timed_lock_shared(boost::system_time const & abs_time)
    {
        return try_lock();
    }


private:
    bool m_locked;
};

}} // namespace fhtagn::threads

#endif // guard
