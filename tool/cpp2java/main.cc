#include <base/log.h>
#include <base/string_util.h>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>   
#include <boost/tokenizer.hpp>   
#include <boost/algorithm/string.hpp> 

#define NAMESPACE_KEYWORD "namespace"
#define ENUM_KEYWORD "enum"

void main(int argc, char* argv[] ) {
  //这里用来初始化一些全局配置
  std::string current_dir = boost::filesystem::current_path().string();
  BASE_LOG::InitLog(BASE_LOG::kLogLevelDebug, current_dir);
  if (argc != 3) {
    printf("参数不对: input.cpp output.java");
    return;
  }

  std::ifstream cpp_file(argv[1]);
  if (!cpp_file.good()) {
    printf("打开文件失败:%s", argv[1]);
    return;
  }
  std::ofstream java_file(argv[2], std::ios_base::out);
  java_file << "package com.mvvm;" << std::endl;
  java_file << "public class LogicDefine {" << std::endl;

  std::string cpp_line;
  int line_count = 0;
  std::string current_keyword;
  int32_t space_count = 4;
  while (!cpp_file.eof()) {
    std::getline(cpp_file, cpp_line);

    line_count++;
    std::vector<std::string> split_strings = BASE_STRING_UTIL::Split(cpp_line, " ,\t");
    if (split_strings.size() ==0) {
      continue;
    }
    std::string first_part = split_strings[0];
    if (first_part.empty()) {
      continue;
    }
    if (first_part.find("}") != std::string::npos) {
      //结尾
      if (current_keyword != ENUM_KEYWORD) {
        java_file << "\t" ;
      } else {
        java_file << "\t\t" ;
      }
      java_file << "}" << std::endl;
      current_keyword = "";
      continue;
    }

    if ((first_part == NAMESPACE_KEYWORD) || (first_part == ENUM_KEYWORD)) {
      if (first_part == NAMESPACE_KEYWORD) {
        java_file << "\t";
      } else {
        java_file << "\t\t" ;
      }

      java_file << "public static class " << split_strings[1] << "{" << std::endl;
      current_keyword = first_part;
      continue;
    }

    if (current_keyword == NAMESPACE_KEYWORD) {
      
    } else if (current_keyword == ENUM_KEYWORD) {
      //写入枚举值
      java_file << "\t\t\t" << "public static final int " << split_strings[0] << " = " << line_count << ";" << std::endl;

    }
  }

  java_file << "}" << std::endl;
}
