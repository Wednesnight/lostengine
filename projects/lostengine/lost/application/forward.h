/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LOST_APPICATION_FORWARD_H
#define LOST_APPICATION_FORWARD_H

namespace lost
{
  namespace application
  {
    struct Tasklet;
    struct Window;

    struct DragNDropEvent;
    typedef lost::shared_ptr<DragNDropEvent> DragNDropEventPtr;  
        
    struct TaskletEvent;
    typedef lost::shared_ptr<TaskletEvent> TaskletEventPtr;

    struct ResizeEvent;
    typedef lost::shared_ptr<ResizeEvent> ResizeEventPtr;

    struct Application;
    typedef lost::shared_ptr<Application> ApplicationPtr;

    struct ApplicationEvent;
    typedef lost::shared_ptr<ApplicationEvent> ApplicationEventPtr;

    struct KeyEvent;
    typedef lost::shared_ptr<KeyEvent> KeyEventPtr;

    struct MouseEvent;
    typedef lost::shared_ptr<MouseEvent> MouseEventPtr;
    
    struct SpawnTaskletEvent;
    typedef lost::shared_ptr<SpawnTaskletEvent> SpawnTaskletEventPtr;

    struct Queue;
    typedef lost::shared_ptr<Queue> QueuePtr;
    
    struct QueueEvent;
    typedef lost::shared_ptr<QueueEvent> QueueEventPtr;
    
    struct ProcessEvent;
    typedef lost::shared_ptr<ProcessEvent> ProcessEventPtr;

    struct Window;
    struct TaskletConfig;

    struct WindowEvent;
    typedef lost::shared_ptr<WindowEvent> WindowEventPtr;
  
    struct SpawnTaskletEvent;
    typedef lost::shared_ptr<SpawnTaskletEvent> SpawnTaskletEventPtr;

    struct DebugEvent;
    typedef lost::shared_ptr<DebugEvent> DebugEventPtr;
  }
}

#endif
