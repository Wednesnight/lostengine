#ifndef LOST_GUIRO_CONTROL_SCREEN_H
#define LOST_GUIRO_CONTROL_SCREEN_H

#include "lost/guiro/control/UserInterface.h"
#include "lost/common/DisplayAttributes.h"

namespace lost
{
namespace guiro
{
namespace control
{

/** top level guiro View that is the actual container for UserInterfaces.
 */
struct Screen : public View
{
  const lost::common::DisplayAttributes& displayAttributes;
  
  Screen(const lost::common::DisplayAttributes& inAttributes)
  : View("screen"), // name is always fixed since it's teh only root element for all UIs
    displayAttributes(inAttributes)
  {}

  virtual ~Screen() {}
  
  void updateGeometry()
  {
    rect.reset( 0, 0, displayAttributes.width, displayAttributes.height );
    updateChildGeometry();
  }
  
  void draw()
  {
    PROFILE_METHOD();
    View::draw(rect, color );
  }

  // only accept UIs here, but add them as regular views.
  virtual void add(boost::shared_ptr<UserInterface> inUI)
  {
    View::add(inUI);
  }
  
  void injectKeyboardEvent( const lost::application::KeyEvent& inEvent )
  {
    // FIXME: maybe implement this with visitor pattern so we need to write the iteration onyl once?
    for(ChildList::iterator i=unsortedChildren.begin(); i!=unsortedChildren.end(); ++i)
    {
      // since we made sure in the "add" method that all inserted Views were UserInterfaces, 
      // we can safely reuse the unsortedChildren View list, cast each element to UI* and
      // call the appropriate method
      View* view = i->get();
      UserInterface* ui = static_cast<UserInterface*>(view);
      ui->injectKeyboardEvent(inEvent);
    }
  }
  
  void injectMouseButtonEvent( const lost::application::MouseEvent& inEvent )
  {
    // FIXME: maybe implement this with visitor pattern so we need to write the iteration onyl once?
    for(ChildList::iterator i=unsortedChildren.begin(); i!=unsortedChildren.end(); ++i)
    {
      // since we made sure in the "add" method that all inserted Views were UserInterfaces, 
      // we can safely reuse the unsortedChildren View list, cast each element to UI* and
      // call the appropriate method
      View* view = i->get();
      UserInterface* ui = static_cast<UserInterface*>(view);
      ui->injectMouseButtonEvent(inEvent);
    }
  }
  
  void injectMouseMoveEvent( const lost::application::MouseEvent& inEvent )
  {
    // FIXME: maybe implement this with visitor pattern so we need to write the iteration onyl once?
    for(ChildList::iterator i=unsortedChildren.begin(); i!=unsortedChildren.end(); ++i)
    {
      // since we made sure in the "add" method that all inserted Views were UserInterfaces, 
      // we can safely reuse the unsortedChildren View list, cast each element to UI* and
      // call the appropriate method
      View* view = i->get();
      UserInterface* ui = static_cast<UserInterface*>(view);
      ui->injectMouseMoveEvent(inEvent);
    }
  }  
private:
  // made this private so that only UIs are allowed through add
  virtual void add( const boost::shared_ptr<View>& inView )
  {
    View::add(inView);
  }
};
}
}
}

#endif

