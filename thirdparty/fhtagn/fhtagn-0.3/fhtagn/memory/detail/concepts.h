/**
 * $Id: concepts.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_DETAIL_CONCEPTS_H
#define FHTAGN_MEMORY_DETAIL_CONCEPTS_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <boost/concept_check.hpp>

namespace fhtagn {
namespace memory {
namespace concepts {

/**
 * AllocationPolicyConcept tries to ensure that your custom allocation policies
 * follow the concept expected by the allocator class below.
 **/
template <
  typename T,
  typename policyT
>
struct AllocationPolicyConcept
{
  void constraints()
  {
    // policies must have a few subtypes
    typedef typename policyT::value_type      value_type;
    typedef typename policyT::pointer         pointer;
    typedef typename policyT::const_pointer   const_pointer;
    typedef typename policyT::reference       reference;
    typedef typename policyT::const_reference const_reference;
    typedef typename policyT::size_type       size_type;
    typedef typename policyT::difference_type difference_type;

    // check for rebind.
    typedef typename policyT::template rebind<int>::other rebind_check;

    // check for ctors. can't check the ctor from a policy for a different type,
    // though.
    policyT p1;
    policyT p2(p1);

    // check for allocate/deallocate
    p = p1.allocate(s);
    p = p1.allocate(s, reinterpret_cast<std::allocator<void>::const_pointer>(
          fhtagn::size_t(0xdeadbeef)));
    p1.deallocate(p, s);

    // check for max_size()
    s = p1.max_size();

    // Check for equality operator
    bool b = (p1 == p2);
    boost::ignore_unused_variable_warning(b);
  }

  typename policyT::pointer & p;
  typename policyT::size_type & s;
};


/**
 * Similarly, ObjectTraitsConcept tries to ensure that your custom object traits
 * provide the required functionality.
 **/
template <
  typename T,
  typename traitsT
>
struct ObjectTraitsConcept
{
  void constraints()
  {
    // check for rebind
    typedef typename traitsT::template rebind<int>::other rebind_check;

    // check for ctors. can't check the ctor from a policy for a different type,
    // though.
    traitsT t1;
    traitsT t2(t1);

    // check for address functions
    p1 = t1.address(t);
    p2 = t1.address(const_t);

    // construction/destruction
    t1.construct(p1, const_t);
    t1.destroy(p1);
  }


  T & t;
  T * p1;

  T const & const_t;
  T const * p2;
};

}}} // namespace fhtagn::memory::concepts

#endif // guard
