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
using namespace std;

void TaskletConfig::reset()
{
  taskletName = "<unnamed tasklet>";
  taskletWaitForEvents = false;
  taskletHasWindow = true;

  windowTitle = taskletName;
  windowRect = Rect(0,0,640,480);

  framerate = 1.0 / 60.0;
}

TaskletConfig::TaskletConfig()
{
  reset();
}

// c-function so it doesn't have to go in the header with luabind::object in the signature
void parse(object config, TaskletConfig* tc)
{
  if(type(config["tasklet"]) == LUA_TTABLE) {

    if(type(config["tasklet"]["name"]) != LUA_TNIL) { tc->taskletName = object_cast<string>(config["tasklet"]["name"]); }
    if(type(config["tasklet"]["waitForEvents"]) != LUA_TNIL) { tc->taskletWaitForEvents = object_cast<bool>(config["tasklet"]["waitForEvents"]); }
    if(type(config["tasklet"]["hasWindow"]) != LUA_TNIL) { tc->taskletHasWindow = object_cast<bool>(config["tasklet"]["hasWindow"]); }    
  }

  if(type(config["window"]) == LUA_TTABLE) {

    if(type(config["window"]["title"]) != LUA_TNIL) { tc->windowTitle = object_cast<string>(config["window"]["title"]); }
    if(type(config["window"]["x"]) != LUA_TNIL) { tc->windowRect.x = object_cast<float>(config["window"]["x"]); }
    if(type(config["window"]["y"]) != LUA_TNIL) { tc->windowRect.y = object_cast<float>(config["window"]["y"]); }
    if(type(config["window"]["width"]) != LUA_TNIL) { tc->windowRect.width = object_cast<float>(config["window"]["width"]); }
    if(type(config["window"]["height"]) != LUA_TNIL) { tc->windowRect.height = object_cast<float>(config["window"]["height"]); }
  }

  if(type(config["renderer"]) == LUA_TTABLE) {
    
    if(type(config["renderer"]["framerate"]) != LUA_TNIL) { tc->framerate = object_cast<double>(config["renderer"]["framerate"]); }
  }
}

bool TaskletConfig::load(lua::StatePtr interpreter, resource::LoaderPtr loader)
{
  bool result = false;
  if(loader->exists("config.lua"))
  {
    interpreter->doString("__tasklet_config = require(\"config\")");
    luabind::object config = interpreter->globals["__tasklet_config"];
    if(luabind::type(config) == LUA_TTABLE)
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
