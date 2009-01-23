#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "lost/event/Event.h"
#include "gl/Context.h"

struct MainLoop
{
  bool running;
  
  void quitEventHandler(boost::shared_ptr<lost::event::Event> event)
  {
    running = false;
  }
  
  virtual void process()
  {
  }
  
  void run()
  {
    running = true;
    while(running)
    {
      process();
    }
  }    
};


struct FunctorMainLoop : public MainLoop
{
  boost::function<void (void)> func;
  
  FunctorMainLoop(const boost::function<void (void)>& f)
  : func(f)
  {
  }
  
  virtual void process()
  {
    func();
  }
};

#endif
