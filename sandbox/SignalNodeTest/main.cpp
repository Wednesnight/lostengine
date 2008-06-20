#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <exception>

#include <boost/any.hpp>
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "lost/signal/SignalNode.h"
#include "lost/signal/ConnectionMapping.h"
#include "lost/signal/SignalNodeManager.h"

struct View : public lost::signal::SignalNode
{
  View()
  {
    nodeName("view");
    registerSignal("clicked", clicked);
    registerSignal("valueChanged", valueChanged);
  }
  
  boost::signal<void(boost::any)> clicked;
  boost::signal<void(boost::any)> valueChanged;
};

struct Controller : public lost::signal::SignalNode
{
  Controller()
  {    
    nodeName("controller");
    registerSlot("selectStuff", boost::bind(&Controller::selectStuff, *this, _1));
    registerSlot("newValue", boost::bind(&Controller::newValue, *this, _1));
  }
  
  void selectStuff(const boost::any& val) { std::cout << "selectStuff" << std::endl;};
  void newValue(const boost::any& val) { std::cout << "newValue: " << boost::any_cast<float>(val) << std::endl;};
};

int main()
{
  try
  {    
    boost::shared_ptr<View> v(new View());
    boost::shared_ptr<Controller> c(new Controller());
    
    boost::shared_ptr<lost::signal::ConnectionMapping> m(new lost::signal::ConnectionMapping());
    m->sourceName("view");
    m->destinationName("controller");
    m->addConnection("clicked", "selectStuff");
    m->addConnection("valueChanged", "newValue");

    lost::signal::SignalNodeManager manager;
    manager.addNode(v);
    manager.addNode(c);
    manager.addMapping(m);
    manager.rewire();
    
    v->clicked(boost::any());
    v->valueChanged(boost::any(3.0f));
    
    return 0;
  }
  catch(std::exception& e)
  {
    std::cout << "error: " << e.what() << std::endl;
  }
}