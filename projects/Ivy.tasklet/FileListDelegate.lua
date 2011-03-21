require("lost.common.Class")
require("FileListCell")
lost.common.Class "FileListDelegate" {}

function FileListDelegate:constructor()
end

function FileListDelegate:heightForRowAtIndexPath(listView,indexPath)
  return 50
end

function FileListDelegate:cellForRowAtIndexPath(listView,indexPath)
  local result = listView:dequeueCell(FileListCell.reuseId)
  if not result then
    result = FileListCell{}
  end
  result:switchColor((indexPath[2] % 2)==0)
  return result
end