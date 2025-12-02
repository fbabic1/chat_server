#ifndef LIB_CONFIG_CONFIGFACTORY_H
#define LIB_CONFIG_CONFIGFACTORY_H

#include <string>
#include <memory>
#include "lib/config/config.h"

namespace config {

class ConfigFactory {
public:
    static std::shared_ptr<Config> loadFromFile(const std::string& path);
    static std::shared_ptr<Config> loadFromString(const std::string& xml);
};

} // namespace config

#endif // LIB_CONFIG_CONFIGFACTORY_H
