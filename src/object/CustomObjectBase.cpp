#include <Geode/Geode.hpp>
#include "../impl.hpp"

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