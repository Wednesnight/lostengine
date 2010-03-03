/**
 * $Id: allocspeed.cpp 319 2009-06-08 22:17:47Z unwesen $
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

#include <stdlib.h>

#include <iostream>
#include <vector>

#include <fhtagn/memory/allocator.h>
#include <fhtagn/memory/pool_allocator.h>
#include <fhtagn/memory/memory_pool.h>
#include <fhtagn/memory/size_based_pool.h>

#include <fhtagn/util/stopwatch.h>

#include <boost/program_options.hpp>

#ifdef _WIN32
#define random_func   rand
#define srandom_func  srand
#else
#define random_func   random
#define srandom_func  srandom
#endif

namespace mem = fhtagn::memory;


// A test type for larger memory blocks. The value array stays uninitalized -
// it's only purpose is to represent memory of a certain size.
template <
  boost::uint32_t SIZE
>
struct test_value
{
  enum {
    ARRAY_SIZE = SIZE / sizeof(boost::uint32_t),
  };

  test_value()
  {
  }

  explicit test_value(boost::uint32_t int_value)
  {
  }

  boost::uint32_t value[ARRAY_SIZE];
};


typedef mem::size_based_pool<256, 1, 32>  size_based_pool_t;


FHTAGN_POOL_ALLOCATION_INITIALIZE_BASE(mem::heap_pool);

FHTAGN_POOL_ALLOCATION_INITIALIZE(boost::uint32_t, mem::heap_pool);
FHTAGN_POOL_ALLOCATION_INITIALIZE(boost::uint64_t, mem::heap_pool);
FHTAGN_POOL_ALLOCATION_INITIALIZE(test_value<16>, mem::heap_pool);
FHTAGN_POOL_ALLOCATION_INITIALIZE(test_value<32>, mem::heap_pool);
FHTAGN_POOL_ALLOCATION_INITIALIZE(test_value<64>, mem::heap_pool);
FHTAGN_POOL_ALLOCATION_INITIALIZE(test_value<128>, mem::heap_pool);

FHTAGN_POOL_ALLOCATION_INITIALIZE_BASE(size_based_pool_t);

FHTAGN_POOL_ALLOCATION_INITIALIZE(boost::uint32_t, size_based_pool_t);
FHTAGN_POOL_ALLOCATION_INITIALIZE(boost::uint64_t, size_based_pool_t);
FHTAGN_POOL_ALLOCATION_INITIALIZE(test_value<16>, size_based_pool_t);
FHTAGN_POOL_ALLOCATION_INITIALIZE(test_value<32>, size_based_pool_t);
FHTAGN_POOL_ALLOCATION_INITIALIZE(test_value<64>, size_based_pool_t);
FHTAGN_POOL_ALLOCATION_INITIALIZE(test_value<128>, size_based_pool_t);


#define PRINT_STOPWATCH_TIMES(times)                                \
  std::cout << "wall: " << times.get<0>() << " usec     \t"         \
               "sys:  " << times.get<1>() << " usec     \t"         \
               "user: " << times.get<2>() << " usec" << std::endl;


// With the randomly generated numbers, we establish a pattern of allocs/frees,
// or rather inserts and erases from the test vector. To make this reproducible,
// we explicitly seed the RNG.
unsigned long g_seed = 0xdeadbeef;

// Then, we pre-populate a list of allocs/erases. Odd-indexed entries denote
// the number of allocations we're making, even-indexed entries denote the
// number of erases we're making.
std::vector<boost::uint32_t> g_actions;

// For allocations, we'll set the values we're adding to values from the random
// pool. For erases, we'll take values from the random tool % the size of the
// test vector, and erase the entry at that index.
std::vector<boost::uint32_t> g_random_pool;

// Both g_action and g_random_pool are filled from this function. The size of
// g_action is determined by the num_actions parameter, and g_random_pool is
// filled to satsify the requirements of g_actions entries.
void initRandomPool(boost::uint32_t num_actions, boost::uint32_t max_items_per_action,
    bool verbose)
{
  if (verbose) {
    std::cout << "Initializing random pool..." << std::endl;
  }

  fhtagn::util::stopwatch sw;

  // Initialize RNG. We always want the same numbers.
  srandom_func(g_seed);

  g_actions.clear();
  g_random_pool.clear();

  for (boost::uint32_t i = 0 ; i < num_actions ; ++i) {
    // Determine the number of items for this action.
    boost::uint32_t items = random_func() % max_items_per_action;
    g_actions.push_back(items);

    // Fill random pool with enough numbers for this action.
    for (boost::uint32_t j = 0 ; j < items ; ++j) {
      g_random_pool.push_back(random_func());
    }
  }

  fhtagn::util::stopwatch::times_t times = sw.get_times();
  if (verbose) {
    PRINT_STOPWATCH_TIMES(times);
    std::cout << std::endl;
  }
}



template <
  typename valueT,
  typename allocatorT
>
void testAllocator(bool verbose)
{
  if (verbose) {
    std::cout << "Running tests..." << std::endl;
  }

  fhtagn::util::stopwatch sw;

  typedef std::vector<valueT, allocatorT> test_vector_t;
  test_vector_t test_vector;

  // Alternately fill the test_vector and erase stuff from it.
  boost::uint32_t count = 0;
  std::vector<boost::uint32_t>::const_iterator action_end = g_actions.end();
  for (std::vector<boost::uint32_t>::const_iterator action_iter = g_actions.begin()
      ; action_iter != action_end ; ++action_iter, ++count)
  {
    if (count % 2) {
      // Free stuff!
      for (boost::uint32_t i = 0 ; i < *action_iter ; ++i) {
        if (test_vector.empty()) {
          break;
        }

        boost::uint32_t index = g_random_pool.back() % test_vector.size();
        g_random_pool.pop_back();

        typename test_vector_t::iterator iter = test_vector.begin();
        iter += index;
        test_vector.erase(iter);
      }
    }
    else {
      // Allocate stuff!
      for (boost::uint32_t i = 0 ; i < *action_iter ; ++i) {
        boost::uint32_t random_value = g_random_pool.back();
        g_random_pool.pop_back();

        valueT value(random_value);
        test_vector.push_back(value);
      }
    }
  }

  fhtagn::util::stopwatch::times_t times = sw.get_times();
  PRINT_STOPWATCH_TIMES(times);
}


template <
  typename valueT
>
inline void
runTests(std::string const & alloc, bool verbose)
{

  if (alloc == "std") {
    testAllocator<valueT, std::allocator<valueT> >(verbose);
  }
  else if (alloc == "heap") {
    typedef mem::allocator<
      valueT,
      mem::pool_allocation_policy<
        valueT,
        mem::heap_pool
      >
    > heap_allocator_t;

    heap_allocator_t::global_memory_pool = typename heap_allocator_t::memory_pool_ptr(
        new mem::heap_pool());

    testAllocator<valueT, heap_allocator_t>(verbose);
  }
  else if (alloc == "size") {
    typedef mem::pool_allocation_policy<
      valueT,
      size_based_pool_t
    > alloc_policy_t;
    typedef mem::allocator<
      valueT,
      alloc_policy_t
    > size_based_allocator_t;

    alloc_policy_t::global_memory_pool = typename alloc_policy_t::memory_pool_ptr(
        new size_based_pool_t());

    testAllocator<valueT, size_based_allocator_t>(verbose);
  }
  else {
    std::cout << "Invalid allocator" << std::endl;
  }
}



inline void
runTests(std::string const & alloc, boost::uint32_t value_size, bool verbose)
{
  switch (value_size) {
    case 4:
      runTests<boost::uint32_t>(alloc, verbose);
      break;

    case 8:
      runTests<boost::uint64_t>(alloc, verbose);
      break;

    case 16:
      runTests<test_value<16> >(alloc, verbose);
      break;

    case 32:
      runTests<test_value<32> >(alloc, verbose);
      break;

    case 64:
      runTests<test_value<64> >(alloc, verbose);
      break;

    case 128:
      runTests<test_value<128> >(alloc, verbose);
      break;

    default:
      std::cout << "Invalid value size" << std::endl;
      break;
  }
}


template <
  typename allocatorT
>
inline void
runTests(boost::uint32_t value_size)
{
  std::cout << "woot!" << std::endl;
}



int main(int argc, char **argv)
{
  namespace po = boost::program_options;

  po::options_description desc(
    "Allocator speed tests.\n\n"
    "This test times the speed of allocations and deallocations of different\n"
    "allocators. Tests alternately fill and drain an STL vector of a certain\n"
    "value type, and wall time, system time and user time is measured from the\n"
    "start of the test to the end.\n\n"
    "You can influence the following parameters:\n"
    " - The allocator used. Possible allocators are\n"
    "   1. std::allocator\n"
    "   2. Fhtagn's allocator with a heap_pool\n"
    "   3. Fhtagn's allocator with a size_based_pool, with pools for objects of\n"
    "      size 1 to 32 - that is, objects of size 64 or 128 will be allocated.\n"
    "      from the heap.\n"
    " - The size of the value type.\n"
    " - The number of fill and drain cycles for the test, e.g. a value of 10\n"
    "   would indicate 10 fill cycles alternating with 10 drain cycles.\n"
    " - The maximum number of fills and drains per cycle. The actual number of\n"
    "   fills and drains is semi-random per cycle. (Note: a RNG is used, but\n"
    "   it's seeded to a compiled-in value.)\n\n"
    "Command line arguments"
  );

  std::string allocator;
  boost::uint32_t num_cycles = 0;
  boost::uint32_t items_per_cycle = 0;
  boost::uint32_t value_size = 0;
  bool verbose = true;

  desc.add_options()
    ("help", "Prints this help text and exits.")
    ("allocator", po::value<std::string>(&allocator)->default_value("std"),
        "Allocator used. Possible values are 'std' (referring to "
        "std::allocator), 'heap' (referring to Fhtagn's heap_pool) or 'size' ("
        "referring to Fhtagn's size_based_pool)")
    ("num_cycles", po::value<boost::uint32_t>(&num_cycles)->default_value(100),
        "Number of fill/drain cycles.")
    ("max_items", po::value<boost::uint32_t>(&items_per_cycle)->default_value(10000),
        "Maximum number of fills/drains per cycle.")
    ("value_size", po::value<boost::uint32_t>(&value_size)->default_value(4),
        "Size of the value type in bytes. Possible values are 4, 8, 16, 32, 64,"
        " 128.")
    ("verbose", po::value<bool>(&verbose)->default_value(true),
        "Be verbose about the output (=1), or only display the results (=0).")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }

  if (verbose) {
    std::cout << "Settings: " << std::endl
              << "  allocator:  " << allocator << std::endl
              << "  num_cycles: " << num_cycles << std::endl
              << "  max_items:  " << items_per_cycle << std::endl
              << "  value_size: " << value_size << std::endl << std::endl;
  }

  initRandomPool(num_cycles, items_per_cycle, verbose);
  runTests(allocator, value_size, verbose);
}
