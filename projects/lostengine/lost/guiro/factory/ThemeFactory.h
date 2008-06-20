#ifndef LOST_GUIRO_FACTORY_THEMEFACTORY_H
#define LOST_GUIRO_FACTORY_THEMEFACTORY_H

#include "lost/guiro/factory/Factory.h"
#include "lost/guiro/io/ThemeIo.h"
#include <boost/serialization/shared_ptr.hpp>

namespace lost
{
  namespace guiro
  {
    namespace factory
    {

    struct ThemeFactory : public lost::guiro::factory::Factory
    {
      ThemeFactory(boost::shared_ptr<lost::common::Config> inConfig)
      : Factory(inConfig)
      {
      }
      
      ThemeFactory( boost::shared_ptr<resource::Loader> inLoader, boost::shared_ptr<lost::common::Config> inConfig )
        : Factory( inLoader, inConfig )
      {
        const std::string                  value(config->get<std::string>("lost.guiro.ThemeFactory.sets",""));
        std::vector<std::string>           fileList;
        std::vector<std::string>::iterator file;
        split( fileList, value, boost::is_any_of(",;") );
        for (file = fileList.begin(); file != fileList.end(); file++)
        {
          if ((*file).size() > 0)
            this->loadThemeSet( *file );
        }
      }

      void add(boost::shared_ptr<common::Theme> theme)
      {
        themes[theme->id] = theme;
      }
      
      boost::shared_ptr<object::Object> operator[]( const std::string& id )
      {
        if (themes.find(id) != themes.end())
        {
          boost::shared_ptr<common::Theme> result( themes[id]->clone() );
          return result;
        }
        else
        {
          throw std::runtime_error( "couldn't find theme '"+ id +"'" );
        }
      }

      void loadThemeSet( const std::string& filename )
      {
        lost::resource::Set<common::Theme> themeSet = loadSet<common::Theme>( filename );
        std::vector<boost::shared_ptr<common::Theme> >::iterator theme;
        for (theme = themeSet.objects.begin(); theme != themeSet.objects.end(); theme++)
          themes[(*theme)->id] = (*theme);
      }

      std::map<std::string, boost::shared_ptr<common::Theme> > themes;
    };
    }
  } // namespace guiro
} // namespace lost

#endif


