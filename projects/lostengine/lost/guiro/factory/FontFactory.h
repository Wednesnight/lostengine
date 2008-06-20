#ifndef LOST_GUIRO_FACTORY_FONTFACTORY_H
#define LOST_GUIRO_FACTORY_FONTFACTORY_H

#include "lost/guiro/factory/Factory.h"
#include <map>

namespace lost
{
  namespace guiro
  {
    namespace factory
    {
      
    struct FontFactory : public lost::guiro::factory::Factory
    {
      typedef std::map<std::string, boost::shared_ptr<lost::font::Font> > FontMap;
      
      FontFactory(boost::shared_ptr<lost::common::Config> inConfig)
      : Factory(inConfig)
      {
      }
      
      FontFactory( boost::shared_ptr<resource::Loader> inLoader, boost::shared_ptr<lost::common::Config> inConfig )
        : Factory( inLoader, inConfig )
      {
      }

      void add(boost::shared_ptr<lost::font::Font> font)
      {
        fonts[font->id] = font;
      }
      
      boost::shared_ptr<object::Object> operator[]( const std::string& id )
      {
        // cached or previously added fonts are preferred
        FontMap::iterator pos = fonts.find(id);
        if(pos != fonts.end())
        {
          return pos->second;
        }
        else
        {
          if(!resourceLoader) { throw std::runtime_error("attempted load without resourceloader"); }
          // FIXME: at a later stage, retrieve actual location of file from with Font
          //        for now, location equals id
          std::string path = id;
          boost::shared_ptr<lost::font::TrueTypeFont> result = resourceLoader->loadFont(path);
          fonts[path] = result;
          result->id = path;
          return result;
        }
      }
    private:
      std::map<std::string, boost::shared_ptr<lost::font::Font> > fonts;
    };
    }
  }
}

#endif
