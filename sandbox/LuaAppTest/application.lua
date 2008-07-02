print("memory usage in kb: "..gcinfo())

require("stuff")

display.width=640
display.height=480
display.redbits=8
display.bluebits=8
display.greenbits=8
display.alphabits=8
display.depthbits=32
display.stencilbits=0
display.fullscreen=false
display.title="das Bunker"

print("-------------")

config["lost.font.flag"] = true
config["lost.bitmap.sets"] = "asd"

print("memory usage in kb: "..gcinfo())

ev = Event("helloEvent")
print(ev.type)

mev = MouseEvent("oogaMouse");
print(mev.type)
print(MouseEvent.MOUSE_UP)
print(MouseEvent.MOUSE_DOWN)
print(MouseEvent.MOUSE_MOVE)


function genericHandler(ev)
  print("Lua got event: "..(ev.type))
end

gh = genericHandler

print("-----testing lua events--------")
eventDispatcher = EventDispatcher()
eventDispatcher:addEventListener(MouseEvent.MOUSE_UP, gh)
eventDispatcher:addEventListener(KeyEvent.KEY_UP, gh)
eventDispatcher:dispatchEvent(MouseEvent(MouseEvent.MOUSE_UP))
eventDispatcher:dispatchEvent(KeyEvent(KeyEvent.KEY_UP))
eventDispatcher:doDispatch()

print("bitmap")
bitmapLoader = lost.bitmap.BitmapLoader()

names = {"ssao.png", "286die.jpg", "firefox logo ktten+tits.jpg"}

function Bitmaps(t)
  result = {}
  for k,name in pairs(t)
  do
    result[name] = bitmapLoader:loadFromResourcePath(name)
  end
  return result
end

--[[res = Bitmaps(names)
for name, bitmap in pairs(res)
do
  print(name.." : "..bitmap.width.." / "..bitmap.height)
end]]

function LostApplication(t)
  print("----- App Helper")
  for k,v in pairs(t)
  do
    print(k)
  end
end




app = LostApplication
{
  bitmaps = Bitmaps
  {
    "ssao.png",
    "286die.jpg",
    "firefox logo ktten+tits.jpg"
  }
}




print("memory usage in kb: "..gcinfo())

