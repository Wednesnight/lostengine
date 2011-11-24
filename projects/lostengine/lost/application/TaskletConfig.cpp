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
void parse(slub::reference config, TaskletConfig* tc)
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
