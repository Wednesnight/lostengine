#ifndef L_SIGNAL_H
#define L_SIGNAL_H

struct Signal
{
  Signal();
  virtual ~Signal();
  
  void addListener();
};

#endif