/**
 * $Id: future.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_THREADS_FUTURE_H
#define FHTAGN_THREADS_FUTURE_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <string>
#include <exception>

#include <boost/signal.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

#include <fhtagn/shared_ptr.h>
#include <fhtagn/property.h>

namespace fhtagn {
namespace threads {

namespace futures {

/**
 * Constructor tag. @see future below
 **/
struct lazy_evaluate {};

/**
 * Exception for futures, may be thrown if a bound future function throws.
 **/
struct exception
  : public std::runtime_error
{
  explicit inline exception(std::string const & msg);
};

} // namespace futures


/**
 * The future class is a simple futures implementation. It's parametrized with
 * a (return) value type, which must be copy-constructible.
 *
 * A future object is constructed with a function object that is run in a
 * background thread to compute the value of the future. An optional tag
 * for lazy_evaluation can be provided, which launches the thread only when the
 * future's value is about to be read.
 *
 * Other than that, the future object acts as a read-only version of the
 * wrapped value type, i.e. all attempts to modify it's value will result in
 * compile-time failures.
 *
 * Reading the value will block until the thread used to calculate it finishes.
 * Once the thread is finished, the future's value can be read without further
 * delays.
 *
 * Note #1: futures are not designed to be thread-safe. That is, some thought
 *          into making them thread-safe had to be invested purely by virtue of
 *          them abstracting thread creation and destruction. But it would not
 *          be advisable to share a future instance between multiple threads.
 **/
template <
  typename return_valueT
>
class future
  : public fhtagn::property<
      return_valueT,
      future<return_valueT>,
      fhtagn::read_only_property
    >
{
private:
  // Utility typedefs
  typedef typename fhtagn::property<
    return_valueT,
    future<return_valueT>,
    fhtagn::read_only_property
  > property_t;

public:
  // Utility typedefs
  typedef boost::signal<return_valueT ()> func_type;


  /**
   * Constructor. Either construct with just a function, or with an additional
   * tag to signify lazy evaluation.
   **/
  inline future(typename func_type::slot_type slot);
  inline future(typename func_type::slot_type slot, futures::lazy_evaluate const &);

private:
  // Getter - see fhtagn/property.h for details
  inline return_valueT get() const;

  struct future_impl
  {
    inline future_impl();
    inline ~future_impl();

    // Spawns the calculating thread.
    inline void start_thread();

    // Helper function to call the bound function and set m_value or exception
    // text.
    inline void thread_runner();

    // Bound function
    func_type         m_func;
    // Internal thread in which the bound function is executed.
    boost::thread *   m_thread;
    // Condition to signal a change in the m_stopped flag.
    boost::condition  m_finish;
    // Mutex used with the condition variable above
    boost::mutex      m_mutex;

    // Result of the bound function. Pointer, so we can avoid default-
    // construction and (possible) save expensive setup.
    return_valueT *   m_value;
    // Optional exception message.
    std::string *     m_exception_message;
  };

  // By pimpl-ing the future, future objects become copyable - they've
  // got read-only semantics, so sharing an impl doesn't hurt.
  fhtagn::shared_ptr<future_impl> m_impl;
};


}} // namespace fhtagn::threads

#include <fhtagn/threads/detail/future.tcc>

#endif // guard
