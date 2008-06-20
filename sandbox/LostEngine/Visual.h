#ifndef VISUAL_H
#define VISUAL_H

struct Visual
{
  virtual ~Visual() {}
  virtual void redraw(double timeSinceLastCallSec) = 0;
  virtual void keyboard( int key, bool pressed ) {}
  virtual void mouseButton( int button, bool pressed, lost::math::Vec2 point ) {}
  virtual void mouseMove( lost::math::Vec2 point ) {}
};

#endif
