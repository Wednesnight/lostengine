return function() -- constructor. everything outside this function inside this file shoudl be made local
  log.debug("PhysicsManager constructor called")
  
  local instance = 
  {
    constructor = function(self)
      self.gravity = box2d.b2Vec2(0,-10)
      self.doSleep = true
      self.debugDraw = lost.box2d.DebugDraw()
      self.world = box2d.b2World(self.gravity, self.doSleep)
      self.world:SetDebugDraw(self.debugDraw)
      self.timeStep = 1/30
      self.velocityIterations = 6
      self.positionIterations = 2
      self.accu = 0
    end,
    
    update = function(self, dt)
      self.accu = self.accu + dt
      log.debug("---")
      if self.accu >= self.timeStep then
        while self.accu >= self.timeStep do
          log.debug("+++ updating physics")
          self.world:Step(self.timeStep,self.velocityIterations,self.positionIterations)
          self.world:ClearForces()
          self.world:DrawDebugData()
          self.accu = self.accu - self.timeStep
        end
      end
    end    
  }
  
  instance:constructor()
  return instance
end
