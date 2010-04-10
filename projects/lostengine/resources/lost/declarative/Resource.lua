-- lost.declarative.Resource
module("lost.declarative", package.seeall)

lost.common.Class "lost.declarative.Resource" {}

function Resource:constructor(loader)
  self.loader = loader
end

function Resource:File(def)
  local path = def["path"]
  if not path then 
    error("you have to specify a path for the file",2)
  end
  return self.loader:load(path)
end
