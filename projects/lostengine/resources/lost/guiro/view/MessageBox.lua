module("lost.guiro.view", package.seeall)

-- no need for class overhead
MessageBox = {
  _currentWindow = nil,
  _windowQueue = {}
}

-- MessageBox button types
MessageBox.YES = "Yes"
MessageBox.NO = "No"
MessageBox.OK = "OK"
MessageBox.CANCEL = "Cancel"
-- combinations
MessageBox.YESNO = {MessageBox.YES, MessageBox.NO}
MessageBox.YESNOCANCEL = {MessageBox.YES, MessageBox.NO, MessageBox.CANCEL}
MessageBox.OKCANCEL = {MessageBox.OK, MessageBox.CANCEL}

-- default button padding + size
MessageBox.buttonPadding = 5
MessageBox.buttonSize = lost.math.Vec2(75, 20)

-- internal, create (list of) lost.guiro.view.Button
function MessageBox._buttons(buttons, callback, offset)
  -- initialize button offset
  if type(offset) ~= "number" then
    offset = MessageBox.buttonPadding
  end

  -- create button
  if type(buttons) == "string" then
    local button = lost.guiro.view.Button
    {
      id = buttons,
      bounds = {{"right", -offset}, {"bottom", MessageBox.buttonPadding}, MessageBox.buttonSize.x, MessageBox.buttonSize.y},
      title = buttons,
      listeners =
      {
        buttonClick = function(event)
          if type(callback) == "function" then
            callback(event.target.id)
          end
          event.target:superview():superview():close()
        end
      }
    }
    return button

  -- create button list
  elseif type(buttons) == "table" then
    local buttonList = {}
    local offset = MessageBox.buttonPadding
    for k,button in next,buttons do
      table.insert(buttonList, MessageBox._buttons(button, callback, offset))
      offset = offset + MessageBox.buttonPadding + MessageBox.buttonSize.x
    end
    return unpack(buttonList)
  end
end

function MessageBox._nextMessageBox(event)
  if #MessageBox._windowQueue > 0 then
    log.debug("open from queue")
    MessageBox._currentWindow = table.remove(MessageBox._windowQueue, 1)
    MessageBox._currentWindow:open()
  else
    log.debug("close")
    MessageBox._currentWindow = nil
  end
end

-- show a message
-- optional: call callback on close, first parameter is always MessageBox.OK
function MessageBox.message(title, message, callback)
  local window = lost.guiro.view.Window
  {
    bounds = {"center", "center", 300, 150},
    title = title,
    modal = true,
    closeButton = false,
    resizable = false,
    subviews =
    {
      lost.guiro.view.Label
      {
        bounds = {"left", "top", "1", {"1", -(MessageBox.buttonPadding*2 + MessageBox.buttonSize.y)}},
        text = message
      },
      MessageBox._buttons(MessageBox.OK, callback)
    },
    listeners =
    {
      windowClose = MessageBox._nextMessageBox
    }
  }
  if MessageBox._currentWindow == nil then
    MessageBox._currentWindow = window
    MessageBox._currentWindow:open()
  else
    table.insert(MessageBox._windowQueue, window)
  end
end

-- show a message that has to be confirmed
-- optional: call callback on close, first parameter is MessageBox.YES or MessageBox.NO
function MessageBox.confirm(title, message, callback)
  local window = lost.guiro.view.Window
  {
    bounds = {"center", "center", 300, 150},
    title = title,
    modal = true,
    closeButton = false,
    resizable = false,
    subviews =
    {
      lost.guiro.view.Label
      {
        bounds = {"left", "top", "1", {"1", -(MessageBox.buttonPadding*2 + MessageBox.buttonSize.y)}},
        text = message
      },
      MessageBox._buttons(MessageBox.YESNO, callback)
    },
    listeners =
    {
      windowClose = MessageBox._nextMessageBox
    }
  }
  if MessageBox._currentWindow == nil then
    log.debug("open")
    MessageBox._currentWindow = window
    MessageBox._currentWindow:open()
  else
    log.debug("queue")
    table.insert(MessageBox._windowQueue, window)
  end
end
