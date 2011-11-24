#include "lost/lua/bindings/LostApplication.h"
#include "lost/lua/lua.h"

#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/DragNDropEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/SpawnTaskletEvent.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/Tasklet.h"
#include "lost/application/Window.h"
#include "lost/application/Queue.h"
#include "lost/application/QueueEntity.h"
#include "lost/application/QueueEntityLua.h"
#include "lost/application/DebugEvent.h"
#include "lost/rg/Node.h"
#include "lost/resource/Loader.h"
#include "lost/resource/ApplicationResourceRepository.h"
#include "lost/resource/FilesystemRepository.h"
#include "lost/gl/Context.h"
#include "lost/event/EventDispatcher.h"
#include "lost/event/Event.h"
#include "lost/font/FontManager.h"
#include "lost/lua/State.h"

using namespace lost::application;
using namespace lost::event;
using namespace lost::resource;

namespace slub {

  template<typename T>
  struct field<T, KeyCode> : public abstract_field {
    
    KeyCode T::*m;
    
    field(KeyCode T::*m) : m(m) {
    }
    
    int get(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      lua_pushinteger(L, t->ref->*m);
      return 1;
    }
    
    int set(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      t->ref->*m = (KeyCode) luaL_checkinteger(L, -1);
      return 0;
    }
    
  };
  
}

namespace lost
{
  namespace lua
  {

    void LostApplicationApplication(lua_State* state)
    {
      slub::package(state, "lost").package("application")
        .clazz<Application>("Application")
          .method("showMouse", &Application::showMouse)
          .field("eventDispatcher", &Application::eventDispatcher)
// TODO: stl_iterator
//          .field("tasklets", &Application::tasklets)
          .function("getInstance", (ApplicationPtr(*)())&Application::getInstance);
    }

    void LostApplicationApplicationEvent(lua_State* state)
    {
      slub::package(state, "lost").package("application")
        .clazz<ApplicationEvent>("ApplicationEvent")
          .extends<Event>()
          .function("create", &ApplicationEvent::create)
          .constant("RUN", ApplicationEvent::RUN())
          .constant("QUIT", ApplicationEvent::QUIT());
    }

    void LostApplicationDragNDropEvent(lua_State* state)
    {
      slub::package(state, "lost").package("application")
        .clazz<DragNDropEvent>("DragNDropEvent")
          .extends<Event>()
          .field("window", &DragNDropEvent::window)
          .field("pos", &DragNDropEvent::pos)
          .field("absPos", &DragNDropEvent::absPos)
          .method("numPaths",&DragNDropEvent::numPaths)
          .method("getPath",&DragNDropEvent::getPath)
          .constant("DRAG_ENTER", DragNDropEvent::DRAG_ENTER())
          .constant("DRAG_UPDATE", DragNDropEvent::DRAG_UPDATE())
          .constant("DRAG_LEAVE", DragNDropEvent::DRAG_LEAVE())
          .constant("DROP", DragNDropEvent::DROP());
    }
    
    void LostApplicationKeyEvent(lua_State* state)
    {
      slub::package application = slub::package(state, "lost").package("application");

      application.clazz<KeyEvent>("KeyEvent")
        .extends<Event>()
        .field("key", &KeyEvent::key)
        .field("character", &KeyEvent::character)
        .field("pressed", &KeyEvent::pressed)
        .field("repeat", &KeyEvent::repeat)
        .field("ctrlDown", &KeyEvent::ctrlDown)
        .field("shiftDown", &KeyEvent::shiftDown)
        .field("altDown", &KeyEvent::altDown)
        .field("specialDown", &KeyEvent::specialDown)
        .constant("KEY_UP", KeyEvent::KEY_UP())
        .constant("KEY_DOWN", KeyEvent::KEY_DOWN());

      application
        .enumerated("K_UNKNOWN", K_UNKNOWN)

        .enumerated("K_BACKSPACE", K_BACKSPACE)
        .enumerated("K_TAB", K_TAB)
        .enumerated("K_ENTER", K_ENTER)
        .enumerated("K_PAUSE", K_PAUSE)
        .enumerated("K_ESCAPE", K_ESCAPE)
        .enumerated("K_SPACE", K_SPACE)
      
        .enumerated("K_LEFT", K_LEFT)
        .enumerated("K_UP", K_UP)
        .enumerated("K_RIGHT", K_RIGHT)
        .enumerated("K_DOWN", K_DOWN)
      
        .enumerated("K_0", K_0)
        .enumerated("K_1", K_1)
        .enumerated("K_2", K_2)
        .enumerated("K_3", K_3)
        .enumerated("K_4", K_4)
        .enumerated("K_5", K_5)
        .enumerated("K_6", K_6)
        .enumerated("K_7", K_7)
        .enumerated("K_8", K_8)
        .enumerated("K_9", K_9)
      
        .enumerated("K_A", K_A)
        .enumerated("K_B", K_B)
        .enumerated("K_C", K_C)
        .enumerated("K_D", K_D)
        .enumerated("K_E", K_E)
        .enumerated("K_F", K_F)
        .enumerated("K_G", K_G)
        .enumerated("K_H", K_H)
        .enumerated("K_I", K_I)
        .enumerated("K_J", K_J)
        .enumerated("K_K", K_K)
        .enumerated("K_L", K_L)
        .enumerated("K_M", K_M)
        .enumerated("K_N", K_N)
        .enumerated("K_O", K_O)
        .enumerated("K_P", K_P)
        .enumerated("K_Q", K_Q)
        .enumerated("K_R", K_R)
        .enumerated("K_S", K_S)
        .enumerated("K_T", K_T)
        .enumerated("K_U", K_U)
        .enumerated("K_V", K_V)
        .enumerated("K_W", K_W)
        .enumerated("K_X", K_X)
        .enumerated("K_Y", K_Y)
        .enumerated("K_Z", K_Z)
      
        .enumerated("K_NUMPAD_0", K_NUMPAD_0)
        .enumerated("K_NUMPAD_1", K_NUMPAD_1)
        .enumerated("K_NUMPAD_2", K_NUMPAD_2)
        .enumerated("K_NUMPAD_3", K_NUMPAD_3)
        .enumerated("K_NUMPAD_4", K_NUMPAD_4)
        .enumerated("K_NUMPAD_5", K_NUMPAD_5)
        .enumerated("K_NUMPAD_6", K_NUMPAD_6)
        .enumerated("K_NUMPAD_7", K_NUMPAD_7)
        .enumerated("K_NUMPAD_8", K_NUMPAD_8)
        .enumerated("K_NUMPAD_9", K_NUMPAD_9)
      
        .enumerated("K_NUMPAD_PLUS", K_NUMPAD_PLUS)
        .enumerated("K_NUMPAD_MINUS", K_NUMPAD_MINUS)
      
        .enumerated("K_F1", K_F1)
        .enumerated("K_F2", K_F2)
        .enumerated("K_F3", K_F3)
        .enumerated("K_F4", K_F4)
        .enumerated("K_F5", K_F5)
        .enumerated("K_F6", K_F6)
        .enumerated("K_F7", K_F7)
        .enumerated("K_F8", K_F8)
        .enumerated("K_F9", K_F9)
        .enumerated("K_F10", K_F10)
        .enumerated("K_F11", K_F11)
        .enumerated("K_F12", K_F12)
      
        .enumerated("K_DELETE", K_DELETE)
        .enumerated("K_INSERT", K_INSERT)
      
        .enumerated("K_HOME", K_HOME)
        .enumerated("K_END", K_END)
        .enumerated("K_PAGEUP", K_PAGEUP)
        .enumerated("K_PAGEDOWN", K_PAGEDOWN)
      
        .enumerated("K_RSHIFT", K_RSHIFT)
        .enumerated("K_LSHIFT", K_LSHIFT)
        .enumerated("K_RCTRL", K_RCTRL)
        .enumerated("K_LCTRL", K_LCTRL);
    }

    void LostApplicationMouseEvent(lua_State* state)
    {
      slub::package application = slub::package(state, "lost").package("application");

      application.clazz<MouseEvent>("MouseEvent")
        .extends<Event>()
        .field("pos", &MouseEvent::pos)
        .field("absPos", &MouseEvent::absPos)
        .field("button", &MouseEvent::button)
        .field("pressed", &MouseEvent::pressed)
        .field("scrollDelta", &MouseEvent::scrollDelta)
        .constant("MOUSE_UP", MouseEvent::MOUSE_UP())
        .constant("MOUSE_DOWN", MouseEvent::MOUSE_DOWN())
        .constant("MOUSE_MOVE", MouseEvent::MOUSE_MOVE())
        .constant("MOUSE_SCROLL", MouseEvent::MOUSE_SCROLL())
        .constant("MOUSE_UP_INSIDE", MouseEvent::MOUSE_UP_INSIDE())
        .constant("MOUSE_UP_OUTSIDE", MouseEvent::MOUSE_UP_OUTSIDE())
        .constant("MOUSE_ENTER", MouseEvent::MOUSE_ENTER())
        .constant("MOUSE_LEAVE", MouseEvent::MOUSE_LEAVE());

      application
        .enumerated("MB_UNKNOWN", MB_UNKNOWN)
        .enumerated("MB_LEFT", MB_LEFT)
        .enumerated("MB_RIGHT", MB_RIGHT)
        .enumerated("MB_MIDDLE", MB_MIDDLE);
    }

    void LostApplicationResizeEvent(lua_State* state)
    {
      slub::package(state, "lost").package("application")
        .clazz<ResizeEvent>("ResizeEvent")
          .extends<Event>()
          .field("width", &ResizeEvent::width)
          .field("height", &ResizeEvent::height)
          .constant("TASKLET_WINDOW_RESIZE", ResizeEvent::TASKLET_WINDOW_RESIZE());
    }

    int Tasklet_id(Tasklet* tasklet) {
      return *(int*)tasklet;
    }

    void Tasklet_id(Tasklet* tasklet, int i) {
      // not implemented
    }
    
    void LostApplicationTasklet(lua_State* state)
    {
      slub::package(state, "lost").package("application")
        .clazz<Tasklet>("Tasklet")
          .field("eventDispatcher", &Tasklet::eventDispatcher)
          .field("scheduler", &Tasklet::scheduler)
          .field("name", &Tasklet::name)
          .field("loader", &Tasklet::loader)
          .field("lua", &Tasklet::lua)
          .field("window", &Tasklet::window)
          .field("waitForEvents", &Tasklet::waitForEvents)
          .field("updateQueue", &Tasklet::updateQueue)
          .field("clearNode", &Tasklet::clearNode)
          .field("renderNode", &Tasklet::renderNode)
          .field("uiNode", &Tasklet::uiNode)
          .field("fontManager", &Tasklet::fontManager)
          .field("running", &Tasklet::running)
//          .method("dispatchApplicationEvent", &Tasklet::dispatchApplicationEvent)
          .method("spawnTasklet",&Tasklet::spawnTasklet)
          .method("getClipboardString", &Tasklet::getClipboardString)
          .method("setClipboardString", &Tasklet::setClipboardString)
          .field("id", (int(*)(Tasklet*)) &Tasklet_id, (void(*)(Tasklet*, int)) &Tasklet_id);
    }

/*    EventPtr SpawnTaskletEvent_create(const string& absolutePath)
    {
      LoaderPtr loader = Loader::create();
      loader->addRepository(FilesystemRepository::create(absolutePath));
      loader->addRepository(ApplicationResourceRepository::create());
      return SpawnTaskletEvent::create(loader);
    }

    void LostApplicationSpawnTaskletEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<SpawnTaskletEvent, Event>("SpawnTaskletEvent")
            .scope
            [
              def("create", &SpawnTaskletEvent_create)
            ]
        ]
      ];
    }*/

    void LostApplicationTaskletEvent(lua_State* state)
    {
      slub::package(state, "lost").package("application")
        .clazz<TaskletEvent>("TaskletEvent")
          .extends<Event>()
          .field("tasklet", &TaskletEvent::tasklet)
          .constant("START", TaskletEvent::START())
          .constant("DONE", TaskletEvent::DONE());
    }
    
    void LostApplicationWindow(lua_State* state)
    {
      slub::package(state, "lost").package("application")
        .clazz<Window>("Window")
          .method("open", &Window::open)
          .method("close", &Window::close)
          .field("context", &Window::context)
          .field("size", &Window::size);
    }

    void LostApplicationQueue_queue(Queue* queue, const slub::reference& targetMethod,
                                    const slub::reference& targetObject)
    {
      queue->queue(new QueueEntityLua(targetObject, targetMethod));
    }

    void LostApplicationQueue(lua_State* state)
    {
      slub::package(state, "lost").package("application")
        .clazz<Queue>("Queue")
          .method("queue", &LostApplicationQueue_queue);
    }

    EventPtr DebugEvent_create(const Type& type, Tasklet* tasklet)
    {
      return DebugEvent::create(type, tasklet);
    }

    void LostApplicationDebugEvent(lua_State* state)
    {
      slub::package application = slub::package(state, "lost").package("application");

      application.clazz<DebugEvent>("DebugEvent")
        .extends<Event>()
        .field("mode", &DebugEvent::mode)
        .field("tasklet", &DebugEvent::tasklet)
        .field("info", &DebugEvent::info)
        .function("create", &DebugEvent_create)
        .constant("CMD_MEM_INFO", DebugEvent::CMD_MEM_INFO())
        .constant("CMD_PAUSE", DebugEvent::CMD_PAUSE())
        .constant("CMD_CONTINUE", DebugEvent::CMD_CONTINUE())
        .constant("MEM_INFO", DebugEvent::MEM_INFO())
        .constant("PAUSE", DebugEvent::PAUSE());
      
      application.package("DebugEvent")
        .clazz<DebugEvent::DebugInfo>("DebugInfo")
          .field("debug", &DebugEvent::DebugInfo::debug)
          .field("memSize", &DebugEvent::DebugInfo::memSize);
    }
    
    void LostApplication(lua_State* state)
    {
      LostApplicationApplication(state);
      LostApplicationApplicationEvent(state);
      LostApplicationDragNDropEvent(state);
      LostApplicationKeyEvent(state);
      LostApplicationMouseEvent(state);
      LostApplicationResizeEvent(state);
      LostApplicationTasklet(state);
      LostApplicationTaskletEvent(state);
      LostApplicationWindow(state);
      LostApplicationQueue(state);
      LostApplicationDebugEvent(state);
    }
    
  }
}
