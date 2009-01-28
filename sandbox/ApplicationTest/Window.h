#ifndef WINDOW_H
#define WINDOW_H

#include <boost/shared_ptr.hpp>

#include "gl/Context.h"
#include "lost/math/Vec2.h"
#include "lost/event/EventDispatcher.h"

struct WindowParams
{
public:
  std::string      caption;
  lost::math::Vec2 size;
  lost::math::Vec2 position;

  WindowParams(const std::string& inCaption,
               const lost::math::Vec2& inSize,
               const lost::math::Vec2& inPosition)
  : caption(inCaption),
    size(inSize),
    position(inPosition)
  {
  }
};

struct Window
{
private:
  struct WindowHiddenMembers;
  WindowHiddenMembers* hiddenMembers;
protected:
  WindowParams params;
public:
  boost::shared_ptr<lost::event::EventDispatcher> dispatcher;
  boost::shared_ptr<Context> context;

  Window(const boost::shared_ptr<lost::event::EventDispatcher>& inDispatcher,
         const WindowParams& inParams);
  ~Window();

  void initialize();
  void finalize();

  void close();
};

#endif
