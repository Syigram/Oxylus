#ifndef HELPER_H
#define HELPER_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

class Helper {
private:
  /* data */



public:
  Helper(){};
  std::string run_shell_cmd(const char* cmd_command);
  int extract_integer_from_str(std::string input);
  // virtual ~Helper ();
};



#endif /* HELPER_H */
