/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/event/Signal.h"
#include "lost/event/Listener.h"

namespace lost
{
namespace event
{

Signal::Signal()
{
  counter = 0;
}

Signal::~Signal()
{
}
  
uint32_t Signal::addListener(ListenerPtr listener)
{
  uint32_t result = counter;
  ++counter;
  
  id2listener[result] = listener;
  
  return result;
}

void Signal::removeListener(uint32_t index)
{
  map<uint32_t, ListenerPtr>::iterator pos = id2listener.find(index);
  if(pos != id2listener.end())
  {
    id2listener.erase(pos);
  }
}

void Signal::call(EventPtr event)
{
  map<uint32_t, ListenerPtr>::iterator i;
  for(i = id2listener.begin(); i!= id2listener.end(); ++i)
  {
    i->second->call(event);
  }
}

}
}
