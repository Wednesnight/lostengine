#include "lost/resource/Bundle.h"
#include "lost/resource/FilesystemRepository.h"
#include "lost/common/Data.h"
#include "lost/common/Logger.h"

namespace lost
{
namespace resource
{

Bundle::Bundle(const fs::Path& inRootDir)  
{
  RepositoryPtr repo = FilesystemRepository::create(inRootDir);
  addRepository(repo);
}

BundlePtr Bundle::create(const fs::Path& inRootDir)
{
  return BundlePtr(new Bundle(inRootDir));
}

BundlePtr Bundle::subBundle(const lost::fs::Path& relativePath)
{
  return Bundle::create(repo->rootDirectory / relativePath);
}
  
slub::reference Bundle::require(const lost::fs::Path& relativePathToLuaFile, lua_State* state)
{
  common::DataPtr data = load(relativePathToLuaFile);
  string sdata = data->str();
  int res = luaL_dostring(state, sdata.c_str());
  if(res)
  {
    EOUT("!!! Error loading "<<relativePathToLuaFile.string());
    return slub::reference();
  }
  else
  {
    return slub::reference(state);    
  }
}

}
}