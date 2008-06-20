#ifndef LOST_SIGNAL_SIGNALNODEMANAGER
#define LOST_SIGNAL_SIGNALNODEMANAGER

#include "lost/signal/SignalNode.h"
#include "lost/signal/ConnectionMapping.h"
#include <stdexcept>
#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace lost
{
  namespace signal
  {
  /** All SignalNodes whose connections should be automatically rewired from serialized 
   * ConnectionMapping should register with the SignalNodeManager.
   * Once all Nodes and Mappings are known to the SignalNodeManager, you can call rewire 
   * and the connections will be setup.
   * This allows for a purely data driven management of connections between signals and slots.
   * Furthermore, the exact types of the actual classes are irrelevant, since the names of the 
   * nodes, signals and slots are the only information required by the manager to reestablish
   * the connections. It's therefore pretty easy to run different configuration (release, debug), 
   * perform redirections of signals via a proxy class and switch mappings at runtime.
   */
  struct SignalNodeManager
  {
    
    void addNode(boost::shared_ptr<SignalNode> node)
    {
      if(nodes.find(node->nodeName()) != nodes.end())
      {
        throw std::runtime_error("SignalNodeManager: Node '"+node->nodeName() + "' was already registered. Names must be unique!");
      }
      nodes[node->nodeName()] = node;
    }
    
    void addMapping(boost::shared_ptr<ConnectionMapping> mapping) { mappings.push_back(mapping);}
    
    void rewire()
    {
      for(unsigned long i = 0; i<mappings.size(); ++i)
      {
        boost::shared_ptr<ConnectionMapping> mapping = mappings[i];
        boost::shared_ptr<SignalNode> leftnode = findNode(mapping->sourceName());
        boost::shared_ptr<SignalNode> rightnode = findNode(mapping->destinationName());
        mapping->rewire(leftnode, rightnode);
      }
    }
    
  private:  
    boost::shared_ptr<SignalNode> findNode(const std::string& name)
    {
      std::map<std::string, boost::shared_ptr<SignalNode> >::iterator pos;
      
      if((pos = nodes.find(name)) == nodes.end())
      {
        throw std::runtime_error("SignalNodeManager : can't find node '"+name+"'");
      }
      
      return pos->second;
    }
    
    std::map<std::string, boost::shared_ptr<SignalNode> > nodes; // node names are unique, therefore we can use a map for node storage
    std::vector<boost::shared_ptr<ConnectionMapping> > mappings; // arbitrary mappings are possible, and since they carry the node names we can simply put them in a list
  };
  }
}
    
#endif