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

#ifndef LOST_AUDIO_ENGINE_H
#define LOST_AUDIO_ENGINE_H

#include "lost/al/al.h"

namespace lost
{

namespace al{ struct Context; }
namespace audio
{

struct Source;

struct Engine : public lost::enable_shared_from_this<Engine>
{
  Engine();
	Engine(const string& inDeviceId);
  ~Engine();

	vector<string> deviceIds;
  lost::shared_ptr<audio::Source> createSource();
  void destroySource(audio::Source* source);

  lost::shared_ptr<al::Context> context;
	list<lost::weak_ptr<audio::Source> > sources;

  static lost::shared_ptr<Engine> create()
  {
    return lost::shared_ptr<Engine>(new Engine);
  }
};

}
}


#endif
