#ifndef LOST_GUIRO_COMMON_THEME_H
#define LOST_GUIRO_COMMON_THEME_H

#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>
#include "lost/guiro/control/View.h"
#include "lost/guiro/common/Clonable.h"

namespace lost
{
  namespace guiro
  {
    namespace common
    {

      struct Theme : public object::Object, public object::ObjectReferenceContainer, public Clonable
      {
        boost::shared_ptr<lost::guiro::control::View> view;
        std::map<std::string, std::string>            properties;

        Theme() {}
        virtual ~Theme() {}

        Theme( Theme& theme )
        : view(theme.view->clone()),
          properties(theme.properties)
        {
          view->childrenCopyFrom( theme.view );
        }

        virtual Theme* clone()
        {
          return new Theme(*this);
        }

        bool operator ()( const std::string& propertyId )
        {
          return (properties.find( propertyId ) != properties.end());
        }

        std::string operator []( const std::string& propertyId )
        {
          if (!(*this)(propertyId))
            throw std::runtime_error( "[lost::guiro::Theme[]] property not found: "+ propertyId );
          return properties[propertyId];
        }

        boost::shared_ptr<control::View> viewByProperty( const std::string& propertyName )
        {
          boost::shared_ptr<control::View> result;
          if ((*this)(propertyName))
            result = (*(view.get()))[(*this)[propertyName]];
          return result;
        }

        template <typename ResultType>
        boost::shared_ptr<ResultType> viewByProperty( const std::string& propertyName )
        {
          boost::shared_ptr<ResultType> result;
          if ((*this)(propertyName))
            result = boost::dynamic_pointer_cast<ResultType>((*(view.get()))[(*this)[propertyName]]);
          return result;
        }

        virtual void resolve( object::ObjectReferenceResolver& inResolver )
        {
          if (view)
          {
            view->resolve( inResolver );
          }
        }

      };

    }
  } // namespace guiro
} // namespace lost

#endif
