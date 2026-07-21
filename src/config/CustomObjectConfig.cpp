#include <Geode/Geode.hpp>
#include "CustomObjectConfig.hpp"
#include "../manager/CustomObjectsManager.hpp"

using namespace geode::prelude;

struct CustomObjectConfig::Impl {
    ObjectConstructor m_ctor;
    int m_objectID;
};

CustomObjectConfig::~CustomObjectConfig() = default;
CustomObjectConfig::CustomObjectConfig(int objectID, ObjectConstructor ctor) : m_impl(std::make_unique<CustomObjectConfig::Impl>()) {
    m_impl->m_objectID = objectID;
    m_impl->m_ctor = ctor;
}

GameObject* CustomObjectConfig::createCustomObject() const {
    if (!m_impl->m_ctor) return nullptr;

    auto obj = m_impl->m_ctor();
    if (obj->init("")) {
        obj->autorelease();
        return obj;
    }

    delete obj;
    return nullptr;
}

int CustomObjectConfig::getObjectID() const {
    return m_impl->m_objectID;
}

CustomObjectConfig* CustomObjectConfig::registerConfig(std::string_view stringID, ObjectConstructor ctor) {
    return CustomObjectsManager::get()->registerObjectConfig(stringID, ctor);
}