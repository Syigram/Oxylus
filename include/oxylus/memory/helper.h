#ifndef HELPER_H
#define HELPER_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <random>

class Helper {
private:
  /* data */



public:
  Helper(){};
  std::string run_shell_cmd(const char* cmd_command);
  int extract_integer_from_str(std::string input);
  int ImageFileCounter(std::string basePath, int start, int end);
  /* static int PrintIterable(auto iterable); */
  static std::string ImageFileNameHandler(std::string basePath, int number);
  static std::mt19937 InitializeRandomDevice();
  // virtual ~Helper ();
};



#endif /* HELPER_H */
