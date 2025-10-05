#ifndef LIB_CONFIG_CONFIG_H
#define LIB_CONFIG_CONFIG_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <mutex>
#include <variant>
#include <exception>

namespace config {

class Config;

namespace internal {

using ConfigEntry = std::variant<
    long long int,
    bool,
    std::string,
    Config*
>;

class EntryNotFoundException : public std::runtime_error {
    EntryNotFoundException(const std::string& str) : std::runtime_error(str) {}
};

} // namespace internal

class Config {
public:
    bool getBoolean(const std::string& name) const;
    long long getInteger(const std::string& name) const;
    std::string getString(const std::string& name) const;

    bool getBooleanWithDefault(const std::string& name, bool defaultValue) const noexcept;
    long long getIntegerWithDefault(const std::string& name, long long defaultValue) const noexcept;
    std::string getStringWithDefault(const std::string& name, const std::string& defaultValue) const noexcept;

    Config* getConfig(const std::string& name) const noexcept;

private:
    template <typename T>
    T getValue(const std::string& name) const {
        std::lock_guard<std::recursive_mutex> guard(m_mutex);
        const auto iterator = m_map.find(name);
        if (iterator == m_map.end()) {
            throw internal::EntryNotFoundException("config::getValue: name does not exist in this config context");
        }

        const internal::ConfigEntry& value = iterator->second;
        if (std::holds_alternative<T>(value)) {
            throw std::logic_error("config::getValue: config context of invalid type");
        }
        return std::get<T>(value);
    }

    template <typename T>
    T getValueWithDefault(const std::string& name, const T defaultValue) const {
        try {
            return getValue<T>(name);
        } catch (const internal::EntryNotFoundException& e) {
            return std::move(defaultValue);
        }
    }

    template <typename T>
    void addValue(const std::string& name, const T value) {
        std::lock_guard<std::recursive_mutex> guard(m_mutex);
        
        auto result = m_map.insert_or_assign(name, internal::ConfigEntry(std::move(value)));
        const bool isInserted = result.second;
        if (isInserted) {
            std::clog << "Config Parsing: value " << name << "overriden";
        }
    }

private:
    void addBoolean(const std::string& name, bool value);
    void addInteger(const std::string& name, long long value);
    void addString(const std::string& name, const std::string value);
    void addConfig(const std::string& name, Config* value);

private:
    mutable std::recursive_mutex m_mutex;
    std::unordered_map< std::string, internal::ConfigEntry > m_map;
};

} // namespace lib::config

#endif