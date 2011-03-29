local Loader = lost.resource.Loader
local FilesystemRepository = lost.resource.FilesystemRepository
local ApplicationResourceRepository = lost.resource.ApplicationResourceRepository

return {
  init = function(self, ui)
    -- init ui
    self.ui = ui
    lost.guiro.ui():add{ui}

    self.loader = Loader.create()
    self.loader:addRepository(FilesystemRepository.create("/"))
    self.loader:addRepository(ApplicationResourceRepository.create())

    -- init event listeners
    -- ui
    lost.guiro.ui():addEventListener(lost.application.KeyEvent.KEY_DOWN, function(event) self:keyDown(event) end)
    -- shader editors
    self.ui("content")("shaders")("vs"):addEventListener(lost.application.DragNDropEvent.DROP, function(event) self:dropFileInEditor(event, "vs") end)
    self.ui("content")("shaders")("fs"):addEventListener(lost.application.DragNDropEvent.DROP, function(event) self:dropFileInEditor(event, "fs") end)

    --
--    local shader = lost.gl.buildShader(tasklet.loader, "bloom", tasklet.loader:load("bloom.vs"):str(), tasklet.loader:load("bloom.fs"):str())
    --
  end,

  keyDown = function(self, event)
    if event.key == lost.application.K_ESCAPE then
      tasklet.running = false
    end
  end,

  dropFileInEditor = function(self, event, ext)
    local originalEvent = event.lostAppDragNDropEvent
    if originalEvent:numPaths() > 0 then
      local filename = string.match(originalEvent:getPath(0), ".*%.".. ext .."$")
      if filename ~= nil then
        local data = self.loader:load(filename)
        event.target:text(data:str())
      end
    end
  end
}
