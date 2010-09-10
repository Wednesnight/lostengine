#ifndef LOST_STD_H
#define LOST_STD_H

#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>

#include "MemoryPoolAllocator.h"

namespace lost
{

typedef std::basic_string<char, std::char_traits<char>, MemoryPoolAllocator<char> > string;
//typedef std::vector<T, MemoryPoolAllocator<T> > vector;

template<typename T>
class vector : public std::vector<T, MemoryPoolAllocator<T> >
{
};

}

template<typename K, typename V, typename COMP = std::less<K> >
class map : public std::map<K,V,COMP, MemoryPoolAllocator<std::pair<const K, V> > >
{
};

template<typename V>
class list : public std::list<V, MemoryPoolAllocator<V> >
{
};

#endif