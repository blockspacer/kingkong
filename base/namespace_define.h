#ifndef _BASE_NAMESPACE_H_
#define _BASE_NAMESPACE_H_

#define BEGIN_NAMESPACE(ns) namespace ns {
#define END_NAMESPACE()    }


#define BASE_TIME base::time
#define BEGIN_NAMESPACE_TIME BEGIN_NAMESPACE(base) BEGIN_NAMESPACE(time)
#define END_NAMESPACE_TIME END_NAMESPACE() END_NAMESPACE()

#endif
