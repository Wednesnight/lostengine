module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.DirectoryView" "lost.guiro.view.View" {}

-- a simple directory list view
function DirectoryView:constructor(args)
  lost.guiro.view.View.constructor(self, args)

  self._rootDirectory = args.directory or ""
  self:directory(self._rootDirectory)
end

function DirectoryView:directory(...)
  if arg.n > 0 then
    self._directory = arg[1]
    self._directoryDirty = true
    self:needsUpdate()
  else
    return self._directory
  end
end

function DirectoryView:select(row, path)
  local now = lost.platform.currentTimeSeconds()
  if path ~= nil and self._selection == row and self._lastClick ~= nil and now - self._lastClick <= 0.5 then
    -- double click
    self:directory(tostring(path))
  else
    self._lastClick = now
    if self._selection ~= row then
      if self._selection ~= nil then
        self._selection.layer("selection"):hidden(true)
      end
      self._selection = row
      self._selection.layer("selection"):hidden(false)
    end
  end
end

function DirectoryView:update()
  lost.guiro.view.View.update(self)

  if self._directoryDirty then
    self:removeAllSubviews()

    local top = nil
    if self._directory ~= self._rootDirectory then
      top = self:createRow()
      top:addEventListener("mouseDown", function(event)
        self:select(event.target, boost.filesystem.path(self._directory):remove_filename())
      end)
      self:addSubview(top)
    end

    local offset = 0
    if top ~= nil then
      offset = self.entryHeight
    end

    for dir in tasklet.loader:directory(self._directory) do
      local copy = self:createRow()
      local bounds = copy:bounds()
      bounds.y = lost.guiro.Bounds.decodeEntry(2, {"top", -offset})
      offset = offset + self.entryHeight

      local currentDir = boost.filesystem.path(self._directory) / tostring(dir)
      if boost.filesystem.is_directory(dir:status()) then
        copy:addEventListener("mouseDown", function(event) self:select(event.target, currentDir) end)
      else
        copy.layer("image"):texture(self.fileTexture)
        copy:addEventListener("mouseDown", function(event) self:select(event.target) end)
      end
      copy.layer("text"):text(tostring(currentDir))

      self:addSubview(copy)
    end

    self._directoryDirty = false
  end
end

