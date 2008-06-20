#ifndef LOST_GUIRO_FACTORY_BITMAPFACTORY_H
#define LOST_GUIRO_FACTORY_BITMAPFACTORY_H

#include <sstream>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>

#include "lost/bitmap/Bitmap.h"
#include "lost/bitmap/BitmapLoader.h"
#include "lost/guiro/factory/Factory.h"
#include "lost/resource/Set.h"

namespace lost
{
  namespace guiro
  {
    namespace factory
    {

    struct BitmapFactory : public lost::guiro::factory::Factory
    {
      typedef std::map<std::string, boost::shared_ptr<bitmap::Bitmap> > BitmapMap;
      
      BitmapFactory(boost::shared_ptr<lost::common::Config> inConfig)
      : Factory(inConfig)
      {
      }
      
      BitmapFactory( boost::shared_ptr<resource::Loader> inLoader, boost::shared_ptr<lost::common::Config> inConfig )
        : Factory( inLoader, inConfig )
      {
        const std::string                  value(config->get<std::string>("lost.guiro.BitmapFactory.sets",""));
        std::vector<std::string>           fileList;
        std::vector<std::string>::iterator file;
        split( fileList, value, boost::is_any_of(",;") );
        for (file = fileList.begin(); file != fileList.end(); file++)
        {
          if ((*file).size() > 0)
            this->loadBitmapSet( *file );
        }
      }

      boost::shared_ptr<object::Object> operator[]( const std::string& id )
      {
        BitmapMap::iterator pos;
        pos = bitmaps.find(id); 
        if (pos != bitmaps.end())
        {
          boost::shared_ptr<bitmap::Bitmap> result = pos->second;
          if (!result->data)
          {
            boost::shared_ptr<lost::resource::File> file = resourceLoader->load( result->location );
            boost::shared_ptr<lost::bitmap::Bitmap> bitmap = bitmapLoader.loadFromMemory( file );
                        
            result->data = bitmap->data;
            result->width = bitmap->width;
            result->height = bitmap->height;
            result->format = bitmap->format;
            result->type = bitmap->type;
            
            bitmap->data = NULL; // this is a hack to prevent deletion from the destructor of the temporary object
          }
          return result;
        }
        else
        {
          throw std::runtime_error( "couldn't find bitmap '"+ id +"'" );
        }
      }

      void loadBitmapSet( const std::string& filename )
      {
        lost::resource::Set<bitmap::Bitmap> bitmapSet = loadSet<lost::bitmap::Bitmap>( filename );
        std::vector<boost::shared_ptr<bitmap::Bitmap> >::iterator bitmap;
        for (bitmap = bitmapSet.objects.begin(); bitmap != bitmapSet.objects.end(); bitmap++)
          bitmaps[(*bitmap)->id] = (*bitmap);
      }

      lost::bitmap::BitmapLoader  bitmapLoader;
      BitmapMap                   bitmaps;
    };
    }
  }
}

#endif


