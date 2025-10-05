#include "lib/config/config.h"

namespace config {

bool Config::getBoolean(const std::string& name) const {
    return getValue<bool>(name);
}

long long Config::getInteger(const std::string& name) const {
    return getValue<long long>(name);
}

std::string Config::getString(const std::string& name) const {
    return getValue<std::string>(name);
}

bool Config::getBooleanWithDefault(const std::string& name, bool defaultValue) const {
    return getValueWithDefault<bool>(name, defaultValue);
}

long long Config::getIntegerWithDefault(const std::string& name, long long defaultValue) const {
    return getValueWithDefault<long long>(name, defaultValue);
}

std::string Config::getStringWithDefault(const std::string& name, const std::string& defaultValue) const {
    return getValueWithDefault<std::string>(name, std::move(defaultValue));
}

Config* Config::getConfig(const std::string& name) const {
    return getValueWithDefault<Config*>(name, nullptr);
}

void Config::addInteger(const std::string& name, long long value) {
    addValue<long long>(name, value);
}

void Config::addBoolean(const std::string& name, bool value) {
    addValue<bool>(name, value);
}

void Config::addString(const std::string& name, const std::string value) {
    addValue<std::string>(name, std::move(value));
}

void Config::addConfig(const std::string& name, Config* value) {
    addValue<Config*>(name, value);
}

} // namespace lib::config