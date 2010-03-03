/**
 * $Id: allocator.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_ALLOCATOR_H
#define FHTAGN_MEMORY_ALLOCATOR_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <fhtagn/memory/detail/concepts.h>
#include <fhtagn/memory/defaults.h>

namespace fhtagn {
namespace memory {

/**
 * Following a 2003 article on memory allocators (which you can find at
 * http://www.codeproject.com/KB/cpp/allocator.aspx), this class defines an
 * STL-compatible memory allocator.
 *
 * It delegates most of it's work to an object traits type, and to a memory
 * allocation policy type, allowing for maximum extensibility.
 **/
template<
  typename T,
  typename allocation_policyT = default_allocation_policy<T>,
  typename object_traitsT = default_object_traits<T>
>
class allocator
  : public allocation_policyT
  , public object_traitsT
{
public:
  /**
   * Concept checks, to catch errors early on.
   **/
  BOOST_CLASS_REQUIRE2(T,
      allocation_policyT,
      ::fhtagn::memory::concepts,
      AllocationPolicyConcept);

  BOOST_CLASS_REQUIRE2(T,
      object_traitsT,
      ::fhtagn::memory::concepts,
      ObjectTraitsConcept);


  /**
   * Typedefs, aliased from allocation policy.
   **/
  typedef typename allocation_policyT::size_type size_type;
  typedef typename allocation_policyT::difference_type difference_type;
  typedef typename allocation_policyT::pointer pointer;
  typedef typename allocation_policyT::const_pointer const_pointer;
  typedef typename allocation_policyT::reference reference;
  typedef typename allocation_policyT::const_reference const_reference;
  typedef typename allocation_policyT::value_type value_type;

  /**
   * Rebind this allocator to one for a different type.
   **/
  template <typename U>
  struct rebind
  {
    typedef allocator<
      U,
      typename allocation_policyT::template rebind<U>::other,
      typename object_traitsT::template rebind<U>::other
    > other;
  };


  /**
   * Constructors, for default construction, for construction from an equivalent
   * allocator, from allocators for different types, or even from allocators
   * with different allocation policies/object traits.
   **/
  inline explicit allocator();
  inline ~allocator();

  inline allocator(allocator const & other);

  template <typename U>
  inline allocator(allocator<U> const &);

  template <typename U, typename other_policyT, typename other_traitsT>
  inline allocator(allocator<U, other_policyT, other_traitsT> const & other);
};


/**
 * Equality and inequality comparison operators - used to detect if memory
 * allocated from one allocator can be deallocated from the current one.
 * Many variations again, as with the constructors.
 **/
template <
  typename T,
  typename allocation_policyT,
  typename object_traitsT
>
inline bool operator==(
    allocator<T, allocation_policyT, object_traitsT> const & lhs,
    allocator<T, allocation_policyT, object_traitsT> const & rhs);



template <
  typename T1,
  typename allocation_policyT1,
  typename object_traitsT1,
  typename T2,
  typename allocation_policyT2,
  typename object_traitsT2
>
inline bool operator==(
    allocator<T1, allocation_policyT1, object_traitsT1> const & lhs,
    allocator<T2, allocation_policyT2, object_traitsT2> const & rhs);



template <
  typename T,
  typename allocation_policyT,
  typename object_traitsT,
  typename other_allocatorT
>
inline bool operator==(
    allocator<T, allocation_policyT, object_traitsT> const & lhs,
    other_allocatorT const & rhs);



template <
  typename T,
  typename allocation_policyT,
  typename object_traitsT
>
inline bool operator!=(
    allocator<T, allocation_policyT, object_traitsT> const & lhs,
    allocator<T, allocation_policyT, object_traitsT> const & rhs);



template <
  typename T1,
  typename allocation_policyT1,
  typename object_traitsT1,
  typename T2,
  typename allocation_policyT2,
  typename object_traitsT2
>
inline bool operator!=(
    allocator<T1, allocation_policyT1, object_traitsT1> const & lhs,
    allocator<T2, allocation_policyT2, object_traitsT2> const & rhs);



template <
  typename T,
  typename allocation_policyT,
  typename object_traitsT,
  typename other_allocatorT
>
inline bool operator!=(
    allocator<T, allocation_policyT, object_traitsT> const & lhs,
    other_allocatorT const & rhs);


}} // namespace fhtagn::memory

#include <fhtagn/memory/detail/allocator.tcc>

#endif // guard
