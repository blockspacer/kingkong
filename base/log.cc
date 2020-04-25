#include "log.h"
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/phoenix.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/filesystem.hpp>

BEGIN_NAMESPACE_LOG

boost::log::sources::severity_logger<LogLevel> s_named_logger;

BOOST_LOG_ATTRIBUTE_KEYWORD(log_severity, "Severity", LogLevel)
BOOST_LOG_ATTRIBUTE_KEYWORD(log_timestamp,
  "TimeStamp",
  boost::posix_time::ptime)

BOOST_LOG_ATTRIBUTE_KEYWORD(
  process_id,
  "ProcessID",
  boost::log::attributes::current_process_id::value_type)

BOOST_LOG_ATTRIBUTE_KEYWORD(
  thread_id,
  "ThreadID",
  boost::log::attributes::current_thread_id::value_type)


  // The formatting logic for the severity level
  template <typename CharT, typename TraitsT>
inline std::basic_ostream<CharT, TraitsT>& operator<<(
  std::basic_ostream<CharT, TraitsT>& strm,
  LogLevel lvl) {
  static const char* const str[] = { "trace", "debug", "info","warning",
                                    "error", "fatal" };
  if (static_cast<std::size_t>(lvl) < (sizeof(str) / sizeof(*str)))
    strm << str[lvl];
  else
    strm << static_cast<int>(lvl);
  return strm;
}

boost::log::attributes::current_process_id::value_type::native_type GetNativeProcessID(
  boost::log::value_ref<boost::log::attributes::current_process_id::value_type,
  tag::process_id> const& pid) {
  if (pid)
    return pid->native_id();
  return 0;
}

// Get Thread native ID
boost::log::attributes::current_thread_id::value_type::native_type GetNativeThreadID(
  boost::log::value_ref<boost::log::attributes::current_thread_id::value_type,
  tag::thread_id> const& tid) {
  if (tid)
    return tid->native_id();
  return 0;
}

void InitConsole(LogLevel log_level) {
  auto console_sink = boost::log::add_console_log();
  auto formatter_console =
    boost::log::expressions::stream << "["
    << boost::log::expressions::format_date_time(log_timestamp,
      "%Y-%m-%d %H:%M:%S.%f")
    << "]"
    << "[" << boost::phoenix::bind(&GetNativeProcessID, process_id.or_none())
    << ":" << boost::phoenix::bind(&GetNativeThreadID, thread_id.or_none()) << "]"
    << boost::log::expressions::format_named_scope(
      "Scope", boost::log::keywords::format = "%n (%f:%l)")
    << "\n<" << log_severity << ">" << boost::log::expressions::message;

  console_sink->set_formatter(formatter_console);
  console_sink->set_filter(log_severity >= log_level);
  boost::log::core::get()->add_sink(console_sink);
}

void InitLogFile(LogLevel log_level, const std::string& log_dir) {
  boost::filesystem::path log_part_name(log_dir);
  log_part_name = log_part_name / "%Y-%m-%d %H.log";
  auto file_sink = boost::log::add_file_log(
    boost::log::keywords::file_name = log_part_name,  //文件路径
    boost::log::keywords::rotation_size =
    10 * 1024 * 1024,  //单个文件限制大小
    boost::log::keywords::time_based_rotation =
    boost::log::sinks::file::rotation_at_time_interval(boost::posix_time::hours(1)),  //每小时重建
    boost::log::keywords::open_mode = std::ios::out | std::ios::app);

  file_sink->locked_backend()->set_file_collector(
    boost::log::sinks::file::make_collector(
      boost::log::keywords::target = log_dir,  //文件夹名
      boost::log::keywords::max_size =
      100 * 1024 * 1024,  //文件夹所占最大空间
      boost::log::keywords::min_free_space =
      100 * 1024 * 1024  //磁盘最小预留空间
    ));

  file_sink->set_filter(log_severity >= log_level);  //日志级别过滤

  file_sink->locked_backend()->scan_for_files();


  boost::log::formatter formatter_file =
    boost::log::expressions::stream << "["
    << boost::log::expressions::format_date_time(log_timestamp,
      "%Y-%m-%d %H:%M:%S.%f")
    << "]"
    << "[" << boost::phoenix::bind(&GetNativeProcessID, process_id.or_none())
    << ":" << boost::phoenix::bind(&GetNativeThreadID, thread_id.or_none()) << "]"
    << boost::log::expressions::format_named_scope(
      "Scope", boost::log::keywords::format = "%n (%f:%l)")
    << "\n<" << log_severity << ">" << boost::log::expressions::message;


  file_sink->set_formatter(formatter_file);
  file_sink->locked_backend()->auto_flush(true);
  boost::log::core::get()->add_global_attribute(
    "Scope", boost::log::attributes::named_scope());
  boost::log::core::get()->add_sink(file_sink);
}


void InitLog(LogLevel log_level, const std::string& log_dir) {
  InitConsole(log_level);
  InitLogFile(log_level, log_dir);
  boost::log::add_common_attributes();
}

void UnInitLog() {
  boost::log::core::get()->remove_all_sinks();
}

END_NAMESPACE_LOG

