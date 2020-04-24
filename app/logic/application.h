#ifndef _APPLICATION_H_
#define _APPLICATION_H_
#include <string>
#include <boost/serialization/singleton.hpp> 

class Application : public boost::serialization::singleton<Application> {
public:
  void Init(const std::string& data_dir);
  void UnInit();
};

#endif
