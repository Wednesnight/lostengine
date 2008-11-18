/**
 * $Id: tasklet.cpp 189 2008-09-28 19:07:28Z unwesen $
 *
 * Copyright (C) 2008 the authors.
 *
 * Author: Jens Finkhaeuser <unwesen@users.sourceforge.net>
 *
 * This file is part of the Fhtagn! C++ Library, and may be distributed under
 * the following license terms:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 **/
#include <boost/bind.hpp>
#include <boost/thread/xtime.hpp>

#include <fhtagn/threads/tasklet.h>


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
    m_state = STOPPED;
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

    while (m_state == RUNNING || m_state == SLEEPING || m_state == STOPPED) {
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
    return (m_state == RUNNING || m_state == SLEEPING || m_state == STOPPED);
}



tasklet::state
tasklet::sleep(uint32_t usecs /* = 0 */)
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
        if (m_state != ABORTED) {
            m_state = FINISHED;
        }
    }

    m_finish.notify_all();
}


}} // namespace fhtagn::threads
