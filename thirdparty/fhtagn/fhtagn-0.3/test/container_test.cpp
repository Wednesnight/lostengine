/**
 * $Id: container_test.cpp 319 2009-06-08 22:17:47Z unwesen $
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

#include <boost/noncopyable.hpp>

#include <cppunit/extensions/HelperMacros.h>

#include <fhtagn/containers/ntree.h>

namespace {

struct tree_value
{
  bool x;

  tree_value()
    : x(true)
  {
  }

  bool operator==(tree_value const & other)
  {
    return (x == other.x);
  }

  bool operator!=(tree_value const & other)
  {
    return (x != other.x);
  }
};

} // anonymous namespace

class ContainerTest
    : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(ContainerTest);

      CPPUNIT_TEST(testNTreeQuad);
      CPPUNIT_TEST(testNTreeUnlimited);

    CPPUNIT_TEST_SUITE_END();

private:

    template <typename n_treeT>
    void testNTreeGeneric(n_treeT & tree, typename n_treeT::size_type expected_max_size)
    {
      // Keep a const reference around so we can test const functions.
      n_treeT const & const_tree = tree;

      // So, an empty tree must fulfil a few criteria
      CPPUNIT_ASSERT_EQUAL(true, tree.leaf());
      CPPUNIT_ASSERT_EQUAL(true, tree.empty());
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(0), tree.size());
      CPPUNIT_ASSERT_EQUAL(expected_max_size, tree.max_size());
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::contained_ptr(NULL), tree.get());
      CPPUNIT_ASSERT_EQUAL(false, static_cast<bool>(tree));
      CPPUNIT_ASSERT_EQUAL(true, !tree);

      // Newly created trees are root trees by defnition and must fulfil some
      // more criteria.
      CPPUNIT_ASSERT_EQUAL(reinterpret_cast<typename n_treeT::pointer>(NULL), tree.parent());
      CPPUNIT_ASSERT_EQUAL(reinterpret_cast<typename n_treeT::pointer>(NULL), tree.next());
      CPPUNIT_ASSERT_EQUAL(reinterpret_cast<typename n_treeT::pointer>(NULL), tree.prev());

      // An emptry tree can have a value. Check that setting and retrieving
      // the value works.
      *tree = 42;
      CPPUNIT_ASSERT(tree.get());
      CPPUNIT_ASSERT_EQUAL(int(42), *tree);
      CPPUNIT_ASSERT_EQUAL(int(42), *tree.get());
      CPPUNIT_ASSERT_EQUAL(true, static_cast<bool>(tree));
      CPPUNIT_ASSERT_EQUAL(false, !tree);
      CPPUNIT_ASSERT_EQUAL(int(42), *const_tree);
      CPPUNIT_ASSERT_EQUAL(int(42), *const_tree.get());

      // Iterating over a leaf node should result in a zero-step loop
      std::size_t loop_count = 0;
      typename n_treeT::const_iterator tree_end = tree.end();
      for (typename n_treeT::const_iterator t_iter = tree.begin()
          ; t_iter != tree_end ; ++t_iter)
      {
        ++loop_count;
      }
      CPPUNIT_ASSERT_EQUAL(std::size_t(0), loop_count);


      // Right, now make some child nodes.
      tree.split();
      CPPUNIT_ASSERT_EQUAL(false, tree.leaf());
      CPPUNIT_ASSERT_EQUAL(true, tree.empty());
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(0), tree.size());

      // Now we have children, let's iterate over them. None of the iterators
      // should resolve to anything but NULL, though.
      tree_end = tree.end();
      for (typename n_treeT::const_iterator t_iter = tree.begin()
          ; t_iter != tree_end ; ++t_iter)
      {
        CPPUNIT_ASSERT_EQUAL(static_cast<n_treeT *>(NULL), *t_iter);
      }

      typename n_treeT::iterator mutable_tree_end = tree.end();
      for (typename n_treeT::iterator t_iter = tree.begin()
          ; t_iter != mutable_tree_end ; ++t_iter)
      {
        CPPUNIT_ASSERT_EQUAL(static_cast<n_treeT *>(NULL), *t_iter);
      }

      // The same goes for reverse iteration.
      typename n_treeT::const_reverse_iterator tree_rend = tree.rend();
      for (typename n_treeT::const_reverse_iterator t_iter = tree.rbegin()
          ; t_iter != tree_rend ; ++t_iter)
      {
        CPPUNIT_ASSERT_EQUAL(static_cast<n_treeT *>(NULL), *t_iter);
      }

      typename n_treeT::reverse_iterator mutable_tree_rend = tree.rend();
      for (typename n_treeT::reverse_iterator t_iter = tree.rbegin()
          ; t_iter != mutable_tree_rend ; ++t_iter)
      {
        CPPUNIT_ASSERT_EQUAL(static_cast<n_treeT *>(NULL), *t_iter);
      }

      // Now we'll fill values into two of the child nodes. We're very specific
      // in that it's the second (index 1) and fourth (index 3) we're filling,
      // because that allows for nice boundary tests later on. Note that while
      // operator[] returns a reference to n_treeT, n_treeT itself acts as a
      // pointer to int, therefore the return value must be dereferenced again.
      *tree[1] = 666;
      CPPUNIT_ASSERT_EQUAL(int(666), *const_tree[1]);
      *tree[3] = 1337;
      CPPUNIT_ASSERT_EQUAL(int(1337), *const_tree[3]);
      CPPUNIT_ASSERT_EQUAL(false, const_tree.empty());

      // That also means iterating over the tree's children will have at least
      // two non-empty children.
      std::size_t non_empty = 0;
      tree_end = tree.end();
      for (typename n_treeT::iterator t_iter = tree.begin()
          ; t_iter != tree_end ; ++t_iter)
      {
        if (*t_iter) {
          ++non_empty;
        }
      }
      CPPUNIT_ASSERT_EQUAL(std::size_t(2), non_empty);
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(2), tree.size());

      // Try to erase elements by various different methods. We'll need to
      // add the element again afterwards for the next erase to have anything
      // to do.
      typename n_treeT::iterator tree_iter = tree.begin();
      ++tree_iter;
      CPPUNIT_ASSERT(*tree_iter);
      CPPUNIT_ASSERT((*tree_iter)->get());
      CPPUNIT_ASSERT_EQUAL(int(666), *(*tree_iter)->get());
      tree.erase(tree_iter);
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(1), tree.size());
      CPPUNIT_ASSERT(!*tree_iter);

      *tree[1] = 666;
      CPPUNIT_ASSERT_EQUAL(int(666), *(*tree_iter)->get());

      // Actually, before we try anything else, let's try to erase an invalid
      // iterator.
      tree.erase(tree.end());
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(2), tree.size());
      CPPUNIT_ASSERT_EQUAL(int(666), *(*tree_iter)->get());

      *tree[1] = 666;
      CPPUNIT_ASSERT_EQUAL(int(666), *(*tree_iter)->get());

      // Now let's try erasing by index.
      tree.erase(typename n_treeT::size_type(1));
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(1), tree.size());
      CPPUNIT_ASSERT(!*tree_iter);

      *tree[1] = 666;
      CPPUNIT_ASSERT_EQUAL(int(666), *(*tree_iter)->get());

      // Awesome, let's try erasing by contained_type.
      tree.erase(typename n_treeT::contained_type(666));
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(1), tree.size());
      CPPUNIT_ASSERT(!*tree_iter);

      *tree[1] = 666;
      CPPUNIT_ASSERT_EQUAL(int(666), *(*tree_iter)->get());

      // Now to erase by value type.
      n_treeT * n = *tree_iter;
      CPPUNIT_ASSERT(n);
      tree.erase(*n);
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(1), tree.size());
      CPPUNIT_ASSERT(!*tree_iter);

      *tree[1] = 666;
      CPPUNIT_ASSERT_EQUAL(int(666), *(*tree_iter)->get());

      // Lastly, erase all from tree_iter to the end. Since tree_iter happens to
      // point at the first node with a value, this should result in all values
      // being empty, i.e. size() being zero afterwards.
      tree.erase(tree_iter, tree.end());
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(0), tree.size());
      CPPUNIT_ASSERT(!*tree_iter);

      // Right, now let's fill the container again so we can test the various
      // incarnations of find()
      *tree[1] = 666;
      CPPUNIT_ASSERT_EQUAL(int(666), *const_tree[1]);
      *tree[3] = 1337;
      CPPUNIT_ASSERT_EQUAL(int(1337), *const_tree[3]);
      CPPUNIT_ASSERT_EQUAL(false, const_tree.empty());

      {
        // Find by value_type
        CPPUNIT_ASSERT(*tree_iter);
        typename n_treeT::const_iterator tmp_iter = tree.find(**tree_iter);
        CPPUNIT_ASSERT(tree.end() != tmp_iter);
        CPPUNIT_ASSERT(tree_iter == tmp_iter);

        // Find by contained type
        tmp_iter = tree.find(int(666));
        CPPUNIT_ASSERT(tree.end() != tmp_iter);
        CPPUNIT_ASSERT(tree_iter == tmp_iter);
        CPPUNIT_ASSERT_EQUAL(int(666), *(*tmp_iter)->get());

        // Find by value_type (const)
        CPPUNIT_ASSERT(*tree_iter);
        tmp_iter = const_tree.find(**tree_iter);
        CPPUNIT_ASSERT(const_tree.end() != tmp_iter);
        CPPUNIT_ASSERT(tree_iter == tmp_iter);

        // Find by contained type (const)
        tmp_iter = const_tree.find(int(666));
        CPPUNIT_ASSERT(const_tree.end() != tmp_iter);
        CPPUNIT_ASSERT(tree_iter == tmp_iter);
        CPPUNIT_ASSERT_EQUAL(int(666), *(*tmp_iter)->get());
      }

      // Test counting.
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(1), const_tree.count(666));
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(1), const_tree.count(1337));
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(0), const_tree.count(42));

      // Test swapping two n_tree objects.
      CPPUNIT_ASSERT_EQUAL(int(666), *tree[1]);
      CPPUNIT_ASSERT_EQUAL(int(1337), *tree[3]);
      tree[1].swap(tree[3]);
      CPPUNIT_ASSERT_EQUAL(int(1337), *tree[1]);
      CPPUNIT_ASSERT_EQUAL(int(666), *tree[3]);

      // Test deep copies - we create a temporary tree that must be equivalent
      // to the original.
      n_treeT tmp_tree = tree;
      CPPUNIT_ASSERT_EQUAL(int(1337), *tmp_tree[1]);
      CPPUNIT_ASSERT_EQUAL(int(666), *tmp_tree[3]);

      // Now we have two trees, we can exercise the comparison operators.
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree == tree));
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree != tree));
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree > tree));
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree < tree));
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree >= tree));
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree <= tree));
      CPPUNIT_ASSERT_EQUAL(false, (tree > tmp_tree));
      CPPUNIT_ASSERT_EQUAL(false, (tree < tmp_tree));
      CPPUNIT_ASSERT_EQUAL(true,  (tree >= tmp_tree));
      CPPUNIT_ASSERT_EQUAL(true,  (tree <= tmp_tree));

      // We can also compare tree[1] to tree[3].
      CPPUNIT_ASSERT_EQUAL(false, (tree[1] == tree[3]));
      CPPUNIT_ASSERT_EQUAL(true,  (tree[1] != tree[3]));
      CPPUNIT_ASSERT_EQUAL(true,  (tree[1] > tree[3]));
      CPPUNIT_ASSERT_EQUAL(false, (tree[1] < tree[3]));
      CPPUNIT_ASSERT_EQUAL(true,  (tree[1] >= tree[3]));
      CPPUNIT_ASSERT_EQUAL(false, (tree[1] <= tree[3]));
      CPPUNIT_ASSERT_EQUAL(false, (tree[3] > tree[1]));
      CPPUNIT_ASSERT_EQUAL(true,  (tree[3] < tree[1]));
      CPPUNIT_ASSERT_EQUAL(false, (tree[3] >= tree[1]));
      CPPUNIT_ASSERT_EQUAL(true,  (tree[3] <= tree[1]));

      // If we now drop one of the nodes in tmp_tree, then tree and tmp_tree
      // must differ - tree must be considered larger.
      tmp_tree.erase(typename n_treeT::size_type(1));
      CPPUNIT_ASSERT_EQUAL(false, (tree == tmp_tree));
      CPPUNIT_ASSERT_EQUAL(true,  (tree != tmp_tree));
      CPPUNIT_ASSERT_EQUAL(true,  (tree > tmp_tree));
      CPPUNIT_ASSERT_EQUAL(false, (tree < tmp_tree));
      CPPUNIT_ASSERT_EQUAL(true,  (tree >= tmp_tree));
      CPPUNIT_ASSERT_EQUAL(false, (tree <= tmp_tree));
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree > tree));
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree < tree));
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree >= tree));
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree <= tree));

      // Nodes with a value can be compared to the contained type.
      CPPUNIT_ASSERT_EQUAL(true,  (666 == *tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(false, (666 != *tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(true,  (667 > *tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(true,  (665 < *tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(true,  (667 >= *tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(true,  (665 <= *tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(false, (665 > *tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(false, (667 < *tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(false, (665 >= *tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(false, (667 <= *tmp_tree[3]));

      CPPUNIT_ASSERT_EQUAL(true,  (*tmp_tree[3] == 666));
      CPPUNIT_ASSERT_EQUAL(false, (*tmp_tree[3] != 666));
      CPPUNIT_ASSERT_EQUAL(true,  (*tmp_tree[3] > 665));
      CPPUNIT_ASSERT_EQUAL(true,  (*tmp_tree[3] < 667));
      CPPUNIT_ASSERT_EQUAL(true,  (*tmp_tree[3] >= 665));
      CPPUNIT_ASSERT_EQUAL(true,  (*tmp_tree[3] <= 667));
      CPPUNIT_ASSERT_EQUAL(false, (*tmp_tree[3] > 667));
      CPPUNIT_ASSERT_EQUAL(false, (*tmp_tree[3] < 665));
      CPPUNIT_ASSERT_EQUAL(false, (*tmp_tree[3] >= 667));
      CPPUNIT_ASSERT_EQUAL(false, (*tmp_tree[3] <= 665));

      // We'll also need to compare trees with a value and trees without.
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree[3] == tmp_tree[2]));
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree[3] != tmp_tree[2]));
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree[3] > tmp_tree[2]));
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree[3] < tmp_tree[2]));
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree[3] >= tmp_tree[2]));
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree[3] <= tmp_tree[2]));
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree[2] > tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree[2] < tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree[2] >= tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree[2] <= tmp_tree[3]));

      // Also of interest is what happens when the two nodes have the same value,
      // but one of them has children.
      tmp_tree[2].split();
      *tmp_tree[2] = *tmp_tree[3];
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree[3] == tmp_tree[2]));
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree[3] != tmp_tree[2]));
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree[3] > tmp_tree[2]));
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree[3] < tmp_tree[2]));
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree[3] >= tmp_tree[2]));
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree[3] <= tmp_tree[2]));
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree[2] > tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree[2] < tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(true,  (tmp_tree[2] >= tmp_tree[3]));
      CPPUNIT_ASSERT_EQUAL(false, (tmp_tree[2] <= tmp_tree[3]));


      // We've erased tmp_tree's element at index one. Now let's unlink the
      // third...
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(2), tmp_tree.size());
      n_treeT & tree3 = tmp_tree[3];
      CPPUNIT_ASSERT_EQUAL(&tmp_tree, tree3.parent());
      tree3.unlink();
      CPPUNIT_ASSERT_EQUAL(static_cast<n_treeT *>(NULL), tree3.parent());
      delete &tree3; // need to clean the leak
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(1), tmp_tree.size());

      // Since we've just tested unlink, also test the next and prev
      // functions.
      n_treeT & tree4 = tree[1];
      CPPUNIT_ASSERT_EQUAL(static_cast<n_treeT *>(NULL), tree4.next());
      CPPUNIT_ASSERT_EQUAL(static_cast<n_treeT *>(NULL), tree4.prev());

      // To make this more exciting, add a new entry to tree.
      *tree[2] = 54321;
      n_treeT & tree5 = tree[2];
      CPPUNIT_ASSERT_EQUAL(&tree5, tree4.next());
      CPPUNIT_ASSERT_EQUAL(&tree4, tree5.prev());

      // Assignment from self must succeed without problems.
      CPPUNIT_ASSERT_EQUAL(tree, tree = tree);

      // Assignment to a node with a value must overwrite that value.
      tree[2] = tree[3];
      CPPUNIT_ASSERT(tree[2] == tree[3]);

      // begin() and end() must return the same on an empty node.
      CPPUNIT_ASSERT(tree[2].end() == tree[2].begin());

      // Test capacity() - in the unlimited case, the number is somewhat of a
      // guess, as vector<T> doubles in capacity, and we have an element at
      // index 3.
      CPPUNIT_ASSERT_EQUAL(typename n_treeT::size_type(4), tree.capacity());

      // Whatever we try to find on an empty node must fail.
      CPPUNIT_ASSERT(tree[2].end() == tree[2].find(tree));

      // We shouldn't be able to find the tmp_tree in tree.
      CPPUNIT_ASSERT(tree.end() == tree.find(tmp_tree));

      // Last, construct a tree from an interator range.
      n_treeT tree6(tree.begin(), tree.end());
      *tree6 = *tree;
      CPPUNIT_ASSERT(tree == tree6);
    }


    void testNTreeQuad()
    {
      namespace cnt = fhtagn::containers;

      typedef cnt::n_tree<4, int> quad_tree_t;
      quad_tree_t quad_tree;
      testNTreeGeneric(quad_tree, 4);

      // Test operator->
      cnt::n_tree<4, tree_value> quad_tree_struct;
      *quad_tree_struct = tree_value();
      CPPUNIT_ASSERT_EQUAL(true, quad_tree_struct->x);
    }


    void testNTreeUnlimited()
    {
      namespace cnt = fhtagn::containers;

      typedef cnt::n_tree<0, int> unlimited_tree_t;
      unlimited_tree_t unlimited_tree;

      typedef std::vector<int> vector_t;
      vector_t::size_type expected_max_size = 0;
      {
        typedef std::vector<int *> int_ptr_vector_t;
        expected_max_size = int_ptr_vector_t::size_type(-1)
          / sizeof(int_ptr_vector_t::value_type);
      }
      testNTreeGeneric(unlimited_tree, expected_max_size);

      // Test operator->
      cnt::n_tree<0, tree_value> unlimited_tree_struct;
      *unlimited_tree_struct = tree_value();
      CPPUNIT_ASSERT_EQUAL(true, unlimited_tree_struct->x);
    }
};


CPPUNIT_TEST_SUITE_REGISTRATION(ContainerTest);
