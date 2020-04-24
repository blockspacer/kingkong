#ifndef _APPLICATION_H_
#define _APPLICATION_H_
#include <boost/serialization/singleton.hpp> 

class Application : public boost::serialization::singleton<Application> {
public:
  void Init();
};

#endif
