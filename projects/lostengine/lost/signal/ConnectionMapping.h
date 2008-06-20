#ifndef LOST_SIGNAL_CONNECTIONMAPPING
#define LOST_SIGNAL_CONNECTIONMAPPING

#include "lost/signal/SignalNode.h"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>


namespace lost
{
  namespace signal
  {

  struct ConnectionMapping
  {
    void rewire(boost::shared_ptr<SignalNode> leftnode, boost::shared_ptr<SignalNode> rightnode)
    {
      for(unsigned long i = 0; i<connections.size(); ++i)
      {
        leftnode->getSignal(connections[i].first)->connect(rightnode->getSlot(connections[i].second));
      }
    }
    
    void addConnection(const std::string& source, const std::string& dest)
    {
      connections.push_back(std::make_pair(source, dest));
    }
    
    const std::string& sourceName() { return source; }
    void sourceName(const std::string& name) { source = name; }
    const std::string& destinationName() { return destination; }
    void destinationName(const std::string& name) { destination = name; }
    
  private:
    std::string source;
    std::string destination;
    std::vector<std::pair<std::string, std::string> > connections; // we use a vector of pairs because one source can have multiple connections
  };

  }
}

#endif
