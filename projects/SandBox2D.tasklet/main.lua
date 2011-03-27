function startup()
   gravity = box2d.b2Vec2(0,-10)
   doSleep = true
  world = box2d.b2World(gravity, doSleep)
   groundBodyDef = box2d.b2BodyDef()
  groundBodyDef.position:Set(0.0,-10.0)
   groundBody = world:CreateBody(groundBodyDef)
   groundBox = box2d.b2PolygonShape()
  groundBox:SetAsBox(50.0,10.0)
  groundBody:CreateFixture(groundBox,0)
  
  bodyDef = box2d.b2BodyDef()
  bodyDef.type = box2d.b2_dynamicBody
  bodyDef.position:Set(0,4)
  body = world:CreateBody(bodyDef)
  dynamicBox = box2d.b2PolygonShape()
  dynamicBox:SetAsBox(1,1)
  
  fixtureDef = box2d.b2FixtureDef()
  fixtureDef:setShape(dynamicBox)
--  fixtureDef.shape = dynamicBox
  fixtureDef.density = 1
  fixtureDef.friction = .3
  
  fixture = body:CreateFixture(fixtureDef)
  
  timeStep = 1/60
  velocityIterations = 6
  positionIterations = 2
  
  for i=1,60 do
    world:Step(timeStep,velocityIterations,positionIterations)
    world:ClearForces()
    position = body:GetPosition()
    angle = body:GetAngle()
    log.debug(position.x.." "..position.y.." "..angle)
  end
end

function update()
  
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end