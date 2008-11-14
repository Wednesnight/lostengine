#ifndef LOST_GL_CONTEXT_H
#define LOST_GL_CONTEXT_H

#include <list>
#include <boost/shared_ptr.hpp>
#include "lost/gl/State.h"

namespace lost { namespace common{ struct DisplayAttributes;}}
namespace lost { namespace gl{ struct State;}}
namespace lost{ namespace math{ struct Vec3; struct Vec2;  struct AABB; struct Rect;} }
namespace common{ struct Color; };

namespace lost
{
  namespace gl
  {
    struct Texture;

    struct Context
    {
    private:
      std::list<boost::shared_ptr<State> > stateStack;
      boost::shared_ptr<common::DisplayAttributes> displayAttributes;

    public:
//      boost::shared_ptr<State> state;

      /** creates a new gl::Context object.
       *
       * @param inDisplayAttributes displayAttrbutes object that is maintained and updated by application.
       *                            This is required for various functions and should always be kept up to date.
       */
      Context(boost::shared_ptr<common::DisplayAttributes> inDisplayAttributes);
      virtual ~Context();

      static boost::shared_ptr<State> newState();
      boost::shared_ptr<State> copyState();
//      void pushState();
        void Context::pushState(const boost::shared_ptr<State>& inState);        
        void setState(const boost::shared_ptr<State>& oldState, const boost::shared_ptr<State>& newState);
      void popState();
      void clear(GLbitfield flags);
      
      // set helpers
      void setColor(const lost::common::Color& col);
      
      void set2DProjection(const lost::math::Vec2& offset, const lost::math::Vec2& dimension);
      void set3DProjection(const lost::math::Vec3& eye,
                           const lost::math::Vec3& at,
                           const lost::math::Vec3& up,
                           const float& fovy,
                           const lost::math::Vec2& depth);      
      boost::shared_ptr<State>& defaultVertexState();
      
      
      // drawing
      void drawLine(const lost::math::Vec2& start, const lost::math::Vec2& end);
      void drawLine(const lost::math::Vec3& start, const lost::math::Vec3& end);
      void drawPoint(const lost::math::Vec2& point);

      // these probably need better render state support, might crash
      void drawRectOutline(const lost::math::Rect& rect);
      void drawRectFilled(const lost::math::Rect& rect);
      // don't forget glEnable(GL_TEXTURE_2D);
      // rect is drawn counterclockwise
      // requires vertex arrays, texture arrays, index arrays
      // FIXME: needs serious optimisation/rethinking, but at least it works
      void drawRectTextured(const lost::math::Rect& rect,
                                   boost::shared_ptr<const lost::gl::Texture> tex,
                                   bool flip=true);
      void drawAABB(const lost::math::AABB& box);
      void drawAxes(const lost::math::Vec3& length);  

      /** Uses glReadPixels to retrieve the current framebuffer data as rgba and saves it
       * as a tga file to the specified file path.
       *
       * @param fullPathName full path name of file to be saved. You must ensure that the location is writable.
       *        
       */
      void writeScreenshot(const std::string& fullPathName,
                          bool withAlphaChannel=false);

      std::string getVersion();
    };
        
  }
}

#endif
