#include "helper.h"


#include <array>
#include <sstream>
#include <iostream>
#include <cctype>


std::string Helper::run_shell_cmd(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}


int Helper::extract_integer_from_str(std::string input){
  std::string temp_str = "";
  int size = input.size();
  int result = 0;
  for (size_t i = 0; i < size; i++) {
    while (isdigit(input[i]) && i < size) {
      temp_str += input[i];
      i++;
    }
  }
  std::istringstream stream(temp_str);
  stream >> result;
  return result;
}
