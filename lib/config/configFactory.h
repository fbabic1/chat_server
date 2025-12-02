#ifndef LIB_CONFIG_CONFIGFACTORY_H
#define LIB_CONFIG_CONFIGFACTORY_H

#include <string>
#include "lib/config/config.h"

namespace config {

class ConfigFactory {
public:
    // Load a configuration from an XML file. Returns a pointer to a heap
    // allocated Config instance; caller is responsible for its lifetime.
    static std::shared_ptr<Config> loadFromFile(const std::string& path);

    // Parse configuration from an XML string. Same ownership rules.
    static std::shared_ptr<Config> loadFromString(const std::string& xml);
};

} // namespace config

#endif // LIB_CONFIG_CONFIGFACTORY_H
