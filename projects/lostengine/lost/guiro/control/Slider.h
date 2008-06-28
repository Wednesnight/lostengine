#ifndef LOST_GUIRO_CONTROL_SLIDER_H
#define LOST_GUIRO_CONTROL_SLIDER_H

#include <string>
#include <sstream>
#include <math.h>

#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>
#include <boost/any.hpp>

#include "lost/guiro/control/View.h"
#include "lost/guiro/control/Button.h"
#include "lost/math/Vec2.h"
#include "lost/guiro/common/Themable.h"
#include "lost/application/MouseSym.h"

#define SLIDER_sliderHandle "Handle"

namespace lost
{
  namespace guiro
  {
    namespace control
    {

      struct Slider : public View, public common::Themable
      {
        enum SliderLayout
        {
          layoutHorizontal = 0,
          layoutVertical   = 1
        };
        enum SliderUpdateMode
        {
          updateAlways  = 0,
          updateRelease = 1
        };

      private:
        float            mMin;
        float            mMax;
        unsigned int     mSteps;
        bool             mHandleActive;
        lost::math::Vec2 mHandleDragOffset;
        SliderLayout     mLayout;
        SliderUpdateMode mUpdateMode;
        bool             mChanged;
        float            oldValue;

      public:
        boost::signal<void (boost::any&)> valueChange;

        Slider( const std::string& inName )
        : View(inName),
          mMin(0),
          mMax(100),
          mSteps(0),
          mHandleActive(false),
          mHandleDragOffset(0,0),
          mLayout(layoutHorizontal),
          mUpdateMode(updateAlways),
          mChanged(false),
          oldValue(0.0f)
        {
          controllable = true;
        }

        Slider( Slider& inSlider )
        : View(inSlider),
          mMin(inSlider.min()),
          mMax(inSlider.max()),
          mSteps(inSlider.steps()),
          mHandleActive(false),
          mHandleDragOffset(0,0),
          mLayout(inSlider.layout()),
          mUpdateMode(inSlider.updateMode()),
          mChanged(false),
          oldValue(0.0f)
        {
          mTheme       = inSlider.theme();
          controllable = true;
        }

        virtual ~Slider() {}

        virtual Slider* clone()
        {
          return new Slider(*this);
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

        virtual void resolve( object::ObjectReferenceResolver& inResolver )
        {
          mTheme.resolve( inResolver );
          View::resolve( inResolver );

          add( mTheme->view );
          value( mMin );
        }

        void doSignalValueChange()
        {
          mChanged = false;

          if (valueChange.num_slots() > 0)
          {
            boost::any sender = this;
            valueChange(sender);
          }
        }

        void layout( const SliderLayout inLayout )
        {
          mLayout = inLayout;
        }
        SliderLayout layout()
        {
          return mLayout;
        }

        void min( const float inMin )
        {
          mMin = inMin;
        }
        float min()
        {
          return mMin;
        }

        void max( const float inMax )
        {
          mMax = inMax;
        }
        float max()
        {
          return mMax;
        }

        void minMax( const float inMin, const float inMax )
        {
          min(inMin);
          max(inMax);
        }

        void steps( const unsigned int inSteps )
        {
          mSteps = inSteps;
        }
        unsigned int steps()
        {
          return mSteps;
        }

        void updateMode( const SliderUpdateMode inMode )
        {
          mUpdateMode = inMode;
        }
        SliderUpdateMode updateMode()
        {
          return mUpdateMode;
        }

        void value( const float inValue )
        {
          float currentValue = std::min( mMax, std::max( mMin, inValue ) );
          if (currentValue != value())
          {
            currentValue -= mMin;
            if (boost::shared_ptr<View> handle = mTheme->viewByProperty(SLIDER_sliderHandle))
            {
              lost::math::Rect handleRect  = handle->getRect();
              float            handleArea  = 0.0f;
              float            handleRange = mMax - mMin;
              switch (mLayout)
              {
                case layoutHorizontal :
                  handleArea = rect.width - handleRect.width;
                  handle->setX( std::min( handleArea, std::max( 0.0f, (currentValue / (handleRange / handleArea)) ) ) );
                  break;
                case layoutVertical :
                  handleArea = rect.height - handleRect.height;
                  handle->setY( std::min( handleArea, std::max( 0.0f, (currentValue / (handleRange / handleArea)) ) ) );
                  break;
              }
            }
            doSignalValueChange();
          }
        }
        float value()
        {
          float result = 0.0f;
          if (boost::shared_ptr<View> handle = mTheme->viewByProperty(SLIDER_sliderHandle))
          {
            lost::math::Rect handleRect  = handle->getRect();
            float            handleArea  = 0.0f;
            float            handleRange = mMax - mMin;
            switch (mLayout)
            {
              case layoutHorizontal :
                handleArea = rect.width - handleRect.width;
                result     = (handleRange / handleArea) * (handleRect.x - rect.x);
                break;
              case layoutVertical :
                handleArea = rect.height - handleRect.height;
                result     = (handleRange / handleArea) * (handleRect.y - rect.y);
                break;
            }
          }
          return result + mMin;
        }

        virtual void mouseButtonEvent( const lost::application::MouseEvent& inEvent )
        {
          if (inEvent.button == lost::application::M_LEFT)
          {
            if (mHandleActive)
            {
              mHandleActive = inEvent.pressed;
            }
            else if (boost::shared_ptr<View> handle = mTheme->viewByProperty(SLIDER_sliderHandle))
            {
              lost::math::Rect handleRect( handle->getRect() );
              if (mHandleActive = inEvent.pressed)
              {
                oldValue = value();
                if (handle->hitTest( inEvent.pos ))
                {
                  mHandleDragOffset = inEvent.pos - lost::math::Vec2(handleRect.x, handleRect.y);
                }
                else
                {
                  mHandleDragOffset = lost::math::Vec2(handleRect.width / 2, handleRect.height / 2);
                  mouseMoveEvent( inEvent );
                }
              }
              else
              {
                mHandleDragOffset = lost::math::Vec2(0,0);
              }
            }
            if (!mHandleActive && mChanged) doSignalValueChange();
          }
        }

        virtual void mouseMoveEvent( const lost::application::MouseEvent& inEvent )
        {
          if (mHandleActive)
          {
            if (boost::shared_ptr<View> handle = mTheme->viewByProperty(SLIDER_sliderHandle))
            {
              lost::math::Rect handleRect( handle->getRect() );
              lost::math::Rect parentRect( getRect() );

              float handleArea   = 0.0f;
              float handlePos    = 0.0f;
              float handlePosOld = 0.0f;
              switch (mLayout)
              {
                case layoutHorizontal :
                  handleArea   = parentRect.width - handleRect.width;
                  handlePos    = std::min( handleArea, std::max( 0.0f, (inEvent.pos.x - mHandleDragOffset.x - parentRect.x) ) );
                  handlePosOld = handleRect.x - parentRect.x;
                  break;
                case layoutVertical :
                  handleArea   = parentRect.height - handleRect.height;
                  handlePos    = std::min( handleArea, std::max( 0.0f, (inEvent.pos.y - mHandleDragOffset.y - parentRect.y) ) );
                  handlePosOld = handleRect.y - parentRect.y;
                  break;
              }

              if (mSteps)
              {
                float gap    = (handleArea / (mMax - mMin)) * ((mMax - mMin) / mSteps);
                int   offset = (int)(handlePos / gap);

                switch (mLayout)
                {
                  case layoutHorizontal :
                    handlePos = std::min( handleArea, std::max( 0.0f, (offset*gap) ) );
                    break;
                  case layoutVertical :
                    handlePos = std::min( handleArea, std::max( 0.0f, (offset*gap) ) );
                    break;
                }
              }

              if (handlePos != handlePosOld)
              {
                switch (mLayout)
                {
                  case layoutHorizontal :
                    handle->setX( handlePos );
                    break;
                  case layoutVertical :
                    handle->setY( handlePos );
                    break;
                }
                mChanged = (value() != oldValue);
                if (mChanged && mUpdateMode == updateAlways) doSignalValueChange();
              }
            }
          }
        }

      };
    }
  }
}

#endif
