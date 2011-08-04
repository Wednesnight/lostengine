#include "lost/application/TaskletConfig.h"
#include "lost/lua/lua.h"
#include "lost/lua/State.h"
#include <boost/filesystem.hpp>
#include "lost/resource/Loader.h"
#include "lost/common/Logger.h"

namespace lost
{
namespace application
{

using namespace lost::math;
using namespace luabind;

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

// c-function so it doesn't have to go in the header with luabind::object in the signature
void parse(slub::reference config, TaskletConfig* tc)
{
  if(config["tasklet"].type() == LUA_TTABLE) {

    if(config["tasklet"]["name"].type() != LUA_TNIL) { tc->taskletName = config["tasklet"]["name"].get<string>(); }
    if(config["tasklet"]["waitForEvents"].type() != LUA_TNIL) { tc->taskletWaitForEvents = config["tasklet"]["waitForEvents"].get<bool>(); }
    if(config["tasklet"]["hasWindow"].type() != LUA_TNIL) { tc->taskletHasWindow = config["tasklet"]["hasWindow"].get<bool>(); }
  }

  if(config["window"].type() == LUA_TTABLE) {

    if(config["window"]["title"].type() != LUA_TNIL) { tc->windowTitle = config["window"]["title"].get<string>(); }
    if(config["window"]["x"].type() != LUA_TNIL) { tc->windowRect.x = config["window"]["x"].get<float>(); }
    if(config["window"]["y"].type() != LUA_TNIL) { tc->windowRect.y = config["window"]["y"].get<float>(); }
    if(config["window"]["width"].type() != LUA_TNIL) { tc->windowRect.width = config["window"]["width"].get<float>(); }
    if(config["window"]["height"].type() != LUA_TNIL) { tc->windowRect.height = config["window"]["height"].get<float>(); }
  }

  if(config["renderer"].type() == LUA_TTABLE) {
    
    if(config["renderer"]["framerate"].type() != LUA_TNIL) { tc->framerate = config["renderer"]["framerate"].get<double>(); }
  }

  if(config["gl"].type() == LUA_TTABLE) {
    
    if(config["gl"]["vsync"].type() != LUA_TNIL) { tc->glVsync = config["gl"]["vsync"].get<bool>(); }
  }

}

bool TaskletConfig::load(lua::StatePtr interpreter, resource::LoaderPtr loader)
{
  bool result = false;
  if(loader->exists("config.lua"))
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
