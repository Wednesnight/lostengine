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

#ifndef LOST_AL_CONTEXT_H
#define LOST_AL_CONTEXT_H

#include "lost/al/al.h"

namespace lost
{
namespace al
{
struct Device;

struct Context
{
  Context(lost::shared_ptr<Device> device);
  Context();
  virtual ~Context();

  void initWithDevice(lost::shared_ptr<Device> device);

  void makeCurrent();

  void process();
  void suspend();

  // call these only after you created a context and made it current
  static ALfloat dopplerFactor();
  static ALfloat speedOfSound();
  static ALenum distanceModel();

  static void distanceModel(ALenum dm);

  static string version();
  static string renderer();
  static string vendor();
  static string extensions();

  ALCcontext* context;
  lost::shared_ptr<Device> device;
};
}
}


#endif
