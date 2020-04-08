﻿#ifndef _BASE_NAMESPACE_H_
#define _BASE_NAMESPACE_H_

#define BEGIN_NAMESPACE(ns) namespace ns {
#define END_NAMESPACE()    }


#define BASE_TIME base::time
#define BEGIN_NAMESPACE_TIME BEGIN_NAMESPACE(base) BEGIN_NAMESPACE(time)
#define END_NAMESPACE_TIME END_NAMESPACE() END_NAMESPACE()


#define BASE_LOOPER base::looper
#define BEGIN_NAMESPACE_LOOPER BEGIN_NAMESPACE(base) BEGIN_NAMESPACE(looper)
#define END_NAMESPACE_LOOPER END_NAMESPACE() END_NAMESPACE()


#define BASE_LOG base::log
#define BEGIN_NAMESPACE_LOG BEGIN_NAMESPACE(base) BEGIN_NAMESPACE(log)
#define END_NAMESPACE_LOG END_NAMESPACE() END_NAMESPACE()


#endif
