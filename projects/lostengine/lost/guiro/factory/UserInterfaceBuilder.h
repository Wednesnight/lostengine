#ifndef LOST_GUIRO_FACTORY_USERINTERFACEBUILDER_H
#define LOST_GUIRO_FACTORY_USERINTERFACEBUILDER_H

#include <boost/shared_ptr.hpp>
#include "lost/guiro/factory/ControlFactory.h"
#include "lost/guiro/control/UserInterface.h"
#include <string>
#include "lost/common/Logger.h"

namespace lost
{
  namespace guiro
  {
    namespace factory
    {
      /** This is the common interface for classes encapsulating construction of a UserInterface
       * with an exchangable ControlFactory. This interface enforces two important constraints:
       * 1.) the UI must be constructed from common elements defined by the ControlFactory interface (Window, Button, etc.)
       * 2.) the ObjectReferences are resolved after the controls were added, with an ObjectResolved provided by
       *     the control factory.
       * for ObjectResolver/Factories requiring OpenGL resources (e.g. Textures) this also means that build
       * may only be called AFTER THE OPENGL CONTEXT WAS CREATED! This is usually done by calling VideoSubSystem::restart
       * BEFORE attempting to build the actual ui.
       * For the sake of simplicity we assume that no custom UserInterface classes are used. If this should be the case,
       * build() can be opverridden and interpreted as a best-practice implementation example.
       */
      struct UserInterfaceBuilder
      {
        boost::shared_ptr<ControlFactory> controlFactory;
        boost::shared_ptr<lost::common::Config>   config;

        UserInterfaceBuilder(boost::shared_ptr<lost::common::Config> inConfig)
        : config(inConfig)
        {}

        virtual ~UserInterfaceBuilder() {}
        virtual boost::shared_ptr<control::UserInterface> build(const std::string uiname)
        {
          controlFactory = createControlFactory(config);
          boost::shared_ptr<control::UserInterface> ui = control::UserInterface::create(uiname);
          addControls(ui, controlFactory);
          ui->resolve(controlFactory->getResolver());
          return ui;
        }

        // override this and create your ControlFactory here
        virtual boost::shared_ptr<ControlFactory> createControlFactory(boost::shared_ptr<lost::common::Config> inConfig) = 0;

        // override this and add your controls to the provided ui object, creating them via the controlFactory member
        virtual void addControls(boost::shared_ptr<control::UserInterface> ui, boost::shared_ptr<ControlFactory> factory) = 0;
      };
    }
  }
}

#endif
