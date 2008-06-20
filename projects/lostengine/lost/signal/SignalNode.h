#ifndef LOST_SIGNAL_SIGNALNODE
#define LOST_SIGNAL_SIGNALNODE

#include <map>
#include <exception>
#include <string>
#include <boost/function.hpp>
#include <boost/any.hpp>
#include <boost/signal.hpp>

namespace lost
{
  namespace signal
  {

  /** This class is the base for all classes that want to expose signals and slots for automatic rewiring.
   * We need this due to the meager introspection features of C++. Since we couldn't come up with an elegant and convenient
   * way to serialize connections with varying signatures, all signals and slots have the same signature, which is exactly 
   * one 'any' parameter. This enables us to pass any amount and type of parameters, but also makes the interface
   * slightly nondescriptive. In practice, this should hopefully not matter much, since serialization and automatic mapping
   * of connections should only be relevant where user interaction can happen, which should be fairly simple to parametrize.
   * The performance overhead introduced by usage of 'any' types should stay minimal (but is not yet tested) since in 
   * case of an empty any sent, no memory allocations should happen. 
   */
  struct SignalNode
  {
    const std::string& nodeName() { return nodename; }
    void nodeName(const std::string& name) { nodename = name; }
    
    void registerSignal(const std::string& inName, boost::signal<void(boost::any)>& inSignal)
    {
      if(signals.find(inName) != signals.end()) { throw std::runtime_error("you already registered a signal with name '"+inName+"'"); }
      signals[inName] = &inSignal;
    }
    
    void registerSlot(const std::string& inName, const boost::function<void(boost::any)>& inFunc)
    {
      if(slots.find(inName) != slots.end()) { throw std::runtime_error("you already registered a slot with name '"+inName+"'"); }
      slots[inName] = inFunc;
    }
    
    boost::signal<void(boost::any)>* getSignal(const std::string& name) 
    {
      return signals[name];
    }
    
    boost::function<void(boost::any)> getSlot(const std::string& name) 
    {
      return slots[name];
    }

  private:
    std::string nodename;
    std::map<std::string, boost::signal<void(boost::any)>*> signals; //signal pointer
    std::map<std::string, boost::function<void(boost::any)> > slots; // boost::function copies
  };
  }
}

#endif