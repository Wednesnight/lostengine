--[[
    hold references in result to avoid objects being garbage collected
  ]]
local _engine = lost.audio.Engine.create()
local _blockRemoved = _engine:createSource()
local _pathRemoved = _engine:createSource()
local _levelFinished = _engine:createSource()
_blockRemoved:initWithFile(lost.application.currentTasklet.loader:load("resources/sounds/block.ogg"))
_pathRemoved:initWithFile(lost.application.currentTasklet.loader:load("resources/sounds/path.ogg"))
_levelFinished:initWithFile(lost.application.currentTasklet.loader:load("resources/sounds/wannabill.ogg"))

local sounds = {enabled = true}

sounds.engine = _engine
sounds.blockRemoved = _blockRemoved
sounds.pathRemoved = _pathRemoved
sounds.levelFinished = _levelFinished

function sounds:play(sound)
  if self.enabled and self[sound] ~= nil then
    self[sound]:play()
  end
end

return sounds
