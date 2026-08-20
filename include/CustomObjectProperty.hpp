#pragma once
#include <Geode/utils/base64.hpp>

class ObjectPropertyInterface {
public:
    virtual ~ObjectPropertyInterface() = default;

    virtual bool isValid() = 0;
    virtual std::string serialize() = 0;
    virtual void deserialize(const std::string&) = 0;
};

template <typename T>
requires (std::is_arithmetic_v<T> || std::is_convertible_v<T, std::string>)
class ObjectProperty : public ObjectPropertyInterface {
    T* m_value = nullptr;
    geode::Function<bool()> m_cond = nullptr;
public:
    ObjectProperty(T& value, geode::Function<bool()> cond) : 
        m_value(&value), m_cond(std::move(cond)) {}

    bool isValid() override {
        if (!m_value || (m_cond && !m_cond())) return false;

        if constexpr (std::is_convertible_v<T, std::string>)
            return !m_value->empty();
        else if constexpr (std::is_same_v<T, bool>)
            return *m_value;
        return true; // Always valid for arithmetic types
    }

    std::string serialize() override {
        if (!m_value) return "";

        if constexpr (std::is_convertible_v<T, std::string>)
            return geode::utils::base64::encode(*m_value);
        else if constexpr (std::is_same_v<T, bool>)
            return *m_value ? "1" : "0";
        else if constexpr (std::is_arithmetic_v<T>)
            return geode::utils::numToString<T>(*m_value);
        return ""; // Unknown type
    }

    void deserialize(const std::string& str) override {
        if (!m_value || str.empty()) return;

        if constexpr (std::is_convertible_v<T, std::string>)
            *m_value = geode::utils::base64::decodeString(str).unwrapOrDefault();
        else if constexpr (std::is_same_v<T, bool>)
            *m_value = (str == "1");
        else if constexpr (std::is_arithmetic_v<T>)
            *m_value = geode::utils::numFromString<T>(str).unwrapOrDefault();
        else *m_value = T(); // Unknown type
    }
};