module("lost.guiro", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.guiro.WindowManager" {}

function WindowManager:constructor()
  self._menus = {}
  self._numMenus = 0
  self._modalLayer = nil
  self._modalCount = 0
end

function WindowManager:openWindow(win)
  if not lost.guiro.ui():containsView(win) then
    if win.modal then
      if self._modalLayer == nil then
        self._modalLayer = lost.guiro.view.View{sublayers = {lost.guiro.layer.Rect{color = lost.common.Color(.3,.3,.3,.3)}}, hidden = true}
      end
      if self._modalCount == 0 then
        lost.guiro.ui():addSubview(self._modalLayer)
        self._modalLayer:hidden(false)
      end
      self._modalCount = self._modalCount+1
    end
    lost.guiro.ui():addSubview(win)
    win:hidden(false)
    lost.guiro.ui().eventManager:receiveFocus(win)
  end
end

function WindowManager:closeWindow(win)
  lost.guiro.ui().eventManager:loseFocus(win)
  win:hidden(true)
  lost.guiro.ui():removeSubview(win)
  if win.modal and self._modalLayer ~= nil then
    self._modalCount = self._modalCount-1
    if self._modalCount <= 0 then
      self._modalCount = 0
      self._modalLayer:hidden(true)
      lost.guiro.ui():removeSubview(self._modalLayer)
    end
  end
end

function WindowManager:openMenu(menu)
  if not self._menus[menu] then
    self:openWindow(menu)
    self._menus[menu] = menu
    self._numMenus = self._numMenus + 1
--    log.debug("menus "..self._numMenus)
  end
end

function WindowManager:closeMenu(menu)
  if self._menus[menu] then
    self:closeWindow(menu)
    self._menus[menu] = nil
    self._numMenus = self._numMenus - 1
--    log.debug("menus "..self._numMenus)
  end
end

function WindowManager:mouseDown(event)
  local oneMenuContainsEvent = false
  if self._numMenus > 0 then
    for k,v in pairs(self._menus) do
      if v:containsCoord(event.pos) then
        oneMenuContainsEvent = true
        break
      end
    end
  end
  
  -- close all menus if click was not on any one of them
  if not oneMenuContainsEvent then
    for k,v in pairs(self._menus) do
      if v:externalCloseRequest(event) then
        v:close()
      end
    end
  end
  
end