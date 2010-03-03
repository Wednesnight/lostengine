/**
 * $Id: future.tcc 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_THREADS_DETAIL_FUTURE_TCC
#define FHTAGN_THREADS_DETAIL_FUTURE_TCC


namespace fhtagn {
namespace threads {

/*****************************************************************************
 * futures::exception
 **/
namespace futures {

exception::exception(std::string const & msg)
  : std::runtime_error(msg)
{
}

} // namespace futures



/*****************************************************************************
 * future::future_impl
 **/
template <
  typename return_valueT
>
future<return_valueT>::future_impl::future_impl()
  : m_thread(NULL)
  , m_value(NULL)
  , m_exception_message(NULL)
{
}



template <
  typename return_valueT
>
future<return_valueT>::future_impl::~future_impl()
{
  if (m_thread) {
    m_thread->join();
    delete m_thread;
  }

  delete m_value;
  delete m_exception_message;
}



template <
  typename return_valueT
>
void
future<return_valueT>::future_impl::start_thread()
{
  m_thread = new boost::thread(boost::bind(&future_impl::thread_runner,
        this));
}



template <
  typename return_valueT
>
void
future<return_valueT>::future_impl::thread_runner()
{
  return_valueT * value = NULL;
  std::string * exception_message = NULL;
  try {
    value = new return_valueT(m_func());

  } catch (std::exception const & ex) {
    boost::mutex::scoped_lock l(m_mutex);
    exception_message = new std::string(ex.what());

  } catch (...) {
    boost::mutex::scoped_lock l(m_mutex);
    exception_message = new std::string("Unspecified exception occurred in "
        "bound function.");
  }

  // Store value and signal that we're done.
  boost::mutex::scoped_lock l(m_mutex);
  m_value = value;
  m_exception_message = exception_message;

  m_finish.notify_all();
}






/*****************************************************************************
 * future
 **/
template <
  typename return_valueT
>
future<return_valueT>::future(typename future::func_type::slot_type slot)
  : property_t(this, &future<return_valueT>::get)
  , m_impl(new future_impl())
{
  m_impl->m_func.connect(slot);
  m_impl->start_thread();
}



template <
  typename return_valueT
>
future<return_valueT>::future(typename future::func_type::slot_type slot,
    futures::lazy_evaluate const &)
  : property_t(this, &future::get)
  , m_impl(new future_impl())
{
  m_impl->m_func.connect(slot);
}



template <
  typename return_valueT
>
return_valueT
future<return_valueT>::get() const
{
  boost::mutex::scoped_lock l(m_impl->m_mutex);
  if (!(m_impl->m_value || m_impl->m_exception_message)) {
    // If there's no thread, start it. That happens with lazy evaluation.
    if (!m_impl->m_thread) {
      m_impl->start_thread();
    }

    // Wait for the thread to finish.
    while (!(m_impl->m_value || m_impl->m_exception_message)) {
      m_impl->m_finish.wait(l);
    }
  }

  // May need to throw an exception.
  if (m_impl->m_exception_message) {
    throw futures::exception(*m_impl->m_exception_message);
  }

  // Return value!
  return *m_impl->m_value;
}



}} // namespace fhtagn::threads


#endif // guard
