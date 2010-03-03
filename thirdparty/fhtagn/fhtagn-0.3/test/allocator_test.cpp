/**
 * $Id: allocator_test.cpp 319 2009-06-08 22:17:47Z unwesen $
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

#include <vector>

#include <cppunit/extensions/HelperMacros.h>

#include <fhtagn/memory/allocator.h>
#include <fhtagn/memory/memory_pool.h>
#include <fhtagn/memory/fixed_pool.h>
#include <fhtagn/memory/pool_allocator.h>
#include <fhtagn/memory/throw_pool.h>
#include <fhtagn/memory/block_pool.h>
#include <fhtagn/memory/dynamic_pool.h>
#include <fhtagn/memory/size_based_pool.h>


typedef boost::uint32_t test_int_t;

typedef fhtagn::memory::dynamic_pool<
  fhtagn::memory::fixed_pool<>,
  1024
> dynamic_pool_t;



FHTAGN_POOL_ALLOCATION_INITIALIZE_BASE(fhtagn::memory::heap_pool);
FHTAGN_POOL_ALLOCATION_INITIALIZE_BASE(fhtagn::memory::fixed_pool<>);
FHTAGN_POOL_ALLOCATION_INITIALIZE_BASE(dynamic_pool_t);
FHTAGN_POOL_ALLOCATION_INITIALIZE_BASE(fhtagn::memory::size_based_pool<>);

FHTAGN_POOL_ALLOCATION_INITIALIZE(test_int_t, fhtagn::memory::heap_pool);
FHTAGN_POOL_ALLOCATION_INITIALIZE(test_int_t, fhtagn::memory::fixed_pool<>);
FHTAGN_POOL_ALLOCATION_INITIALIZE(test_int_t, dynamic_pool_t);
FHTAGN_POOL_ALLOCATION_INITIALIZE(test_int_t, fhtagn::memory::size_based_pool<>);


class AllocatorTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(AllocatorTest);

      CPPUNIT_TEST(testBlockAlignment);

      CPPUNIT_TEST(testHeapMemoryPool);
      CPPUNIT_TEST(testFixedMemoryPool);
      CPPUNIT_TEST(testFixedPoolFragmentation);
      CPPUNIT_TEST(testBlockMemoryPool);
      CPPUNIT_TEST(testDynamicMemoryPool);
      CPPUNIT_TEST(testThrowPool);
      CPPUNIT_TEST(testSizeBasedMemoryPool);

      CPPUNIT_TEST(testDefaultAllocator);
      CPPUNIT_TEST(testHeapPoolAllocator);
      CPPUNIT_TEST(testFixedPoolAllocator);
      CPPUNIT_TEST(testDynamicPoolAllocator);
      CPPUNIT_TEST(testSizeBasedPoolAllocator);

    CPPUNIT_TEST_SUITE_END();
private:

    void testBlockAlignment()
    {
      namespace mem = fhtagn::memory;

      char memory[200];
      char * mem_offset = memory + 1;

      // Test default block alignment. Given that memory allocated on the stack
      // or heap should always be aligned properly, that size_t is the same
      // size as a pointer, and that the default alignment assumes a block size
      // of sizeof(size_t), this really should not have *any* effect
      // whatsoever.
      CPPUNIT_ASSERT_EQUAL(static_cast<void *>(memory),
          mem::block_alignment<>::adjust_pointer(memory));

      // However, testing the offset memory pointer should indeed have an
      // effect.
      CPPUNIT_ASSERT(static_cast<void *>(mem_offset) !=
          mem::block_alignment<>::adjust_pointer(mem_offset));

      // If we're using a block size of sizeof(char), though, things shouldn't
      // matter either way.
      CPPUNIT_ASSERT_EQUAL(static_cast<void *>(memory),
          mem::block_alignment<sizeof(char)>::adjust_pointer(memory));
      CPPUNIT_ASSERT_EQUAL(static_cast<void *>(mem_offset),
          mem::block_alignment<sizeof(char)>::adjust_pointer(mem_offset));
    }



    template <
      typename poolT
    >
    void testMemoryPoolGeneric(poolT & pool)
    {
      // Technically, these tests do not always have to succeed - but they're
      // the most basic memory pool tests and *should* always succeed on properly
      // set up pools.

      char * p = static_cast<char *>(pool.alloc(42));
      CPPUNIT_ASSERT(p);
      CPPUNIT_ASSERT_EQUAL(true, pool.in_use());
      ::memset(p, 0x0f, 42);

      p = static_cast<char *>(pool.realloc(p, 666));
      CPPUNIT_ASSERT(p);
      CPPUNIT_ASSERT_EQUAL(true, pool.in_use());
      for (int i = 0 ; i < 42 ; ++i) {
        CPPUNIT_ASSERT_EQUAL(char(0x0f), *(p + i));
      }
      ::memset(p, 0xf0, 666);

      p = static_cast<char *>(pool.realloc(p, 123));
      CPPUNIT_ASSERT(p);
      CPPUNIT_ASSERT_EQUAL(true, pool.in_use());
      for (int i = 0 ; i < 123 ; ++i) {
        CPPUNIT_ASSERT_EQUAL(char(0xf0), *(p + i));
      }

      // After splitting 123 of a 666 size'd segment, the next allocation of, say
      // 200 bytes should be right behind the 123. If we then memset that, make
      // sure that p is still intact.
      char * p2 = static_cast<char *>(pool.alloc(200));
      CPPUNIT_ASSERT(p);
      CPPUNIT_ASSERT_EQUAL(true, pool.in_use());
      ::memset(p2, 0xea, 200);
      for (int i = 0 ; i < 123 ; ++i) {
        CPPUNIT_ASSERT_EQUAL(char(0xf0), *(p + i));
      }

      pool.free(p);
      pool.free(p2);
    }


    void testHeapMemoryPool()
    {
      namespace mem = fhtagn::memory;

      mem::heap_pool p;

      CPPUNIT_ASSERT_EQUAL(true, p.in_use());
      testMemoryPoolGeneric(p);
      CPPUNIT_ASSERT_EQUAL(true, p.in_use());
    }


    void testFixedMemoryPool()
    {
      namespace mem = fhtagn::memory;

      char memory[1024] = { 0 };
      mem::fixed_pool<> p(memory + 1, sizeof(memory) - 2);

      CPPUNIT_ASSERT_EQUAL(false, p.in_use());
      testMemoryPoolGeneric(p);
      CPPUNIT_ASSERT_EQUAL(false, p.in_use());
    }


    void testFixedPoolFragmentation()
    {
      // Test (de-)fragmentation in fixed pools
      namespace mem = fhtagn::memory;

      char memory[1024] = { 0 };
      mem::fixed_pool<> p(memory, sizeof(memory));

      // For this test, select an allocation size that's larger than the
      // segment header the fixed_pool allocator uses. That way we can produce
      // fragmentation easier.
      std::size_t alloc_size = 10 * sizeof(std::size_t);

      // Allocate three ints in a row. They should be contiguous, and all
      // three allocations should succeed.
      void * first = p.alloc(alloc_size);
      CPPUNIT_ASSERT(first);
      void * second = p.alloc(alloc_size);
      CPPUNIT_ASSERT(second);
      CPPUNIT_ASSERT(second > first);
      void * third = p.alloc(alloc_size);
      CPPUNIT_ASSERT(third);
      CPPUNIT_ASSERT(third > second);

      // Now free the second, and allocate the same amount again. The space
      // just freed should be re-used.
      void * second_prev = second;
      p.free(second);
      second = p.alloc(alloc_size);
      CPPUNIT_ASSERT(second);
      CPPUNIT_ASSERT_EQUAL(second_prev, second);

      // More importantly, if we free the second and allocate twice the amount,
      // that should be placed *behind* third, als there's not enough space
      // between first and third.
      p.free(second);
      second = p.alloc(alloc_size * 2);
      CPPUNIT_ASSERT(second);
      CPPUNIT_ASSERT(second > third);

      // Now we try to allocate half the size of an int in a fourth pointer.
      // That *should* return second_prev, as that's the first space large
      // enough to accommodate that.
      void * fourth = p.alloc(alloc_size / 2);
      CPPUNIT_ASSERT(fourth);
      CPPUNIT_ASSERT_EQUAL(second_prev, fourth);

      // Awesome. If we release fourth again, we'll have fragmentation. To test
      // that defragmentation works as expected, when we again allocate
      // alloc_size, it should again be placed at the same position second_prev
      // is pointing.
      p.free(fourth);
      fourth = p.alloc(alloc_size);
      CPPUNIT_ASSERT(fourth);
      CPPUNIT_ASSERT_EQUAL(second_prev, fourth);
    }


    void testDynamicMemoryPool()
    {
      namespace mem = fhtagn::memory;

      // Let's assume a block size of 1024, and a fixed_pool underlying the
      // dynamic pool. With that, we should be able to create tests fairly easily
      mem::dynamic_pool<mem::fixed_pool<>, 1024> p;

      // Generic tests.
      CPPUNIT_ASSERT_EQUAL(false, p.in_use());
      testMemoryPoolGeneric(p);
      CPPUNIT_ASSERT_EQUAL(false, p.in_use());

      // Allocate five chunks a quarter the size of the fixed pool. That should
      // create a second fixed pool, and prove the new pool creation mechanism.
      void * p1 = p.alloc(256);
      CPPUNIT_ASSERT(p1);
      CPPUNIT_ASSERT_EQUAL(true, p.in_use());
      void * p2 = p.alloc(256);
      CPPUNIT_ASSERT(p2);
      void * p3 = p.alloc(256);
      CPPUNIT_ASSERT(p3);
      void * p4 = p.alloc(256);
      CPPUNIT_ASSERT(p4);
      void * p5 = p.alloc(256);
      CPPUNIT_ASSERT(p5);

      CPPUNIT_ASSERT_EQUAL(true, p.in_use());
      p.free(p1);
      p.free(p2);
      p.free(p3);
      p.free(p4);
      p.free(p5);
      CPPUNIT_ASSERT_EQUAL(false, p.in_use());
    }



    void testBlockMemoryPool()
    {
      namespace mem = fhtagn::memory;

      // 300 bytes memory should translate to
      // 32 bit:                                64 bit:
      // - 70 entries, 280 bytes data           - 70 entries, 280 bytes data
      // - 5 size_t metadata                    - 2 size_t metadata
      // - 0 bytes wasted                       - 4 bytes wasted
      char memory[300] = { 0 };
      int const entries = 70;
      int const entries_2_3rd = entries * 2 / 3;

      mem::block_pool<sizeof(test_int_t)> p(memory, sizeof(memory));

      CPPUNIT_ASSERT_EQUAL(false, p.in_use());

      // Try to allocate something that's not sizeof(test_int_t). It must fail.
      CPPUNIT_ASSERT_EQUAL(static_cast<void *>(NULL), p.alloc(sizeof(test_int_t) / 3));
      CPPUNIT_ASSERT_EQUAL(static_cast<void *>(NULL), p.alloc(sizeof(test_int_t) * 3));

      // On the other hand, allocating an test_int_t must succeed.
      void * q = p.alloc(sizeof(test_int_t));
      CPPUNIT_ASSERT(q);
      CPPUNIT_ASSERT_EQUAL(true, p.in_use());

      // Reallocation must fail, except if there's no size change.
      CPPUNIT_ASSERT(p.realloc(q, sizeof(test_int_t)));
      CPPUNIT_ASSERT_EQUAL(static_cast<void *>(NULL), p.realloc(q, sizeof(test_int_t) / 3));

      // If the above calculation about data + metadata size holds, then we
      // should be able to allocate 46 more entries. The 47th from now on must
      // fail.
      for (int i = 0 ; i < entries_2_3rd ; ++i) {
        CPPUNIT_ASSERT(p.alloc(sizeof(test_int_t)));
      }
      // split in two; we want to remember a q from somewhere in the middle for
      // later
      q = p.alloc(sizeof(test_int_t));
      CPPUNIT_ASSERT(q);
      for (int i = entries_2_3rd ; i < entries - 2 ; ++i) {
        CPPUNIT_ASSERT(p.alloc(sizeof(test_int_t)));
      }

      // Fail.
      CPPUNIT_ASSERT_EQUAL(static_cast<void *>(NULL), p.alloc(sizeof(test_int_t)));

      // Now free q - that should make room for one more allocation.
      p.free(q);
      CPPUNIT_ASSERT(p.alloc(sizeof(test_int_t)));
    }



    void testThrowPool()
    {
      // The best way to test a throw pool is to allocate too much from a
      // fixed_pool.
      namespace mem = fhtagn::memory;

      char memory[200] = { 0 };
      mem::fixed_pool<> p1(memory, sizeof(memory));

      mem::throw_pool<mem::fixed_pool<> > p2(p1);

      // Two small allocations should work, regardless of whether they're
      // through the raw pool or the wrapped pool.
      CPPUNIT_ASSERT(p1.alloc(10));
      CPPUNIT_ASSERT(p2.alloc(10));

      // However, now trying to allocate too much should result in NULL in the
      // first case...
      CPPUNIT_ASSERT_EQUAL(static_cast<void *>(NULL), p1.alloc(sizeof(memory)));
      CPPUNIT_ASSERT_THROW(p2.alloc(sizeof(memory)), std::bad_alloc);
    }



    void testSizeBasedMemoryPool()
    {
      namespace mem = fhtagn::memory;

      mem::size_based_pool<> p;

      CPPUNIT_ASSERT_EQUAL(false, p.in_use());
      testMemoryPoolGeneric(p);
      CPPUNIT_ASSERT_EQUAL(false, p.in_use());
    }



    template <
      typename allocatorT
    >
    void allocatorTests()
    {
      typedef std::vector<test_int_t, allocatorT> vector_t;

      vector_t v;
      v.push_back(1);

      CPPUNIT_ASSERT(!v.empty());
      CPPUNIT_ASSERT_EQUAL(std::size_t(1), v.size());
      CPPUNIT_ASSERT_EQUAL(test_int_t(1), v[0]);

      v.push_back(666);
      v.push_back(42);
      CPPUNIT_ASSERT_EQUAL(std::size_t(3), v.size());
      CPPUNIT_ASSERT_EQUAL(test_int_t(666), v[1]);
      CPPUNIT_ASSERT_EQUAL(test_int_t(42), v[2]);

      // remove entry 666
      typename vector_t::iterator iter = v.begin();
      ++iter;
      v.erase(iter);
      CPPUNIT_ASSERT_EQUAL(std::size_t(2), v.size());
      CPPUNIT_ASSERT_EQUAL(test_int_t(1), v[0]);
      CPPUNIT_ASSERT_EQUAL(test_int_t(42), v[1]);

      // Push back a new entry
      v.push_back(1234);
      CPPUNIT_ASSERT_EQUAL(std::size_t(3), v.size());
      CPPUNIT_ASSERT_EQUAL(test_int_t(1), v[0]);
      CPPUNIT_ASSERT_EQUAL(test_int_t(42), v[1]);
      CPPUNIT_ASSERT_EQUAL(test_int_t(1234), v[2]);
    }



    void testDefaultAllocator()
    {
      // T is test_int_t in these tests.
      allocatorTests<fhtagn::memory::allocator<test_int_t> >();
    }



    void testHeapPoolAllocator()
    {
       // T is test_int_t in these tests.
       namespace mem = fhtagn::memory;

       // heap_pool tests - should always succeed, unless the machine runs out of memory.
       typedef mem::allocator<test_int_t, mem::pool_allocation_policy<test_int_t> > allocator_t;

       // Set global pool to be an instance of heap_pool. That'll be the simplest.
       allocator_t::global_memory_pool = allocator_t::memory_pool_ptr(new mem::heap_pool());

       allocatorTests<allocator_t>();
    }


    void testFixedPoolAllocator()
    {
      namespace mem = fhtagn::memory;

      // fixed_pool tests - the first test tests using a small amount of stack
      // memory.
      typedef mem::allocator<test_int_t, mem::pool_allocation_policy<test_int_t, mem::fixed_pool<> > > allocator_t;

      // Set global pool to be an instance of fixed_pool. That'll be the simplest.
      char memory[200] = { 0 };
      mem::fixed_pool<> * fp = new mem::fixed_pool<>(memory, sizeof(memory));
      CPPUNIT_ASSERT_EQUAL(false, fp->in_use());

      allocator_t::global_memory_pool = allocator_t::memory_pool_ptr(fp);

      CPPUNIT_ASSERT_EQUAL(false, fp->in_use());
      allocatorTests<allocator_t>();
      CPPUNIT_ASSERT_EQUAL(false, fp->in_use());
    }


    void testDynamicPoolAllocator()
    {
      namespace mem = fhtagn::memory;

      // dynamic_pool tests
      typedef mem::allocator<
        test_int_t,
        mem::pool_allocation_policy<
          test_int_t,
          dynamic_pool_t
        >
      > allocator_t;

      // Set global pool to be an instance of dynamic_pool. That'll be the simplest.
      dynamic_pool_t * p = new dynamic_pool_t();
      CPPUNIT_ASSERT_EQUAL(false, p->in_use());

      allocator_t::global_memory_pool = allocator_t::memory_pool_ptr(p);

      CPPUNIT_ASSERT_EQUAL(false, p->in_use());
      allocatorTests<allocator_t>();
      CPPUNIT_ASSERT_EQUAL(false, p->in_use());
    }


    void testSizeBasedPoolAllocator()
    {
      // T is test_int_t in these tests.
      namespace mem = fhtagn::memory;

      // size_based_pool tests - should always succeed, unless the machine runs out of memory.
      typedef mem::allocator<
        test_int_t,
        mem::pool_allocation_policy<
          test_int_t,
          mem::size_based_pool<>
        >
      > allocator_t;

      // Set global pool to be an instance of size_based_pool. That'll be the simplest.
      allocator_t::global_memory_pool = allocator_t::memory_pool_ptr(new mem::size_based_pool<>());

      allocatorTests<allocator_t>();
    }


};


CPPUNIT_TEST_SUITE_REGISTRATION(AllocatorTest);
