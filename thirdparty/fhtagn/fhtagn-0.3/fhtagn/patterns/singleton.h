/**
 * $Id: singleton.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_PATTERNS_SINGLETON_H
#define FHTAGN_PATTERNS_SINGLETON_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif


#include <fhtagn/fhtagn.h>

#include <fhtagn/shared_ptr.h>
#include <fhtagn/threads/lock_policy.h>

namespace fhtagn {
namespace patterns {

/**
 * The Singleton class implements the singleton pattern, as an add-on
 * functionality to existing classes. This style implementation was chosen
 * because in many cases, a singleton is not the best implementation choice
 * - by allowing this functionality to be added onto an existing class, the
 * decision whether or not the singleton pattern should be used is left up
 * to the user of a class.
 *
 * In most cases, adding singleton functionality to an existing class Foo
 * is a matter of a simple typedef:
 *
 *   typedef fhtagn::patterns::Singleton<Foo> FooSingleton;
 *
 * FooSingleton then provides an instance-method returning a shared pointer
 * to the Foo singleton instance:
 *
 *   FooSingleton::shared_ptr f = FooSingleton::instance();
 *
 * In one of your source (non-header) files, also 
 *
 * And that's all you need to know for simple use-cases.
 *
 * There are two additional pieces of functionality which may be of interest,
 * though:
 *  1. The above assumes Foo provides a default constructor. If that's not
 *     the case, you can pass a second template parameter to Singleton
 *     specifying a creator class for Foo.
 *
 *       struct FooCreator
 *       {
 *         Foo * create(); // implement in any way you see fit.
 *       };
 *
 *       typedef fhtagn::patterns::Singleton<Foo, FooCreator> FooSingleton;
 *
 *     The only restriction placed on FooCreator is that it must itself be
 *     default-constructible. If you do not wish FooCreator to be default-
 *     constructed, you can set a hand-costruncted creator instance like this:
 *
 *       FooSingleton::creator_shared_ptr f(new FooCreator(...));
 *       FooSingleton::set_creator(f);
 *
 *    The FooCreator instance will be destructed at program exit, provided its
 *    reference count hits zero.
 *
 *    The path via a creator is a tad more complex than e.g. allowing calling
 *    code to set the Foo singleton instance manually. There are two problems
 *    with this alternative, though, which a creator avoids:
 *
 *    a) This code should not require Foo to provide a default constructor
 *       just in case calling code wants to use it as a singleton.
 *       The creator, on the other hand, exists in code that already makes the
 *       assumption that Foo will be used as a singleton, and can therefore
 *       accomodate the demands of the Singleton class. It's glue code that
 *       exists for the sole purpose of making Foo work as a Singleton.
 *    b) Setting a Foo instance manually would require Foo to be created whether
 *       or not Foo is ever used - and the same applies for the creator. The
 *       difference is that Foo is a functional class that might be expensive
 *       to create. The creator, on the other hand, is thin glue by design.
 *
 *    For those two reasons, it's more appropriate to allow explicit setting of
 *    a creator instance and impose restrictions on such a creator, rather than
 *    allow explicit setting of the singleton instance itself.
 *
 * 2. Simiar to the creator template parameter, you can specify a third
 *    template parameter to customize the locking policy. By default, no locking
 *    is performed.
 **/

/**
 * The default creator for singletons. Assumes the class to be turned into a
 * singleton provides a default constructor.
 **/
namespace detail {

template <typename T>
struct default_creator;

} // namespace detail


/**
 * Macro for creating static instances. Use this only once in your code.
 **/
#define FHTAGN_SINGLETON_INITIALIZE \
  template <typename wrappedT, typename creatorT, typename mutexT>                      \
  typename fhtagn::patterns::singleton<wrappedT, creatorT, mutexT>::mutex_type          \
  fhtagn::patterns::singleton<wrappedT, creatorT, mutexT>::m_mutex;                     \
                                                                                        \
  template <typename wrappedT, typename creatorT, typename mutexT>                      \
  typename fhtagn::patterns::singleton<wrappedT, creatorT, mutexT>::shared_ptr          \
  fhtagn::patterns::singleton<wrappedT, creatorT, mutexT>::m_wrapped;                   \
                                                                                        \
  template <typename wrappedT, typename creatorT, typename mutexT>                      \
  typename fhtagn::patterns::singleton<wrappedT, creatorT, mutexT>::creator_shared_ptr  \
  fhtagn::patterns::singleton<wrappedT, creatorT, mutexT>::m_creator;


/**
 * Singleton pattern implementation, see description above.
 **/
template <
  typename wrappedT,
  typename creatorT = detail::default_creator<wrappedT>,
  typename mutexT = fhtagn::threads::fake_mutex
>
class singleton : virtual public wrappedT
{
public:
  /**
   * Embedded types.
   **/
  typedef fhtagn::shared_ptr<wrappedT> shared_ptr;
  typedef fhtagn::shared_ptr<creatorT> creator_shared_ptr;
  typedef mutexT mutex_type;

  /**
   * Main interface: returns the singleton instance, optionally using the
   * creator to create one.
   **/
  static shared_ptr instance();

  /**
   * Optional interface: use the provided creator instance rather than a
   * default-constructed one. This function will not have any effect once
   * instance() above has been called.
   **/
  static void set_creator(creator_shared_ptr creator);

private:

  static mutexT             m_mutex;
  static shared_ptr         m_wrapped;
  static creator_shared_ptr m_creator;
};



//template <typename T, typename U>
//typename Singleton<T, U>::shared_ptr
//Singleton<T, U>::t;
//
//template <typename T, typename U>
//U *
//Singleton<T, U>::create = 0;

}} // namespace fhtagn::patterns

#include <fhtagn/patterns/detail/singleton.tcc>

#endif // guard
