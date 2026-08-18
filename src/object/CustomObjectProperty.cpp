#include <Geode/utils/base64.hpp>
#include "CustomObjectProperty.hpp"

template <typename T>
requires(
    std::same_as<T, bool> || std::same_as<T, int> ||
    std::same_as<T, float> || std::same_as<T, std::string>
)
ObjectProperty<T>::ObjectProperty(T& value, geode::Function<bool()> cond) :
    m_value(&value), m_cond(std::move(cond)) {}

template <>
bool ObjectProperty<bool>::isValid() {
    return m_value && *m_value && (!m_cond || m_cond());
}

template <>
std::string ObjectProperty<bool>::serialize() {
    return m_value ? geode::utils::numToString<int>(*m_value) : "";
}

template <>
void ObjectProperty<bool>::deserialize(const std::string& str) {
    *m_value = geode::utils::numFromString<int>(str).unwrapOrDefault();
}

template <>
bool ObjectProperty<int>::isValid() {
    return m_value && true && (!m_cond || m_cond());
}

template <>
std::string ObjectProperty<int>::serialize() {
    return m_value ? geode::utils::numToString<int>(*m_value) : "";
}

template <>
void ObjectProperty<int>::deserialize(const std::string& str) {
    *m_value = geode::utils::numFromString<int>(str).unwrapOrDefault();
}

template <>
bool ObjectProperty<float>::isValid() {
    return m_value && true && (!m_cond || m_cond());
}

template <>
std::string ObjectProperty<float>::serialize() {
    return m_value ? geode::utils::numToString<float>(*m_value, 3) : "";
}

template <>
void ObjectProperty<float>::deserialize(const std::string& str) {
    *m_value = geode::utils::numFromString<float>(str).unwrapOrDefault();
}

template <>
bool ObjectProperty<std::string>::isValid() {
    return m_value && !m_value->empty() && (!m_cond || m_cond());
}

template <>
std::string ObjectProperty<std::string>::serialize() {
    return m_value ? geode::utils::base64::encode(*m_value) : "";
}

template <>
void ObjectProperty<std::string>::deserialize(const std::string& str) {
    *m_value = geode::utils::base64::decodeString(str).unwrapOrDefault();
}

template struct ObjectProperty<bool>;
template struct ObjectProperty<int>;
template struct ObjectProperty<float>;
template struct ObjectProperty<std::string>;