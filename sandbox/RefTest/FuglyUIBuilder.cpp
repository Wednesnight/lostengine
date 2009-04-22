#include "FuglyUIBuilder.h"

#include "lost/guiro/control/Label.h"
#include "lost/guiro/control/ShapeView.h"
#include "lost/guiro/factory/FuglyFactory.h"

using namespace boost;
using namespace lost::guiro::factory;
using namespace lost::common;
using namespace lost::guiro;
using namespace lost::guiro::control;
using namespace lost::guiro::common;

FuglyUIBuilder::FuglyUIBuilder(lost::shared_ptr<lost::common::Config> inConfig)
: UserInterfaceBuilder(inConfig)
{
}


shared_ptr<ControlFactory> FuglyUIBuilder::createControlFactory(lost::shared_ptr<Config> inConfig)
{
  shared_ptr<FuglyFactory> fuglyFactory(new lost::guiro::factory::FuglyFactory(inConfig));
  return fuglyFactory;
}

void FuglyUIBuilder::addControls(lost::shared_ptr<UserInterface> ui, lost::shared_ptr<ControlFactory> factory)
{
  lost::shared_ptr<Label> flabel = factory->createLabel("flabel");
  flabel->setArea(0, 0, 200, 50);
  flabel->caption("testing");
  lost::shared_ptr<Label> flabel2 = factory->createLabel("flabel2");
  flabel2->font().id="9x15";
  flabel2->setArea(0, 20, 200, 50);
  flabel2->caption("testing");

  lost::shared_ptr<ShapeView> shapeView(new ShapeView("shapeView"));
  shapeView->addLine(xrel(0), yrel(0), xrel(1), yrel(0));
  shapeView->addLine(xrel(0), yrel(0), xrel(1), yrel(1));
  shapeView->addLine(xrel(0), yrel(1), xrel(1), yrel(1));
  shapeView->setArea(50, 50, 200, 200);
  ui->add(flabel);
  ui->add(flabel2);
  ui->add(shapeView);
  
  lost::shared_ptr<Window> window = factory->createWindow("testWindow", "framedWindow");
  window->setArea(100, 100, 300, 300);
  ui->add(window);
  
  ui->setArea(make_area(xabs(0), yaligntop(), wrel(1), hrel(0.5)));
}
