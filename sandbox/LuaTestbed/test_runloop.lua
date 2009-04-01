function main(application)
  Controller:init(application)
end

Controller = {}

function Controller:init(application)
  self.application = application
  self.application:setRunLoop(function(application) self:runloop(application) end)
end

function Controller:runloop(application)
  --
end
