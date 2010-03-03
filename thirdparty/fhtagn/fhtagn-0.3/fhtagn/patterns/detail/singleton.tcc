/**
 * $Id: singleton.tcc 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_PATTERNS_SINGLETON_TCC
#define FHTAGN_PATTERNS_SINGLETON_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

namespace fhtagn {
namespace patterns {

/*****************************************************************************
 * default_creator
 **/
namespace detail {

template <typename T>
struct default_creator
{
  T * create()
  {
    return new T();
  }
};

} // namespace detail


/*****************************************************************************
 * singleton functions
 **/

template <typename wrappedT, typename creatorT, typename mutexT>
typename singleton<wrappedT, creatorT, mutexT>::shared_ptr
singleton<wrappedT, creatorT, mutexT>::instance()
{
  typename mutex_type::scoped_lock lock(m_mutex);

  if (!m_wrapped) {
    if (!m_creator) {
      m_creator = creator_shared_ptr(new creatorT());
    }

    m_wrapped = shared_ptr(m_creator->create());
  }

  return m_wrapped;
}



template <typename wrappedT, typename creatorT, typename mutexT>
void
singleton<wrappedT, creatorT, mutexT>::set_creator(creator_shared_ptr creator)
{
  typename mutex_type::scoped_lock lock(m_mutex);
  m_creator = creator;
}



}} // namespace fhtagn::patterns

#endif // guard
