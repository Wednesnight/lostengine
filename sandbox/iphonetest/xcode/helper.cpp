/*
 *  helper.cpp
 *  iphonetest
 *
 *  Created by Tony Kostanjsek on 27.07.08.
 *  Copyright 2008 Orbster GmbH. All rights reserved.
 *
 */

#include "helper.h"

#include <iostream>
#include "lost/platform/Platform.h"
#include "lost/resource/DefaultLoader.h"
#include <boost/shared_ptr.hpp>
#include "lost/common/Logger.h"
#include "lost/lua/State.h"
#include "lost/lua/LuaBindings.h"

namespace lost
{
namespace lua
  {
void bindLostBitmapBitmap(lost::lua::State& state) {}
void bindLostBitmapBitmapLoader(lost::lua::State& state) {}
void bindLostApplicationApplication(lost::lua::State& state) {}
  }
}

using namespace std;
using namespace lost;
using namespace boost;
using namespace lost::platform;
using namespace lost::resource;
using namespace lost::common;

void testit()
{
  try
  {
    DOUT("resource path: "<< getResourcePath())
    
    shared_ptr<Loader> loader(new DefaultLoader);
    shared_ptr<File> file = loader->load("init");
    DOUT("file contents: " << file->str())
    shared_ptr<lua::State> interpreter(new lua::State);
    lost::lua::bindAll(*interpreter); // bind lostengine lua mappings    
    interpreter->doString(file->str());  
  }
  catch(exception& ex)
  {
    cerr<<"error: " << ex.what() << endl;
  }
}