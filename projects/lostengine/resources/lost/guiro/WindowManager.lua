module("lost.guiro", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.guiro.WindowManager" {}

function WindowManager:constructor()
end

function WindowManager:openWindow(win)
  if not lost.guiro.ui():containsView(win) then
    lost.guiro.ui():addSubview(win)
    win:hidden(false)
  end
end

function WindowManager:closeWindow(win)
  win:hidden(true)
  lost.guiro.ui():removeSubview(win)
end