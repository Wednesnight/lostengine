#ifndef LOST_GUIRO_CONTROL_VIEW_H
#define LOST_GUIRO_CONTROL_VIEW_H

#include <string>
#include <list>
#include <map>
#include <algorithm>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/bind.hpp>

#include "lost/math/Vec2.h"
#include "lost/math/Rect.h"
#include "lost/common/Color.h"
#include "lost/gl/Scissor.h"
#include "lost/gl/gl.h"

#include "lost/object/ObjectReferenceContainer.h"
#include "lost/guiro/common/Area.h"

#include "lost/guiro/common/Clonable.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/guiro/common/ViewPath.h"

#include "lost/common/Profiler.h"

namespace lost
{
  namespace guiro
  {
    namespace control
    {

    /**
      Interface for all visual and non-visual elements that are used to hold content or
      funtionality within the user interface.
     **/
      struct View : public boost::enable_shared_from_this<lost::guiro::control::View>,
                    public object::ObjectReferenceContainer,
                    public common::Clonable
    {
      typedef std::map<std::string, boost::shared_ptr<View> > ChildMap;
      typedef std::list<boost::shared_ptr<View> >             ChildList;

      typedef boost::signal<void ( const boost::shared_ptr<View>& inView, const lost::math::Vec2& inPoint )> MoveStartSignal;
      typedef boost::signal<void ( const boost::shared_ptr<View>& inView, const lost::math::Vec2& inPoint )> MoveStopSignal;

    protected:
      lost::math::Rect                   rect;              /** This is the snapshot of the area in global screen coordinates. You should ALWAYS use this for drawing. **/
      ChildMap                           children;          /** Sorted, associative list of child Views. **/
    public:
      std::string                        name;              /** Unique name representing the View in any child hierarchy. **/
      lost::guiro::common::Area          area;              /** This is the parametrized rectangular area the view occupies inside its parent. **/
      ChildList                          unsortedChildren;  /** Unsorted list of child Views, represents the order in which the children were added. **/
      lost::common::Color                color;             /** Color used for overlays. **/
      boost::weak_ptr<View>              parent;            /** The View's parent View. **/
      bool                               visible;           /** Visible state. **/
      bool                               focused;           /** Focused state. **/
      bool                               movable;           /** Movable state. **/
      bool                               sizable;           /** Sizable state. **/
      bool                               draggable;         /** Draggable state. **/
      bool                               controllable;      /** Controllable state. **/
      MoveStartSignal                    moveStart;         /** MoveStart signal, usually called by the InputManager. **/
      MoveStopSignal                     moveStop;          /** MoveStop signal, usually called by the InputManager. **/


      /**
       constructor

       @param inName Name identifiyng the control in the (parent's) children hierarchy.
       **/
      View( const std::string& inName )
      : name(inName),
        color(lost::common::whiteColor),
        visible(true),
        focused(false),
        movable(false),
        sizable(false),
        draggable(false),
        controllable(false)
      {
        // default ist full area of parent
        area = make_area(lost::guiro::common::xabs(0), lost::guiro::common::yabs(0), lost::guiro::common::wrel(1), lost::guiro::common::hrel(1));
      }

     /**
      Copy constructor

      @param inView
      **/
      View( View& inView )
        : name(inView.name),
          area(inView.area),
          color(inView.color),
          visible(inView.visible),
          focused(false),
          movable(inView.movable),
          sizable(inView.sizable),
          draggable(inView.draggable),
          controllable(inView.controllable)
      {
      }

      /**
       destructor
       **/
      virtual ~View() {}

      /**
       Deletes actual child hierarchy and clones the child hierarchy from inView

       @param inView
       **/
      void childrenCopyFrom( boost::shared_ptr<View> inView )
      {
        PROFILE_METHOD();

        unsortedChildren.clear();
        children.clear();

        ChildList::iterator child;
        for (child = inView->unsortedChildren.begin(); child != inView->unsortedChildren.end(); ++child)
        {
          boost::shared_ptr<View> clone( (*child)->clone() );
          add( clone );
        }

        std::string list( name );
        for (child = unsortedChildren.begin(); child != unsortedChildren.end(); ++child)
        {
          list.append(", ").append( (*child)->name );
        }
        DOUT( list );
      }

      /**
       Deletes actual child hierarchy and applies the child hierarchy from inView without cloning

       @param inView
       **/
      void childrenApplyFrom( boost::shared_ptr<View> inView )
      {
        PROFILE_METHOD();

        unsortedChildren.clear();
        children.clear();

        ChildList::iterator child;
        for (child = inView->unsortedChildren.begin(); child != inView->unsortedChildren.end(); ++child)
        {
          add( (*child) );
        }

        std::string list( name );
        for (child = unsortedChildren.begin(); child != unsortedChildren.end(); ++child)
        {
          list.append(", ").append( (*child)->name );
        }
        DOUT( list );
      }

      /**
       Calls the copy constructor with the correct type
       **/
      virtual View* clone()
      {
        PROFILE_METHOD();
        return new View(*this);
      }

      boost::shared_ptr<View>& operator[]( const char* inPath )
      {
        PROFILE_METHOD();
        return (*this)[std::string(inPath)];
      }

      boost::shared_ptr<View>& operator[]( const std::string& inPath )
      {
        PROFILE_METHOD();
        return (*this)[lost::guiro::common::ViewPath(inPath)];
      }

      /**
       Returns the requested View if it is found in the View's children hierarchy.
       Raises std::invalid_argument if View was not found.

       @param inPath Path identifiyng the requested control in the children hierarchy.

       @return boost::shared_ptr<lost::guiro::View>&
       **/
      virtual boost::shared_ptr<View>& operator[]( const lost::guiro::common::ViewPath& inPath )
      {
        PROFILE_METHOD();
        if(inPath.size() == 0)
        {
          throw std::runtime_error("don't know what to do with zero element ViewPath");
        }
        else if(inPath.size() == 1)
        {
          ChildMap::iterator pos = children.find( inPath.front() );
          if (pos != children.end())
          {
            return pos->second;
          }
          throw std::invalid_argument( "lost::guiro::View[]: child not found: "+inPath.front()+" in View: "+name );
        }
        else
        {
          // FIXME: if this proves to be too much of a performance hog, try not to remove the front,
          //        but use an index on the elements to designate which element is the current front.
          common::ViewPath newpath = inPath;
          std::string current = inPath.front();
          newpath.popFront();
          if(children.find(current) == children.end()) { throw std::runtime_error("tried to access nonexistent viewpath part: "+current); }
          return (*(children[current]))[newpath];
        }
      }

      template<typename CONTROL>
      boost::shared_ptr<CONTROL> get(const std::string& inViewPath)
      {
        PROFILE_METHOD();
        boost::shared_ptr<CONTROL> result = boost::dynamic_pointer_cast<CONTROL>((*this)[inViewPath]);
        if(!result)
        {
          throw std::runtime_error("couldn't cast "+inViewPath+" to desired type");
        }
        return result;
      }

      /**
       Adds the given View to the children hierarchy. The View will be inserted into the
       associative and unsorted list. Views will be drawn in the order they are added into
       the unsorted list.

       @param inView The View to be added into the children hierarchy.

       @return void
       **/
      virtual void add( const boost::shared_ptr<View> inView )
      {
        PROFILE_METHOD();
        if(!inView) { WOUT("tried to add NULL view"); return; }
        if (children.find( inView->name ) != children.end())
          throw std::runtime_error( "lost::guiro::View::add(): child already exists: "+inView->name );
        children[inView->name] = inView;
        unsortedChildren.push_back( inView );
        inView->setParent( shared_from_this() );
      }

      /**
       Returns the View's rect. The rect always holds global screen coordinates.
       You should ALWAYS use this for drawing.

       @return const lost::math::Rect&
       **/
      const lost::math::Rect& getRect()
      {
        return rect;
      }

      /**
        Call this whenever the parent area or the own area has changed.

        @return void
       **/
      virtual void updateGeometry()
      {
        PROFILE_METHOD();

        boost::shared_ptr<View> parentview = parent.lock();
        if (parentview)
        {
          lost::math::Rect parentRect = parentview->getRect();
          rect = area(parentRect);
        }
        updateChildGeometry();
      }

      /**
        Call this to propagate your own area changes to your children.

        @see updateGeometry()

        @return void
       **/
      virtual void updateChildGeometry()
      {
        PROFILE_METHOD();

        ChildList::iterator child;
        for(child = unsortedChildren.begin(); child != unsortedChildren.end(); ++child)
        {
          (*child)->updateGeometry();
        }
      }

      /**
        Sets the associated area. Notice that the geometry will be updated with every call.

        @see updateGeometry()

        @param x Global screen X-coordinate.
        @param y Global screen Y-coordinates.
        @param w Global screen width.
        @param h Global screen height.

        @return void
       **/
      void setArea(float x, float y, float w, float h)
      {
        area = make_area(common::xabs(x), common::yabs(y), common::wabs(w), common::habs(h));
        updateGeometry();
      }

      /**
        Sets the associated area. Notice that the geometry will be updated with every call.

        @see updateGeometry()

        @param inArea Prepared lost::guiro::Area object.

        @return void
       **/
      void setArea(const lost::guiro::common::Area& inArea)
      {
        area = inArea;
        updateGeometry();
      }

      /**
        Gets the associated area.

        @return lost::guiro::Area&
       **/
      lost::guiro::common::Area& getArea()
      {
        return area;
      }

      /**
        Sets the area's X-coordinate.

        @see updateGeometry()

        @param inX          Relative X-coordinate.
        @param inUpdateRect Controls if updateGeometry() is called.

        @return void
       **/
      void setX(float inX, bool inUpdateRect=true)
      {
        area.x.reset(new lost::guiro::common::xabs(inX));
        if(inUpdateRect) updateGeometry();
      }

      /**
        Sets the area's Y-coordinate.

        @see updateGeometry()

        @param inY          Relative Y-coordinate.
        @param inUpdateRect Controls if updateGeometry() is called.

        @return void
       **/
      void setY(float inY, bool inUpdateRect=true)
      {
        area.y.reset(new lost::guiro::common::yabs(inY));
        if(inUpdateRect) updateGeometry();
      }

      /**
        Sets the area's coordinates. Notice that the geometry will be updated with every call.

        @see updateGeometry()

        @param inX Relative X-coordinate.
        @param inY Relative Y-coordinate.

        @return void
       **/
      void setPos(float inX, float inY)
      {
        setX(inX, false); // save one potentially expensive recalculation of geometry here
        setY(inY, true);  // and trigger it in the last step of the update
      }

      /**
        Sets the area's X-coordinate. Notice that the geometry will be updated with every call.

        @see updateGeometry()

        @param inX Prepared Dimension object.

        @return void
       **/
      template<typename DIM>
      void setX(const DIM& inX)
      {
        area.x.reset(new DIM(inX));
        updateGeometry();
      }

      /**
        Sets the area's Y-coordinate. Notice that the geometry will be updated with every call.

        @see updateGeometry()

        @param inY Prepared Dimension object.

        @return void
       **/
      template<typename DIM>
      void setY(const DIM& inY)
      {
        area.y.reset(new DIM(inY));
        updateGeometry();
      }

      /**
        Draws the View and all it's children. The children are drawn in the order they were added.

        @param parentRect  Rect of the View's parent View.
        @param parentColor Color of the View's parent View.

        @return void
       **/
      virtual void draw(const lost::math::Rect& parentRect, const lost::common::Color& parentColor )
      {
        PROFILE_METHOD();
        if (getVisible())
        {
          lost::math::Rect clippedRect = rect;
          clippedRect.clipTo( parentRect );
          lost::common::Color currentColor = color * parentColor;
          lost::gl::Scissor scissor( clippedRect );

          ChildList::iterator child;
          for (child = unsortedChildren.begin(); child != unsortedChildren.end(); ++child)
          {
            (*child)->draw(clippedRect, currentColor );
          }
        }
      }

      /**
        Resolve any ObjectReferences in this or any child View.

        @param inResolver Prepared resolver object used to resolve objects.

        @return void
       **/
      virtual void resolve( object::ObjectReferenceResolver& inResolver )
      {
        PROFILE_METHOD();

        ChildList::iterator child;
        for (child = unsortedChildren.begin(); child != unsortedChildren.end(); ++child)
        {
          DOUT( "resolving child: "+ (*child)->name );
          (*child)->resolve( inResolver );
        }
      }

      /**
        Update data.

        @param timeSinceLastCallSec Time since last call in seconds.

        @return void
       **/
      virtual void update( double timeSinceLastCallSec )
      {
        PROFILE_METHOD();

        ChildList::iterator child;
        for (child = unsortedChildren.begin(); child != unsortedChildren.end(); ++child)
          (*child)->update( timeSinceLastCallSec );
      }

      /**
        Usually gets called by the InputManager to signal a focus change.

        @param inFocus Controls whether the View gets or loses focus.

        @return void
       **/
      virtual void focus( bool inFocus )
      {
        PROFILE_METHOD();

        focused = inFocus;
        if (boost::shared_ptr<View> parentView = parent.lock())
        {
          parentView->focus( inFocus );
          if (focused)
          {
            ChildList& parentChildren = parentView->getChildList();
            parentChildren.remove( shared_from_this() );
            parentChildren.push_back( shared_from_this() );
          }
        }
      }

      /**
        Sets the View's parent.

        @param inView New parent View.

        @return void
       **/
      void setParent( const boost::shared_ptr<View>& inView )
      {
        parent = boost::weak_ptr<View>( inView );
      }

      /**
        Sets the visible state.

        @param inVisible Visible flag.

        @return void
       **/
      void setVisible( const bool inVisible )
      {
        visible = inVisible;
      }

      /**
        Returns the visible state.

        @return bool
       **/
      bool getVisible()
      {
        bool result = visible;
        if (visible)
        {
          if (boost::shared_ptr<View> parentView = parent.lock())
            result = parentView->getVisible();
        }
        return result;
      }

      /**
        Sets the color.

        @param inColor Color.

        @return void
       **/
      void setColor( const lost::common::Color& inColor )
      {
        color = inColor;
      }

      /**
        Sets the movable state.

        @param inMovable Movable flag.

        @return void
       **/
      void setMovable( const bool inMovable )
      {
        movable = inMovable;
      }

      /**
        Returns the unsorted child hierarchy.

        @return ChildList&
       **/
      ChildList& getChildList()
      {
        return unsortedChildren;
      }

      /**
        Performs a hit test on the View and all of it's children. Every custom View has
        to implement it's own hit test if the hit area differs from the rect.

        @param inPoint Coordinates to perform the hit test on. Have to be global screen coordinates.

        @return bool
       **/
      virtual bool hitTest( const lost::math::Vec2& inPoint )
      {
        PROFILE_METHOD();

        return controllable && getVisible() && rect.contains(inPoint);
      }

      /**
        Usually gets called by the InputManager to signal keyboard input.

        @see lost::guiro::InputManager::keyboardEvent
       **/
      virtual void keyboardEvent( const lost::application::KeyEvent& inEvent )
      {
      }

      /**
        Usually gets called by the InputManager to signal mouse input.

        @see lost::guiro::InputManager::mouseButtonEvent
       **/
      virtual void mouseButtonEvent( const lost::application::MouseEvent& inEvent )
      {
      }

      /**
        Usually gets called by the InputManager to signal mouse input.

        @see lost::guiro::InputManager::mouseMoveEvent
       **/
      virtual void mouseMoveEvent( const lost::application::MouseEvent& inEvent )
      {
      }

      virtual void mouseEnter()
      {
        PROFILE_METHOD();

        boost::shared_ptr<View> view = parent.lock();
        if (view) view->mouseEnter();
      }

      virtual void mouseLeave()
      {
        PROFILE_METHOD();

        boost::shared_ptr<View> view = parent.lock();
        if (view) view->mouseLeave();
      }

      std::string insetSpaces(unsigned long inset)
      {
        std::ostringstream os;
        for(unsigned long i=0; i<inset; ++i)
        {
          if(i == (inset-1))
          { os << "|-- ";}
          else { os << "    "; }
        }
        return os.str();
      }

      std::string getViewHierarchyAsString(unsigned long inset = 0)
      {

        std::ostringstream os;

        os  << insetSpaces(inset)
            << name
            //<< " " << typeid(*this).name()
            << " ("
            << (controllable ? "C" : "")
            << (visible ? "V" : "")
            << (focused ? "F" : "")
            <<  ") \n";
        ChildList::iterator i;
        for(i=unsortedChildren.begin(); i!=unsortedChildren.end(); ++i)
        {
          os << (*i)->getViewHierarchyAsString(inset+1);
        }

        return os.str();
      }

    }; // struct View
    }
  } // namespace guiro
} // namespace lost

#endif


