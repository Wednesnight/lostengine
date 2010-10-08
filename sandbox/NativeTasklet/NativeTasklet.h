#ifndef NATIVETASKLET_H
#define NATIVETASKLET_H

#include "lost/application/Tasklet.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/QueueEntity.h"

struct ToggleClearColor : public lost::application::QueueEntity
{
  void process(const lost::application::Tasklet* sender) const;
};

struct NativeTasklet : public lost::application::Tasklet
{
  NativeTasklet();

  bool startup();
  bool update(double deltaSec);

  void keyDown(lost::application::KeyEventPtr event);

private:
  bool running;

  void toggleClearColor();
};

#endif
