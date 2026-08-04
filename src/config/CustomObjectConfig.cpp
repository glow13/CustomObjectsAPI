#include "CustomObjectConfig.hpp"
#include "../manager/CustomObjectsManager.hpp"

using namespace geode::prelude;

struct CustomObjectConfig::Impl {
    ObjectConstructor m_ctor;
    EditObjectCallback m_editObject;
    EditObjectCallback m_editSpecial;
    int m_objectID;
};

CustomObjectConfig::~CustomObjectConfig() = default;
CustomObjectConfig::CustomObjectConfig(int objectID, ObjectConstructor ctor) : m_impl(std::make_unique<CustomObjectConfig::Impl>()) {
    m_impl->m_objectID = objectID;
    m_impl->m_ctor = ctor;
}

GameObject* CustomObjectConfig::createCustomObject() const {
    if (!m_impl->m_ctor) return nullptr;
    return m_impl->m_ctor(this);
}

int CustomObjectConfig::getObjectID() const {
    return m_impl->m_objectID;
}

void CustomObjectConfig::onEditObjectButton(EditObjectCallback callback) {
    m_impl->m_editObject = callback;
}

void CustomObjectConfig::onEditSpecialButton(EditObjectCallback callback) {
    m_impl->m_editSpecial = callback;
}

bool CustomObjectConfig::hasEditObjectCallback() const {
    return m_impl->m_editObject != nullptr;
}

bool CustomObjectConfig::hasEditSpecialCallback() const {
    return m_impl->m_editSpecial != nullptr;
}

void CustomObjectConfig::customEditObject(GameObject* obj, cocos2d::CCArray* objs) const {
    if (m_impl->m_editObject != nullptr) m_impl->m_editObject(obj, objs);
}

void CustomObjectConfig::customEditSpecial(GameObject* obj, cocos2d::CCArray* objs) const {
    if (m_impl->m_editSpecial != nullptr) m_impl->m_editSpecial(obj, objs);
}

CustomObjectConfig* CustomObjectConfig::registerConfig(std::string_view stringID, ObjectConstructor ctor) {
    return CustomObjectsManager::get()->registerObjectConfig(stringID, ctor);
}