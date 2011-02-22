#ifndef LOST_APPICATION_FORWARD_H
#define LOST_APPICATION_FORWARD_H

namespace lost
{
  namespace application
  {
    struct Tasklet;
    
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
}
}

#endif