#ifndef VANILLAUIBUILDER_H
#define VANILLAUIBUILDER_H

#include "lost/guiro/factory/UserInterfaceBuilder.h"
#include "lost/common/Config.h"

struct VanillaUIBuilder : public lost::guiro::factory::UserInterfaceBuilder
{
  VanillaUIBuilder(boost::shared_ptr<lost::common::Config> inConfig);

  boost::shared_ptr<lost::guiro::factory::ControlFactory>
  createControlFactory  (boost::shared_ptr<lost::common::Config> inConfig);

  void
  addControls           (boost::shared_ptr<lost::guiro::control::UserInterface> ui,
                         boost::shared_ptr<lost::guiro::factory::ControlFactory> factory);
};


#endif
