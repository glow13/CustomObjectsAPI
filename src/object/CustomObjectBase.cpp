#include <Geode/Geode.hpp>
#include "../impl.hpp"

using namespace geode::prelude;

ICustomObjectBase::~ICustomObjectBase() = default;
ICustomObjectBase::ICustomObjectBase() : m_impl(std::make_unique<Impl>()) {}

void ICustomObjectBase::addConfig(const CustomObjectConfig* config) {
    m_impl->m_config = config;
}

void ICustomObjectBase::setupCustomObject(GameObject* obj) const {
    if (m_impl->m_config->m_impl->m_setupObject) m_impl->m_config->m_impl->m_setupObject(obj);
}
void ICustomObjectBase::resetCustomObject(GameObject* obj) const {
    if (m_impl->m_config->m_impl->m_resetObject) m_impl->m_config->m_impl->m_resetObject(obj);
}
void ICustomObjectBase::activateCustomObject(GameObject* obj, GJBaseGameLayer* level, PlayerObject* player) const {
    if (m_impl->m_config->m_impl->m_activateObject) m_impl->m_config->m_impl->m_activateObject(obj, level, player);
}