#ifndef LOST_GUIRO_FACTORY_FACTORY_H
#define LOST_GUIRO_FACTORY_FACTORY_H

#include <sstream>

#include <boost/shared_ptr.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "lost/object/Object.h"
#include "lost/resource/File.h"
#include "lost/resource/Loader.h"
#include "lost/resource/Set.h"
#include "lost/common/Config.h"

namespace lost
{
  namespace guiro
  {
    namespace factory
    {

    struct Factory
    {
      boost::shared_ptr<resource::Loader> resourceLoader; // can be NULL 
      boost::shared_ptr<common::Config>     config; // shouldn't ever be NULL

      /** Constructor for factories without ResourceLoader dependency.
       *  If your factory doesn't or shoudn't require external resources, use this constructor.
       *  Even if you don't want to use any config properties in your factory you should provide
       *  a valid Config object for future expansion.
       */
      Factory(boost::shared_ptr<common::Config> inConfig )
      : config(inConfig)
      {
      }
      
      /** Constructor for factories that actually load external resources.
       *  You
       */
      Factory( boost::shared_ptr<resource::Loader> inLoader, boost::shared_ptr<common::Config> inConfig )
        : resourceLoader(inLoader),
          config(inConfig)
      {
      }

      virtual ~Factory() {}
      
      /** use this to acquire an object instance with a specific id.
       * Usually, you'll use this while resolving ObjectReferences and will have retrieved the 
       * appropriate factory from the ObjectResolver via the type of the ObjectReference.
       */
      virtual boost::shared_ptr<object::Object> operator[](const std::string& id) = 0;

      /** Load a set of object definitions into the factory.
       * A factory can hold fully or partially resolved objects. This method loads a generic set of object definitions
       * for your convenience. Usually, you'll directly insert these definitions into the cache of your factory.
       */
      template <typename Object>
      lost::resource::Set<Object> loadSet( const std::string& filename )
      {
        if(!resourceLoader ) { throw std::runtime_error("Factory: attempted load without ResourceLoader: "+filename); }
        lost::resource::Set<Object> objectSet;
        boost::shared_ptr<lost::resource::File> file = resourceLoader->load( filename );
        std::stringstream stream(file->data.get());
        boost::archive::xml_iarchive ia( stream );
        ia >> BOOST_SERIALIZATION_NVP(objectSet);
        return objectSet;
      }
    };
    }
  }
}

#endif
