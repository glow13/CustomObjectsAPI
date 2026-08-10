#include <Geode/Geode.hpp>
#include "../impl.hpp"

using namespace geode::prelude;

ICustomObjectBase::~ICustomObjectBase() = default;
ICustomObjectBase::ICustomObjectBase() : m_impl(std::make_unique<Impl>()) {}

void ICustomObjectBase::addConfig(const CustomObjectConfig* config) {
    m_impl->m_config = config;
}

void ICustomObjectBase::setupCustomObject(GameObject* obj) const {
    m_impl->m_config->m_impl->m_setupCallback(obj);
}
void ICustomObjectBase::resetCustomObject(GameObject* obj) const {
    m_impl->m_config->resetCustomObject(obj);
}
void ICustomObjectBase::activateCustomObject(GameObject* obj, GJBaseGameLayer* level, PlayerObject* player) const {
    m_impl->m_config->activateCustomObject(obj, level, player);
}