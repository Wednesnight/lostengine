--[[
    hold references in result to avoid objects being garbage collected
  ]]
local _engine = lost.audio.Engine.create()
local _blockRemoved = _engine:createSource()
local _pathRemoved = _engine:createSource()
local _levelFinished = _engine:createSource()
_blockRemoved:initWithFile(lost.application.currentTasklet.loader:load("block.ogg"))
_pathRemoved:initWithFile(lost.application.currentTasklet.loader:load("path.ogg"))
_levelFinished:initWithFile(lost.application.currentTasklet.loader:load("wannabill.ogg"))

return {
  engine = _engine,
  blockRemoved = _blockRemoved,
  pathRemoved = _pathRemoved,
  levelFinished = _levelFinished
}
