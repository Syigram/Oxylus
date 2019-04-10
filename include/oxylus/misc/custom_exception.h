#ifndef CUSTOM_EXCEPTION_H
#define CUSTOM_EXCEPTION_H


#include <cstdlib>

/**
 * Simple exception class to report problems
 */

class CustomException : public std::exception {
  std::string what_;
public:
  CustomException() : std::exception() {}
  virtual ~CustomException() { what_=""; }
  CustomException(const std::string str_) : what_(str_) {}
  virtual const char* what() const noexcept { return what_.c_str(); }
};


#endif
