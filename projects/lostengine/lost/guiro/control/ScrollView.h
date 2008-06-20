#ifndef LOST_GUIRO_CONTROL_SCROLLVIEW_H
#define LOST_GUIRO_CONTROL_SCROLLVIEW_H

#include "lost/guiro/control/View.h"
#include "lost/guiro/control/Scrollbar.h"
#include "lost/guiro/common/Themable.h"
#include "lost/common/Profiler.h"

#define SCROLLVIEW_scrollViewScrollbarVertical   "ScrollbarVertical"
#define SCROLLVIEW_scrollViewScrollbarHorizontal "ScrollbarHorizontal"

namespace lost
{
  namespace guiro
  {
    namespace control
    {

      struct ScrollView : public View, public common::Themable
      {
      private:
        boost::shared_ptr<View> mView;

        ScrollView( const std::string& inName )
        : View(inName),
          mView(new View(std::string(inName).append("_View")))
        {
          controllable = true;
        }

        ScrollView( ScrollView& copy )
        : View(copy),
          mView(copy.view())
        {
          controllable = true;
          mTheme       = copy.theme();
          mView->childrenApplyFrom( copy.view() );
        }

      public:
        virtual ~ScrollView() {}

        virtual boost::shared_ptr<View>& operator[]( const lost::guiro::common::ViewPath& inPath )
        {
          return (*mView)[inPath];
        }

        virtual void add( const boost::shared_ptr<View>& inView )
        {
          mView->add(inView);
        }

        virtual ScrollView* clone()
        {
          return new ScrollView(*this);
        }

        static ScrollView* create( const std::string& inName )
        {
          return new ScrollView(inName);
        }

        virtual void draw(const lost::math::Rect& parentRect, const lost::common::Color& parentColor )
        {
          PROFILE_METHOD();
          if (getVisible())
          {
            lost::math::Rect currentRect = rect;
            if (boost::shared_ptr<Scrollbar> horizontal = mTheme->viewByProperty<Scrollbar>(SCROLLVIEW_scrollViewScrollbarHorizontal))
            {
              lost::math::Rect clientRect = horizontal->getRect();
              currentRect.y      += clientRect.height;
              currentRect.height -= clientRect.height;
            }
            if (boost::shared_ptr<Scrollbar> vertical = mTheme->viewByProperty<Scrollbar>(SCROLLVIEW_scrollViewScrollbarVertical))
            {
              lost::math::Rect clientRect = vertical->getRect();
              currentRect.width -= clientRect.width;
            }

            lost::math::Rect clippedRect = rect;
            clippedRect.clipTo(parentRect);
            lost::common::Color currentColor = color * parentColor;
            lost::gl::Scissor scissor( clippedRect );

            // draw the content view
            mView->draw( currentRect, currentColor );
            // draw the theme view
            mTheme->view->draw( clippedRect, currentColor );
          }
        }

        virtual void resolve( object::ObjectReferenceResolver& inResolver )
        {
          // add the view here to let View handle the child resolve
          View::add( mView );

          mTheme.resolve( inResolver );
          View::resolve( inResolver );

          View::add( mTheme->view );

          if (boost::shared_ptr<Scrollbar> horizontal = mTheme->viewByProperty<Scrollbar>(SCROLLVIEW_scrollViewScrollbarHorizontal))
            horizontal->positionChanged.connect( boost::bind(&ScrollView::horizontalMove, this, _1) );
          if (boost::shared_ptr<Scrollbar> vertical = mTheme->viewByProperty<Scrollbar>(SCROLLVIEW_scrollViewScrollbarVertical))
            vertical->positionChanged.connect( boost::bind(&ScrollView::verticalMove, this, _1) );

          lost::math::Rect currentRect = rect;
          if (boost::shared_ptr<Scrollbar> horizontal = mTheme->viewByProperty<Scrollbar>(SCROLLVIEW_scrollViewScrollbarHorizontal))
          {
            currentRect.height -= horizontal->getRect().height;
          }
          mView->setX(0.0f);
          mView->setY(currentRect.height - mView->getRect().height);
        }

        void horizontalMove( boost::any& sender )
        {
        }
        void verticalMove( boost::any& sender )
        {
        }

        boost::shared_ptr<View>& view()
        {
          return mView;
        }
        void view( const boost::shared_ptr<View>& inView )
        {
          mView = inView;
        }

      };

    }
  }
}

#endif
