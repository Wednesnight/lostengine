#ifndef LOST_GUIRO_CONTROL_SCROLLBAR_H
#define LOST_GUIRO_CONTROL_SCROLLBAR_H

#include <boost/signal.hpp>
#include <boost/any.hpp>

#include "lost/guiro/control/Button.h"
#include "lost/guiro/control/View.h"
#include "lost/guiro/control/Slider.h"
#include "lost/guiro/common/Themable.h"

#define SCROLLBAR_scrollbarButtonUp   "ButtonUp"
#define SCROLLBAR_scrollbarButtonDown "ButtonDown"
#define SCROLLBAR_scrollbarSlider     "Slider"

namespace lost
{
  namespace guiro
  {
    namespace control
    {

      struct Scrollbar : public View, public common::Themable
      {
        enum ScrollbarLayout
        {
          layoutHorizontal = 0,
          layoutVertical   = 1
        };

      private:
        ScrollbarLayout mLayout;
        float           mStepSize;

      public:
        boost::signal<void (boost::any&)> positionChanged;

        Scrollbar( const std::string& inName )
        : View(inName),
          mLayout(layoutHorizontal),
          mStepSize(0.1)
        {
          controllable = true;
        }

        Scrollbar( Scrollbar& copy )
        : View(copy),
          mLayout(copy.layout()),
          mStepSize(copy.stepSize())
        {
          mTheme       = copy.theme();
          controllable = true;
        }

        virtual ~Scrollbar() {}

        virtual Scrollbar* clone()
        {
          return new Scrollbar(*this);
        }

        virtual void draw( lost::math::Rect& parentRect, lost::common::Color& parentColor )
        {
          if (getVisible())
          {
            lost::math::Rect clippedRect = rect;
            clippedRect += parentRect.pos();
            clippedRect.clipTo( parentRect );

            lost::common::Color currentColor = color * parentColor;

            View::draw( parentRect, parentColor );
          }
        }

        float position()
        {
          float result = 0.0f;
          if (boost::shared_ptr<Slider> slider = mTheme->viewByProperty<Slider>(SCROLLBAR_scrollbarSlider))
            result = slider->value();
          return result;
        }
        void position( float value )
        {
          if (boost::shared_ptr<Slider> slider = mTheme->viewByProperty<Slider>(SCROLLBAR_scrollbarSlider))
            slider->value(value);
        }

        virtual void resolve( object::ObjectReferenceResolver& inResolver )
        {
          mTheme.resolve( inResolver );
          View::resolve( inResolver );

          add( mTheme->view );

          if (boost::shared_ptr<Slider> slider = mTheme->viewByProperty<Slider>(SCROLLBAR_scrollbarSlider))
          {
            slider->minMax( 0, 1 );
            slider->valueChange.connect( boost::bind(&Scrollbar::sliderValueChange, this, _1) );
          }

          if (boost::shared_ptr<Button> buttonUp = mTheme->viewByProperty<Button>(SCROLLBAR_scrollbarButtonUp))
            buttonUp->buttonPress.connect( boost::bind(&Scrollbar::buttonUpPress, this, _1) );
          if (boost::shared_ptr<Button> buttonDown = mTheme->viewByProperty<Button>(SCROLLBAR_scrollbarButtonDown))
            buttonDown->buttonPress.connect( boost::bind(&Scrollbar::buttonDownPress, this, _1) );
        }

        ScrollbarLayout layout()
        {
          return mLayout;
        }
        void layout( ScrollbarLayout inLayout )
        {
          mLayout = inLayout;
        }

        float stepSize()
        {
          return mStepSize;
        }
        void stepSize( float inStepSize )
        {
          mStepSize = std::min( 1.0f, std::max( 0.0f, inStepSize ) );
        }

        void buttonUpPress( boost::any& sender )
        {
          if (boost::shared_ptr<Slider> slider = mTheme->viewByProperty<Slider>(SCROLLBAR_scrollbarSlider))
          {
            slider->value( ((Slider*)slider.get())->value() + mStepSize );
          }
        }

        void buttonDownPress( boost::any& sender )
        {
          if (boost::shared_ptr<Slider> slider = mTheme->viewByProperty<Slider>(SCROLLBAR_scrollbarSlider))
          {
            slider->value( ((Slider*)slider.get())->value() - mStepSize );
          }
        }

        void sliderValueChange( boost::any& sender )
        {
          if (positionChanged.num_slots() > 0)
          {
            boost::any newSender(this);
            positionChanged(newSender);
          }
        }

      };

    }
  }
}

#endif
