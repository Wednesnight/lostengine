/**
 * $Id: pool_allocator.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_POOL_ALLOCATOR_H
#define FHTAGN_MEMORY_POOL_ALLOCATOR_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <fhtagn/shared_ptr.h>
#include <fhtagn/memory/memory_pool.h>

namespace fhtagn {
namespace memory {

/**
 * The pool allocator allocates objects from a preconstructed pool of memory.
 *
 * It may be desirable to use the same pool of memory for multiple types of
 * objects; in order to achieve that, but also retain the possibility of using
 * a different pool for different types, pool_allocators delegate all
 * allocation work to a MemoryPool class that is type-agnostic, but merely
 * handles bytes.
 *
 * During construction of pool_allocation_policy, the MemoryPool from which
 * objects are allocated is determined. If there is already a per-type
 * MemoryPool set, that is used. If not and there is a global MemoryPool set,
 * that is used. If neither is set, you must set the MemoryPool to be used by
 * this instance of pool_allocation_policy via a manual set_memory_pool() call.
 * If none of this happens, your application will crash.
 *
 * Note that once a pool_allocation_policy has constructed objects,
 * set_memory_pool() will fail until they are all destroyed again. Note also
 * that neither of the functions for setting global, per-type or per-instance
 * MemoryPools are thread-safe, and may conflict with any allocations or
 * deallocations made.
 *
 * It is therefore advisable to set the global and per-type MemoryPools (if
 * applicable) before any pool_allocation_policy objects may be created in other
 * threads. For per-instance MemoryPools, best make sure they're set before the
 * container ultimately using them is used.
 *
 * Please be aware that functions for setting memory pools are not thread-safe
 * by design - it's simply not advisable to set memory pools at any point
 * other than program startup.
 *
 * There is one problem with the constraints that STL imposes on allocators, as
 * users never explicitly instanciate them - that is, you can construct an STL
 * container to use a specific allocator class, but never to use a specific
 * allocator instance. For some MemoryPools (see e.g. fixed_pool.h), that is
 * unacceptable, as construction is non-trivial.
 *
 * So what are you to do if, for example, you want one vector of ints to use
 * one instance of a pool_allocation_policy<int>, and another to use a different
 * instance?
 *
 * The only way to achieve that is to turn this run-time difference into a
 * compile-time difference. For that purpose alone, pool_allocation_policy
 * receives a third template parameter, a tag type. It's completely unused, but
 * lets you easily create multiple different pool_allocation_policy types for
 * the same type of allocated object.
 *
 * Note also that there is no specific MemoryPool class; MemoryPool is a
 * concept specified in memory_pool.h
 *
 * In order to intialize static members, you must also invoke the
 * FHTAGN_POOL_ALLOCATION_INITIALIZE macro once in your calling code.
 **/
struct default_tag {};


template <
  typename memory_poolT,
  typename tagT
>
class pool_allocation_policy_base
{
public:
  BOOST_CLASS_REQUIRE(memory_poolT, ::fhtagn::memory::concepts,
      MemoryPoolConcept);

  /**
   * Convenience typedefs
   **/
  typedef memory_poolT                      memory_pool_t;
  typedef fhtagn::shared_ptr<memory_pool_t> memory_pool_ptr;

  /**
   * Global memory pool. While shared_ptr itself may be thread safe, access to
   * this pointer by pool_allocation_policy<T> may not be.
   **/
  static memory_pool_ptr  global_memory_pool;
};



template <
  typename T,
  typename memory_poolT = ::fhtagn::memory::heap_pool,
  typename tagT = ::fhtagn::memory::default_tag
>
class pool_allocation_policy
  : public pool_allocation_policy_base<memory_poolT, tagT>
{
public:
  /**
   * Typedefs aliased from base
   **/
  typedef typename pool_allocation_policy_base<memory_poolT, tagT>::memory_pool_t   memory_pool_t;
  typedef typename pool_allocation_policy_base<memory_poolT, tagT>::memory_pool_ptr memory_pool_ptr;

  /**
   * Typedefs - aliased by the allocator
   **/
  typedef T                   value_type;
  typedef value_type *        pointer;
  typedef value_type const *  const_pointer;
  typedef value_type &        reference;
  typedef value_type const &  const_reference;
  typedef fhtagn::size_t      size_type;
  typedef std::ptrdiff_t      difference_type;


  /**
   * Rebind to allocation policy for different type.
   **/
  template<typename U>
  struct rebind
  {
    typedef pool_allocation_policy<U> other;
  };


  /**
   * Constructors, destructor
   **/
  inline explicit pool_allocation_policy();
  inline ~pool_allocation_policy();

  inline explicit pool_allocation_policy(pool_allocation_policy const &);

  template <typename U>
  inline explicit pool_allocation_policy(pool_allocation_policy<U> const &);

  template <typename U, typename other_poolT>
  inline explicit pool_allocation_policy(pool_allocation_policy<U, other_poolT> const &);

  template <typename U, typename other_poolT, typename other_tagT>
  inline explicit pool_allocation_policy(pool_allocation_policy<U, other_poolT, other_tagT> const &);


  /**
   * Memory allocation functions.
   **/
  inline pointer allocate(size_type count,
      typename std::allocator<void>::const_pointer = 0);

  inline void deallocate(pointer p, size_type);


  /**
   * Determine size.
   **/
  inline size_type max_size() const;

  /**
   * Per-T memory pool. See global_memory_pool in parent class and class
   * documentation for information on thread-safety.
   **/
  static memory_pool_ptr  per_type_memory_pool;

  /**
   * Get the memory pool currently in use for this instance.
   **/
  inline memory_pool_ptr get_memory_pool() const;

  /**
   * Set the memory pool currently in use for this instance. Will return true
   * on success, else false. If the current memory pool has allocated space to
   * objects, this operation will fail.
   **/
  inline bool set_memory_pool(memory_pool_ptr pool);


private:
  // Sets m_pool to either the type pool or the global pool. If neither is set,
  // a std::logic_error is thrown.
  inline void initialize_pool();

  // The actuall memory pool currently used.
  memory_pool_ptr         m_pool;
};


/**
 * Equality and inequality comparison operators - used to detect if memory
 * allocated from one allocator can be deallocated from the current one.
 * Many variations again, as with the constructors.
 **/
template <
  typename T,
  typename memory_poolT,
  typename tagT
>
inline bool operator==(
    pool_allocation_policy<T, memory_poolT, tagT> const & rhs,
    pool_allocation_policy<T, memory_poolT, tagT> const & lhs);



template <
  typename T1,
  typename T2,
  typename memory_poolT,
  typename tagT
>
inline bool operator==(
    pool_allocation_policy<T1, memory_poolT, tagT> const & rhs,
    pool_allocation_policy<T2, memory_poolT, tagT> const & lhs);



template <
  typename T,
  typename memory_poolT,
  typename tagT,
  typename other_allocatorT
>
inline bool operator==(
    pool_allocation_policy<T, memory_poolT, tagT> const &,
    other_allocatorT const &);


}} // namespace fhtagn::memory


#define FHTAGN_POOL_ALLOCATION_INITIALIZE_BASE(POOL_T) \
  FHTAGN_POOL_ALLOCATION_INITIALIZE_BASE_TAG(POOL_T, fhtagn::memory::default_tag)

#define FHTAGN_POOL_ALLOCATION_INITIALIZE_BASE_TAG(POOL_T, TAG_T)     \
  namespace fhtagn { namespace memory {                               \
    template <>                                                       \
    pool_allocation_policy_base<POOL_T, TAG_T>::memory_pool_ptr       \
    pool_allocation_policy_base<POOL_T, TAG_T>::global_memory_pool =  \
    pool_allocation_policy_base<POOL_T, TAG_T>::memory_pool_ptr();    \
  }}

#define FHTAGN_POOL_ALLOCATION_INITIALIZE(T, POOL_T) \
  FHTAGN_POOL_ALLOCATION_INITIALIZE_TAG(T, POOL_T, fhtagn::memory::default_tag)

#define FHTAGN_POOL_ALLOCATION_INITIALIZE_TAG(T, POOL_T, TAG_T)       \
  namespace fhtagn { namespace memory {                               \
    template <>                                                       \
    pool_allocation_policy<T, POOL_T, TAG_T>::memory_pool_ptr         \
    pool_allocation_policy<T, POOL_T, TAG_T>::per_type_memory_pool =  \
    pool_allocation_policy<T, POOL_T, TAG_T>::memory_pool_ptr();      \
  }}


#include <fhtagn/memory/detail/pool_allocator.tcc>

#endif // guard
