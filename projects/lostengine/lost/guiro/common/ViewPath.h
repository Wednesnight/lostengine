#ifndef LOST_GUIRO_COMMON_VIEWPATH_H
#define LOST_GUIRO_COMMON_VIEWPATH_H

#include <string>
#include <list>
#include <stdexcept>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/cstdint.hpp>

namespace lost
{
namespace guiro
{
namespace common
{

/** ViewPaths hold paths to views inside a view hierarchy, similar to xpath for XML.
 * The ViewPath class splits the path accoriding to the delimiter character,
 * stores the elements individually in a container, removes any empty elements and
 * superfluous blanks and provides safe accessors and manipulator functions.
 */
struct ViewPath
{  
  typedef std::list<std::string> PartsList;

  ViewPath(const std::string& inPath)
  {
    delimiter = "/";
    parseAndNormalize(inPath);
  }

  void reset(const std::string& inPath)
  {
    parseAndNormalize(inPath);
  }

  virtual ~ViewPath() {}

  const std::string& front() const
  {
    if(parts.size() > 0)
    {
      return parts.front();
    }
    else
    {
      throw std::runtime_error("ViewPath::first called on path with zero parts");
    }
  }

  void popFront()
  {
    if(parts.size() > 0)
    {
      parts.pop_front();
    }
    else
    {
      throw std::runtime_error("ViewPath::popFront called on path with zero parts");
    }
  }

  boost::uint32_t size() const { return parts.size(); }

private:
  PartsList parts;
  std::string delimiter;
  void parseAndNormalize(const std::string& inPath)
  {
    boost::algorithm::split(parts, inPath, boost::algorithm::is_any_of(delimiter));
    PartsList::iterator i;
    for(i = parts.begin(); i!=parts.end();)
    {
      boost::algorithm::trim(*i);
      if((*i).empty())
      {
        i = parts.erase(i);
      }
      else
      {
        ++i;
      }
    }
  }
};
}
}
}

#endif
