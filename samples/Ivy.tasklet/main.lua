
function startup()
  require("lost/guiro")  
  require("MainController")
  mainController = MainController()
  mainController:buildUi()
end


function update()
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end