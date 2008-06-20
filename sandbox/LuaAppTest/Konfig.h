#ifndef KONFIG_H
#define KONFIG_H

#include <map>
#include <string>
#include <iostream>

struct Konfig
{
  std::string& get(const std::string& inKey)
  {
    std::cout << "GETTING" << std::endl;
    return vals[inKey];
  }
  
  void set(const std::string& inKey, const std::string& inVal)
  {
    std::cout << "SETTING" << std::endl;
    vals[inKey] = inVal;
  }
  
  std::map<std::string, std::string> vals;
};

#endif