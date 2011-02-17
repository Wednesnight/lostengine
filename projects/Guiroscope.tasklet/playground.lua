return lost.guiro.view.View
{
  subviews =
  {
    lost.guiro.view.View
    {
      id = "control",
      bounds = {"left", "top", "1", 200},
      subviews =
      {
        lost.guiro.view.TextInput
        {
          id = "code",
          bounds = {"left", "top", {"1", -100}, "1"},
          font={"Vera mono", 10},
          multiLine = true,
          text = [[
            return lost.guiro.view.Button
            {
              bounds = {"left", {"top", -50}, "1", 200},
              title = "click me",
              listeners =
              {
                buttonClick = function(event)
                  event.target:title("damn, you really did it!")
                end
              }
            }
          ]]
        },
        lost.guiro.view.Button
        {
          id = "update",
          title = "Update",
          bounds = {{"right", -5}, "center", 90, 200},
          listeners =
          {
            buttonClick = function(event)

              local code = event.target:superview()("code")
              local target = event.target:superview():superview()("target")

              local success, result = pcall(function()
                return assert(loadstring(code:text()))()
              end)

              target:removeAllSubviews()
              if success then
                if result ~= nil and result.isDerivedFrom and result:isDerivedFrom("lost.guiro.view.View") then
                  target:addSubview(result)
                else
                  target:addSubview(lost.guiro.view.Label{bounds = {0, 0, "1", "1"}, text = "Example: return lost.guiro.view.View{}"})
                end
              else
                target:addSubview(lost.guiro.view.Label{bounds = {0, 0, "1", "1"}, text = result})
              end

            end
          }
        }
      }
    },
    lost.guiro.view.View
    {
      id = "target",
      bounds = {0, 0, "1", {"1", -200}}
    }
  }
}