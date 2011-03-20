require("lost.common.Class")

lost.common.Class "FileListDelegate" {}

function FileListDelegate:constructor()
  log.debug("constructor")
end

function FileListDelegate:cellForRowAtIndexPath(listView,indexPath)
  local reuseId = "cell"
  local result = listView:dequeueCell(reuseId)
  if not result then
    local l = lost.guiro.view.Label{text="Hello"}
    result=lost.guiro.view.View
    {
      subviews={l}
    }
    result.label=l
    result.reuseId = reuseId
    result.dataSource=function(self,ds)
      self.label:text(ds.path)
    end
  end
  return result
end