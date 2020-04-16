#ifndef _BASE_LOG_H_
#define _BASE_LOG_H_
#include <base/base_header.h>

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>

#include <boost/log/sources/logger.hpp>

#include <boost/log/support/date_time.hpp>

BEGIN_NAMESPACE_LOG

enum LogLevel {
  kLogLevelTrace = 0,
  kLogLevelDebug,
  kLogLevelInfo,
  kLogLevelWarning,
  kLogLevelError,
  kLogLevelFatal
};

extern boost::log::sources::severity_logger<LogLevel> s_named_logger;

void InitLog(LogLevel log_level, const std::string& log_dir);
void UnInitLog();

#define LogTrace    BOOST_LOG_FUNCTION();\
                    BOOST_LOG_SEV(BASE_LOG::s_named_logger, BASE_LOG::kLogLevelTrace)

#define LogDebug    BOOST_LOG_FUNCTION();\
                    BOOST_LOG_SEV(BASE_LOG::s_named_logger, BASE_LOG::kLogLevelDebug)

#define LogInfo     BOOST_LOG_FUNCTION();\
                    BOOST_LOG_SEV(BASE_LOG::s_named_logger, BASE_LOG::kLogLevelInfo)

#define LogWarning  BOOST_LOG_FUNCTION();\
                    BOOST_LOG_SEV(BASE_LOG::s_named_logger, BASE_LOG::kLogLevelWarning)

#define LogError    BOOST_LOG_FUNCTION();\
                    BOOST_LOG_SEV(BASE_LOG::s_named_logger, BASE_LOG::kLogLevelError)

#define LogFatal    DCHECK(false); \
                    BOOST_LOG_FUNCTION();\
                    BOOST_LOG_SEV(BASE_LOG::s_named_logger, BASE_LOG::kLogLevelFatal)

END_NAMESPACE_LOG
#endif