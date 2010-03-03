/**
 * $Id: defaults.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_DEFAULTS_H
#define FHTAGN_MEMORY_DEFAULTS_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

namespace fhtagn {
namespace memory {

/**
 * Default allocation policy, using new and delete. See allocator.h for details.
 **/
template <
  typename T
>
class default_allocation_policy
{
public:
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
    typedef default_allocation_policy<U> other;
  };


  /**
   * Constructors, destructor
   **/
  inline explicit default_allocation_policy();
  inline ~default_allocation_policy();

  inline explicit default_allocation_policy(default_allocation_policy const &);

  template <typename U>
  inline explicit default_allocation_policy(default_allocation_policy<U> const &);


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
};

/**
 * Equality and inequality comparison operators - used to detect if memory
 * allocated from one allocator can be deallocated from the current one.
 * Many variations again, as with the constructors.
 **/
template <
  typename T1,
  typename T2
>
inline bool operator==(
    default_allocation_policy<T1> const &,
    default_allocation_policy<T2> const &);



template <
  typename T,
  typename other_allocatorT
>
inline bool operator==(
    default_allocation_policy<T> const &, other_allocatorT const &);




/**
 * Default object traits. See allocator.h for details.
 *
 * You're likely to only use different traits if your class overloads the
 * address-of operator.
 **/
template <
  typename T
>
struct default_object_traits
{
  /**
   * Rebind to a object traits for a different type.
   **/
  template <typename U>
  struct rebind
  {
    typedef default_object_traits<U> other;
  };


  /**
   * Constructors, destructors.
   **/
  inline explicit default_object_traits();
  inline ~default_object_traits();

  template <typename U>
  inline explicit default_object_traits(default_object_traits<U> const &);


  /**
   * Address discovery
   **/
  inline T * address(T & t);
  inline T const * address(T const & t);


  /**
   * Construction/destruction
   **/
  inline void construct(T * p, const T & t);
  inline void destroy(T * p);
};

}} // namespace fhtagn::memory

#include <fhtagn/memory/detail/defaults.tcc>

#endif // guard
