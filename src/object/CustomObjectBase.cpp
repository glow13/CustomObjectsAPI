#include <Geode/Geode.hpp>
#include "CustomObjectBaseImpl.hpp"
#include "../config/CustomObjectConfigImpl.hpp"

using namespace geode::prelude;

CustomObjectInterface::~CustomObjectInterface() = default;
CustomObjectInterface::CustomObjectInterface() : m_impl(std::make_unique<Impl>()) {}

void CustomObjectInterface::setupCustomObject(GameObject* obj) const {
    if (m_impl->m_config && m_impl->m_config->m_impl->m_setupObject) m_impl->m_config->m_impl->m_setupObject(obj);
}

void CustomObjectInterface::resetCustomObject(GameObject* obj) const {
    if (m_impl->m_config && m_impl->m_config->m_impl->m_resetObject) m_impl->m_config->m_impl->m_resetObject(obj);
}

void CustomObjectInterface::activateCustomObject(GameObject* obj, GJBaseGameLayer* level, PlayerObject* player) const {
    if (m_impl->m_config && m_impl->m_config->m_impl->m_activateObject) m_impl->m_config->m_impl->m_activateObject(obj, level, player);
}

const CustomObjectConfig&& CustomObjectInterface::getConfig() const {
    return std::move(*m_impl->m_config);
}

void CustomObjectInterface::setupObjectProperties(std::vector<std::string>& propValues, std::vector<void*>& propIsPresent) {
    for (int key = 4; key < propValues.size(); key++) {
        if (!propIsPresent[key]) continue;

        if (auto it = m_impl->m_properties.find(key); it != m_impl->m_properties.end()) {
            it->second->deserialize(propValues[key]);
        } else m_impl->m_loadedProperties.emplace(key, propValues[key]);
    }
}

std::string CustomObjectInterface::getCustomSaveString() const {
    std::string saveString = "";
    for (auto& [key, prop] : m_impl->m_properties) if (prop->isValid()) {
        saveString += fmt::format(",{},{}", key, prop->serialize());
    }
    return saveString;
}

template<>
void CustomObjectInterface::bindObjectProperty<bool>(int key, bool& value, geode::Function<bool()> condition) {
    auto [prop, _] = m_impl->m_properties.emplace(key, std::make_unique<ObjectProperty<bool>>(value, std::move(condition)));
    if (auto it = m_impl->m_loadedProperties.find(key); it != m_impl->m_loadedProperties.end()) {
        prop->second->deserialize(it->second);
        m_impl->m_loadedProperties.erase(it);
    }
}

template<>
void CustomObjectInterface::bindObjectProperty<int>(int key, int& value, geode::Function<bool()> condition) {
    auto [prop, _] = m_impl->m_properties.emplace(key, std::make_unique<ObjectProperty<int>>(value, std::move(condition)));
    if (auto it = m_impl->m_loadedProperties.find(key); it != m_impl->m_loadedProperties.end()) {
        prop->second->deserialize(it->second);
        m_impl->m_loadedProperties.erase(it);
    }
}

template<>
void CustomObjectInterface::bindObjectProperty<float>(int key, float& value, geode::Function<bool()> condition) {
    auto [prop, _] = m_impl->m_properties.emplace(key, std::make_unique<ObjectProperty<float>>(value, std::move(condition)));
    if (auto it = m_impl->m_loadedProperties.find(key); it != m_impl->m_loadedProperties.end()) {
        prop->second->deserialize(it->second);
        m_impl->m_loadedProperties.erase(it);
    }
}

template<>
void CustomObjectInterface::bindObjectProperty<std::string>(int key, std::string& value, geode::Function<bool()> condition) {
    auto [prop, _] = m_impl->m_properties.emplace(key, std::make_unique<ObjectProperty<std::string>>(value, std::move(condition)));
    if (auto it = m_impl->m_loadedProperties.find(key); it != m_impl->m_loadedProperties.end()) {
        prop->second->deserialize(it->second);
        m_impl->m_loadedProperties.erase(it);
    }
}