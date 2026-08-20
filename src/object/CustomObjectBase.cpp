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

void CustomObjectInterface::bindObjectProperty(int key, std::unique_ptr<ObjectPropertyInterface>&& property) {
    auto [prop, _] = m_impl->m_customProperties.emplace(key, std::move(property));
    if (auto it = m_impl->m_loadedProperties.find(key); it != m_impl->m_loadedProperties.end()) {
        prop->second->deserialize(it->second);
        m_impl->m_loadedProperties.erase(it);
    }
}

void CustomObjectInterface::setupObjectProperties(std::vector<std::string>& propValues, std::vector<void*>& propIsPresent) {
    for (auto& [key, prop] : m_impl->m_customProperties) {
        if (!propIsPresent[key]) continue;
        prop->deserialize(propValues[key]);
        propIsPresent[key] = nullptr;
    }

    for (int key = 4; key < propValues.size(); key++) {
        if (!propIsPresent[key]) continue;
        m_impl->m_loadedProperties.emplace(key, propValues[key]);
    }
}

std::string CustomObjectInterface::getCustomSaveString() const {
    std::string saveString = "";
    for (auto& [key, prop] : m_impl->m_customProperties) if (prop->isValid()) {
        saveString += fmt::format(",{},{}", key, prop->serialize());
    }
    return saveString;
}