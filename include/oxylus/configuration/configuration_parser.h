#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>

namespace rdf  {


  class ConfigParser {
    public:
      ConfigParser();
      ConfigParser(const std::string &filename);
      ~ConfigParser();

  }

}

#endif /* CONFIG_PARSER_H */
