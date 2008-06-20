#ifndef LOST_GUIRO_FACTORY_TEXTUREFACTORY_H
#define LOST_GUIRO_FACTORY_TEXTUREFACTORY_H

#include <boost/shared_ptr.hpp>

#include "lost/guiro/factory/Factory.h"
#include "lost/resource/File.h"
#include "lost/resource/Loader.h"
#include "lost/resource/Set.h"
#include "lost/common/Config.h"
#include "lost/guiro/io/TextureIo.h"

namespace lost
{
namespace guiro
{
namespace factory
{

    struct TextureFactory : public lost::guiro::factory::Factory
    {
      TextureFactory(boost::shared_ptr<lost::common::Config> inConfig )
      : Factory( inConfig )
      {
      }
      
      TextureFactory( boost::shared_ptr<resource::Loader> inLoader, boost::shared_ptr<lost::common::Config> inConfig )
      : Factory( inLoader, inConfig )
      {
        const std::string                  value(config->get<std::string>("lost.guiro.TextureFactory.sets",""));
        std::vector<std::string>           fileList;
        std::vector<std::string>::iterator file;
        split( fileList, value, boost::is_any_of(",;") );
        for (file = fileList.begin(); file != fileList.end(); file++)
        {
          if ((*file).size() > 0)
            this->loadTextureSet( *file );
        }
      }

      void add(boost::shared_ptr<lost::guiro::common::Texture> texture)
      {
        textures[texture->id] = texture;
      }
      
      /** retrieves a theme with the given id.
       */
      boost::shared_ptr<object::Object> operator[]( const std::string& id )
      {
        boost::shared_ptr<lost::guiro::common::Texture> result;

        std::map<std::string, boost::shared_ptr<lost::guiro::common::Texture> >::iterator pos = textures.find(id);
        if(pos != textures.end())
        {
          result = pos->second;
        }
        else
        {
          throw std::runtime_error("couldn't find texture: "+id);
        }

        return result;
      }

      /** precache themes from a file.
       * Theme definitions can be added at runtime or loaded from a file. This functions loads a 
       * set of theme defs via the generic Factory::load method and inserts them into the cache.
       *
       * @param filename full pathname of the set definition file.
       */
      void loadTextureSet( const std::string& filename )
      {
        lost::resource::Set<common::Texture> textureSet = loadSet<common::Texture>( filename );
        std::vector<boost::shared_ptr<common::Texture> >::iterator texture;
        for (texture = textureSet.objects.begin(); texture != textureSet.objects.end(); texture++)
          textures[(*texture)->id] = (*texture);
      }

      std::map<std::string, boost::shared_ptr<lost::guiro::common::Texture> > textures;
    };
  }
}
}

#endif


