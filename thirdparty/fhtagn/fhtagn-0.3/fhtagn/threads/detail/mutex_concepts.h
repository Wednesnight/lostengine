/**
 * $Id: mutex_concepts.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_THREADS_DETAIL_MUTEX_CONCEPTS_H
#define FHTAGN_THREADS_DETAIL_MUTEX_CONCEPTS_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <boost/concept_check.hpp>

namespace fhtagn {
namespace threads {

namespace concepts {

/**
 * These mutex concepts are taken from the boost library documentation.
 **/

template <typename T>
struct Lockable
{
    void constraints()
    {
        lockable.lock();

        bool b = lockable.try_lock();
        boost::ignore_unused_variable_warning(b);

        lockable.unlock();
    }

    T & lockable;
};


template <typename T>
struct TimedLockable
{
    void constraints()
    {
        bool b = lockable.timed_lock(abs_time);
        boost::ignore_unused_variable_warning(b);

        b = lockable.timed_lock(rel_time);
        boost::ignore_unused_variable_warning(b);
    }

    T & lockable;
    boost::system_time const & abs_time;
    boost::posix_time::time_duration const & rel_time;
};


template <typename T>
struct SharedLockable
{
    void constraints()
    {
        lockable.lock_shared();

        bool b = lockable.try_lock_shared();
        boost::ignore_unused_variable_warning(b);

        lockable.unlock_shared();

        b = lockable.timed_lock_shared(abs_time);
        boost::ignore_unused_variable_warning(b);
    }

    T & lockable;
    boost::system_time const & abs_time;
};


template <typename T>
struct UpgradeLockable
{
    void constraints()
    {
        lockable.lock_upgrade();

        lockable.unlock_upgrade();

        lockable.unlock_upgrade_and_lock();

        lockable.unlock_and_lock_upgrade();

        lockable.unlock_upgrade_and_lock_shared();
    }

    T & lockable;
};


}}} // namespace fhtagn::threads::concepts

#endif // guard
