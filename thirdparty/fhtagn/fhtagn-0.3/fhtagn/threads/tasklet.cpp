/**
 * $Id: tasklet.cpp 319 2009-06-08 22:17:47Z unwesen $
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

#include <fhtagn/threads/tasklet.h>


#define IS_ALIVE \
    (RUNNING == m_state || SLEEPING == m_state || STOPPED == m_state)


namespace fhtagn {
namespace threads {


tasklet::tasklet(tasklet::func_type::slot_type slot)
    : m_state(STANDING_BY)
    , m_thread(0)
{
    m_func.connect(slot);
}


tasklet::~tasklet()
{
    stop();
    wait();
}


bool
tasklet::start()
{
    boost::mutex::scoped_lock lock(m_mutex);
    if (m_thread) {
        return false;
    }
    m_state = RUNNING;
    m_thread = new boost::thread(boost::bind(&tasklet::thread_runner,
                this));
    return true;
}


bool
tasklet::stop()
{
    boost::mutex::scoped_lock lock(m_mutex);
    if (!m_thread) {
        return false;
    }
    if (IS_ALIVE) {
        m_state = STOPPED;
    }
    lock.unlock();

    m_finish.notify_all();
    return true;
}


bool
tasklet::wait()
{
    boost::mutex::scoped_lock lock(m_mutex);

    if (!m_thread) {
        return false;
    }

    while (IS_ALIVE) {
        m_finish.wait(lock);
    }

    // Copy m_thread in order to be able to join it without holding m_mutex
    boost::thread * join_thread = m_thread;
    m_thread = NULL;

    lock.unlock();

    join_thread->join();
    delete join_thread;
    return true;
}


bool
tasklet::reset()
{
    boost::mutex::scoped_lock lock(m_mutex);
    switch (m_state) {
      case STANDING_BY:
      case FINISHED:
      case ABORTED:
        m_state = STANDING_BY;
        return true;
        break;

      case RUNNING:
      case STOPPED:
      case SLEEPING:
        return false;
        break;

      default:
        throw std::logic_error("Unreachable line reached");
        break;
    }

    // unreachable
    return false;
}


tasklet::state
tasklet::get_state() const
{
    boost::mutex::scoped_lock lock(m_mutex);
    return m_state;
}



bool
tasklet::alive() const
{
    boost::mutex::scoped_lock lock(m_mutex);
    return IS_ALIVE;
}



tasklet::state
tasklet::sleep(boost::uint32_t usecs /* = 0 */)
{
    // Prepare xtime to sleep until
    boost::xtime t;
    t.sec = 0;
    t.nsec = 0;

    if (usecs) {
        // Add usecs to current time
        boost::xtime_get(&t, boost::TIME_UTC);
        t.sec += usecs / 1000000;
        t.nsec += (usecs % 1000000) * 1000;
        if (t.nsec >= 1000000000) {
            ++t.sec;
            t.nsec -= 1000000000;
        }
    }

    // Sleep until stopped or time runs out.
    boost::mutex::scoped_lock lock(m_mutex);
    if (m_state == RUNNING || m_state == SLEEPING) {
        // Save current state and switch to SLEEPING
        state prev_state = m_state;
        m_state = SLEEPING;

        // Perform the actual sleep. Gets interrupted either by a timeout, or
        // by a state change.
        while (m_state == SLEEPING) {
            if (usecs) {
                // sleep until timeout
                if (!m_finish.timed_wait(lock, t)) {
                    // Timeout! We don't care what the flag says, we have to
                    // exit right now.
                    break;
                }
            } else {
                // sleep indefinitely
                m_finish.wait(lock);
            }
        }

        // If the state's still SLEEPING, we exited the loop of natural causes,
        // and must restore the previous state.
        if (m_state == SLEEPING) {
            m_state = prev_state;
        }
    }
    return m_state;
}




void
tasklet::add_error_handler(error_func_type::slot_type slot)
{
    boost::mutex::scoped_lock lock(m_error_func_mutex);
    m_error_func.connect(slot);
}



void
tasklet::thread_runner()
{
    try {
        m_func(*this);
    } catch (std::exception const & ex) {
        {
            boost::mutex::scoped_lock lock(m_mutex);
            m_state = ABORTED;
        }
        try {
            // Pass any std::exception on to the error handler
            boost::mutex::scoped_lock lock(m_error_func_mutex);
            m_error_func(*this, ex);
        } catch (...) {
            // silently ignore
        }
    } catch (...) {
        {
            boost::mutex::scoped_lock lock(m_mutex);
            m_state = ABORTED;
        }
        try {
            // Pass a new runtime_error on to the error handler.
            std::runtime_error e("Unspecified exception occurred in tasklet's "
                    "bound function.");
            boost::mutex::scoped_lock lock(m_error_func_mutex);
            m_error_func(*this, e);
        } catch (...) {
            // silently ignore
        }
    }

    {
        boost::mutex::scoped_lock lock(m_mutex);
        if (IS_ALIVE) {
            m_state = FINISHED;
        }
    }

    m_finish.notify_all();
}


}} // namespace fhtagn::threads
