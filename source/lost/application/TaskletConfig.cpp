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

#include "lost/application/TaskletConfig.h"
#include "lost/lua/lostlua.h"
#include "lost/lua/State.h"
#include "lost/resource/Loader.h"
#include "lost/common/Logger.h"

namespace lost
{
namespace application
{

using namespace lost::math;

void TaskletConfig::reset()
{
  taskletName = "<unnamed tasklet>";
  taskletWaitForEvents = false;
  taskletHasWindow = true;

  windowTitle = taskletName;
  windowRect = Rect(0,0,640,480);

  framerate = 1.0 / 60.0;
  glVsync = false;
}

TaskletConfig::TaskletConfig()
{
  reset();
}

// c-function so it doesn't have to go in the header with slub::reference in the signature
void parse(const slub::table& config, TaskletConfig* tc)
{
  if(config["tasklet"].type() == LUA_TTABLE) {

    if(config["tasklet"]["name"].type() != LUA_TNIL) { tc->taskletName = config["tasklet"]["name"].cast<string>(); }
    if(config["tasklet"]["waitForEvents"].type() != LUA_TNIL) { tc->taskletWaitForEvents = config["tasklet"]["waitForEvents"].cast<bool>(); }
    if(config["tasklet"]["hasWindow"].type() != LUA_TNIL) { tc->taskletHasWindow = config["tasklet"]["hasWindow"].cast<bool>(); }
  }

  if(config["window"].type() == LUA_TTABLE) {

    if(config["window"]["title"].type() != LUA_TNIL) { tc->windowTitle = config["window"]["title"].cast<string>(); }
    if(config["window"]["x"].type() != LUA_TNIL) { tc->windowRect.x = config["window"]["x"].cast<float>(); }
    if(config["window"]["y"].type() != LUA_TNIL) { tc->windowRect.y = config["window"]["y"].cast<float>(); }
    if(config["window"]["width"].type() != LUA_TNIL) { tc->windowRect.width = config["window"]["width"].cast<float>(); }
    if(config["window"]["height"].type() != LUA_TNIL) { tc->windowRect.height = config["window"]["height"].cast<float>(); }
  }

  if(config["renderer"].type() == LUA_TTABLE) {
    
    if(config["renderer"]["framerate"].type() != LUA_TNIL) { tc->framerate = config["renderer"]["framerate"].cast<double>(); }
  }

  if(config["gl"].type() == LUA_TTABLE) {
    
    if(config["gl"]["vsync"].type() != LUA_TNIL) { tc->glVsync = config["gl"]["vsync"].cast<bool>(); }
  }

}

bool TaskletConfig::load(lua::StatePtr interpreter, resource::LoaderPtr loader)
{
  bool result = false;
  if(loader->exists(lost::fs::Path("config.lua")))
  {
    interpreter->doString("__tasklet_config = require(\"config\")");
    slub::reference config = interpreter->globals["__tasklet_config"];
    if(config.type() == LUA_TTABLE)
    {
      parse(config, this);
      result = true;
    }
    else
    {
      WOUT("no config.lua found or bad return type. Make sure the file exists and returns a table.");
    }
  }
  else {
    WOUT("no config file found!")
  }

  return result;
}

}
}
