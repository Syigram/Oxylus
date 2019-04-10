#include <oxylus/memory/helper.h>


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

/* int Helper::PrintIterable(auto iterable) { */
/*   for (auto elem : iterable) { */
/*     std::cout << elem << std::endl; */
/*   } */
/* } */

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


int Helper::ImageFileCounter(std::string basePath, int start, int end){
  while (start < end){
    std::string fileName = ImageFileNameHandler(basePath, start);
    std::cout << fileName << std::endl;
    start++;
  }
  return 0;
}



std::string Helper::ImageFileNameHandler(std::string basePath, int number){
  std::string numberStr = std::to_string(number);
  if (number < 10)
    return basePath + "000000" + numberStr;
  else if (number < 100)
    return basePath + "00000" + numberStr;
  else if (number < 1000)
    return basePath + "0000" + numberStr;
  else if (number < 10000)
    return basePath + "000" + numberStr;
  else if (number < 100000)
    return basePath + "00" + numberStr;
}



