#ifndef FUGLYUIBUILDER_H
#define FUGLYUIBUILDER_H

#include "lost/guiro/factory/UserInterfaceBuilder.h"
#include "lost/common/Config.h"

struct FuglyUIBuilder : public lost::guiro::factory::UserInterfaceBuilder
{
  FuglyUIBuilder(lost::shared_ptr<lost::common::Config> inConfig);

  lost::shared_ptr<lost::guiro::factory::ControlFactory>
  createControlFactory  (lost::shared_ptr<lost::common::Config> inConfig);

  void
  addControls           (lost::shared_ptr<lost::guiro::control::UserInterface> ui,
                         lost::shared_ptr<lost::guiro::factory::ControlFactory> factory);
};

#endif
