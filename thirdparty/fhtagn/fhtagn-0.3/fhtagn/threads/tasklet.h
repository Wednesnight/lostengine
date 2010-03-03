/**
 * $Id: tasklet.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_THREADS_TASKLET_H
#define FHTAGN_THREADS_TASKLET_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <exception>

#include <boost/signal.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/noncopyable.hpp>

namespace fhtagn {
namespace threads {


/**
 * The tasklet class is a simple wrapper around boost::thread that makes
 * manipulation of a thread a little easier.
 *
 * In particular, it adds several pieces of functionality over boost::thread:
 * 1) Start/stop/restart the tasklet without a need to bind a function to
 *    a boost::thread yet again. The tasklet is bound to the function to be
 *    run in a separate thread, and internally takes care of creating and
 *    tearing down boost::thread objects.
 * 2) Signal the tasklet to stop. The bound function is not interrupted, but
 *    a flag is set that the bound function can check to determine whether it
 *    should end prematurely.
 * 3) Sleep until the tasklet is stopped or some time elapsed. A common use-
 *    case for threads is to sleep most of the time, but wake up periodically
 *    to perform some task. This sleeping should be interruptible so the thread
 *    can be torn down when required. The flag & condition required to implement
 *    such functionality is abstracted out.
 **/
class tasklet
    : public boost::noncopyable
{
public:
    // Utility typedefs
    typedef boost::signal<void (tasklet &)> func_type;
    typedef boost::signal<void (tasklet &, std::exception const &)
        > error_func_type;

    /**
     * Enum that defines the states the tasklet can be in.
     **/
    enum state
    {
        STANDING_BY = 0,   /* Tasklet is not currently running the bound
                              function, so start() can safely be invoked. */
        RUNNING     = 1,   /* Tasklet is currently running the bound function.
                              You can't start() it, but you can stop() it. */
        SLEEPING    = 2,   /* The tasklet's bound function has invoked sleep(),
                              but not yet returned. */
        STOPPED     = 3,   /* The stop() function has been called, but the bound
                              function has not yet finished execution. */
        ABORTED     = 4,   /* Running the bound function resulted in an
                              exception, and has stopped. */
        FINISHED    = 5    /* The bound function has finished executing,
                              possibly because stop() was called. The tasklet is
                              ready for resetting. */
    };

    /**
     * Construct a tasklet instance that, when started, will execute the passed
     * function. The function prototype must be
     *
     *      void foo(tasklet &)
     *
     * It's perfectly fine to bind member functions via boost::bind here; in
     * fact, one type of usage might be to derive from tasklet in the following
     * manner:
     *
     *      struct Foo
     *          : public tasklet
     *      {
     *          Foo()
     *              : tasklet(boost::bind(&Foo::foo, boost::ref(this), _1))
     *          {
     *          }
     *
     *          void foo(tasklet & t)
     *          {
     *              // The t passed will be *this
     *          }
     *      };
     *
     * @param slot Function to execute.
     **/
    explicit tasklet(func_type::slot_type slot);

    /**
     * Virtual dtor to allow for derivation.
     **/
    virtual ~tasklet();


    /**
     * If !started(), will execute the bound function in a new thread and
     * return true. If started(), returns false.
     *
     * @return see above.
     **/
    bool start();

    /**
     * If started(), will set a stopped flag on the tasklet, which the bound
     * function can check via the stopped() function.
     *
     * @return true if started(), else false.
     **/
    bool stop();

    /**
     * Wait for the bound function to finish, and join the thread started
     * internally.
     *
     * @return true if started(), else false.
     **/
    bool wait();


    /**
     * Reset the tasklet's state to STANDING_BY, if possible. This isn't done
     * automatically, so the caller has ample opportunity to query the tasklet's
     * status.
     *
     * @return True if the reset was successful, else false. You may wish to
     *      query the current state before attempting to reset the tasklet.
     **/
    bool reset();


    /**
     * @return the current state of the tasklet, i.e. one of the values of
     *      the state enum.
     **/
    state get_state() const;


    /**
     * @return true if the current state is one of RUNNING, SLEEPING, STOPPED,
     *      else false.
     **/
    bool alive() const;


    /**
     * Waits for the specified number of microseconds, or until the tasklet
     * was stopped via stop(). This function is intended for use within the
     * bound function to put a tasklet to sleep and wake up periodically:
     *
     *      void foo(tasklet & t) {
     *          while (!t.sleep(1000000)) {
     *              // Wake up once a second and do something.
     *          }
     *      }
     *
     * @param usecs [optional] Number of microseconds to sleep. If no time is
     *      specified, the sleep is not limited to a certain amount of time,
     *      but can only be broken by a call to stop().
     * @return The current tasklet state.
     **/
    state sleep(boost::uint32_t usecs = 0);


    /**
     * Allows to set an error handler function for the tasklet that's invoked
     * if the bound function produces any exception derived from std::exception
     * (such as std::runtime_error, etc.).
     *
     * If any other exception is thrown, tasklet passes a std::runtime_error
     * to the error handler.
     *
     * The parameters for the error handler function are a tasklet reference
     * (this) and a reference to the caught exception.
     *
     * Note #1: You may set as many error handlers as you wish, they will all
     *          be invoked in the order they are registered here.
     *
     * Note #2: Any exceptions thrown from an error handler will be silently
     *          ignored.
     *
     * Note #3: Calling add_error_handler() from within any of the error handlers
     *          registered here will deadlock.
     *
     * @param slot The error handler function to invoke on errors.
     **/
    void add_error_handler(error_func_type::slot_type slot);

private:
    // Helper function to call the bound function and set m_done at the end.
    void thread_runner();

    // Bound function
    func_type               m_func;

    // Optional error handling function
    error_func_type         m_error_func;

    // State of the tasklet, can be queried via the get_state() function.
    state                   m_state;
    // Internal thread in which the bound function is executed.
    boost::thread *         m_thread;
    // Condition to signal a change in the m_stopped flag.
    boost::condition        m_finish;
    // Mutex to serialize access to flags and m_thread.
    mutable boost::mutex    m_mutex;
    // Mutex to serialize access to the error handler callback. It's a separate
    // mutex, unfortunately, because we don't want to block access to tasklet's
    // functions from the error handler.
    mutable boost::mutex    m_error_func_mutex;
};


}} // namespace fhtagn::threads

#endif // guard
