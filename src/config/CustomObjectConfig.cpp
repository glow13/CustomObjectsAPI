#include "CustomObjectConfig.hpp"
#include "CustomSpriteConfig.hpp"
#include "../manager/CustomObjectsManager.hpp"

using namespace geode::prelude;

struct CustomObjectConfig::Impl {
    std::string m_id;
    std::string m_mod;
    int m_objectID;

    ObjectConstructor m_ctor;
    EditObjectCallback m_editObject;
    EditObjectCallback m_editSpecial;

    std::unique_ptr<CustomSpriteConfig> m_mainSprite;
    std::unique_ptr<CustomSpriteConfig> m_detailSprite;
    std::unique_ptr<CustomSpriteConfig> m_glowSprite;
};

CustomObjectConfig::~CustomObjectConfig() = default;
CustomObjectConfig::CustomObjectConfig(std::string_view id, int objectID, ObjectConstructor ctor) : m_impl(std::make_unique<Impl>()) {
    m_impl->m_id = std::string(id);
    m_impl->m_mod = id.substr(0, id.find("/"));
    m_impl->m_objectID = objectID;
    m_impl->m_ctor = ctor;
}


std::string CustomObjectConfig::getID() const
    { return m_impl->m_id; }
std::string CustomObjectConfig::getModID() const
    { return m_impl->m_mod; }
int CustomObjectConfig::getObjectID() const
    { return m_impl->m_objectID; }

std::string CustomObjectConfig::getMainSprite() const
    { return hasMainSprite() ? m_impl->m_mainSprite->getFrameName() : ""; }
std::string CustomObjectConfig::getDetailSprite() const
    { return hasDetailSprite() ? m_impl->m_detailSprite->getFrameName() : ""; }
std::string CustomObjectConfig::getGlowSprite() const
    { return hasGlowSprite() ? m_impl->m_glowSprite->getFrameName() : ""; }

bool CustomObjectConfig::hasMainSprite() const
    { return m_impl->m_mainSprite != nullptr; }
bool CustomObjectConfig::hasDetailSprite() const
    { return m_impl->m_detailSprite != nullptr; }
bool CustomObjectConfig::hasGlowSprite() const
    { return m_impl->m_glowSprite != nullptr; }

CustomObjectConfig&& CustomObjectConfig::setMainSprite(std::string frame, int x, int y, int w, int h)
    { m_impl->m_mainSprite = std::make_unique<CustomSpriteConfig>(this, frame, x, y, w, h, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setMainSprite(std::string frame, int w, int h)
    { m_impl->m_mainSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, w, h, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setMainSprite(std::string frame, int s)
    { m_impl->m_mainSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, s, s, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setMainSprite(std::string frame, bool sheet)
    { m_impl->m_mainSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, 0, 0, sheet); return (CustomObjectConfig&&)*this; }

CustomObjectConfig&& CustomObjectConfig::setDetailSprite(std::string frame, int x, int y, int w, int h)
    { m_impl->m_detailSprite = std::make_unique<CustomSpriteConfig>(this, frame, x, y, w, h, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setDetailSprite(std::string frame, int w, int h)
    { m_impl->m_detailSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, w, h, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setDetailSprite(std::string frame, int s)
    { m_impl->m_detailSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, s, s, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setDetailSprite(std::string frame, bool sheet)
    { m_impl->m_detailSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, 0, 0, sheet); return (CustomObjectConfig&&)*this; }

CustomObjectConfig&& CustomObjectConfig::setGlowSprite(std::string frame, int x, int y, int w, int h)
    { m_impl->m_glowSprite = std::make_unique<CustomSpriteConfig>(this, frame, x, y, w, h, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setGlowSprite(std::string frame, int w, int h)
    { m_impl->m_glowSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, w, h, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setGlowSprite(std::string frame, int s)
    { m_impl->m_glowSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, s, s, true); return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::setGlowSprite(std::string frame, bool sheet)
    { m_impl->m_glowSprite = std::make_unique<CustomSpriteConfig>(this, frame, 0, 0, 0, 0, sheet); return (CustomObjectConfig&&)*this; }

CustomObjectConfig&& CustomObjectConfig::onEditObjectButton(EditObjectCallback callback)
    { m_impl->m_editObject = callback; return (CustomObjectConfig&&)*this; }
CustomObjectConfig&& CustomObjectConfig::onEditSpecialButton(EditObjectCallback callback)
    { m_impl->m_editSpecial = callback; return (CustomObjectConfig&&)*this; }

bool CustomObjectConfig::hasEditObjectCallback() const
    { return m_impl->m_editObject != nullptr; }
bool CustomObjectConfig::hasEditSpecialCallback() const
    { return m_impl->m_editSpecial != nullptr; }

void CustomObjectConfig::customEditObject(GameObject* obj, cocos2d::CCArray* objs) const
    { if (m_impl->m_editObject != nullptr) m_impl->m_editObject(obj, objs); }
void CustomObjectConfig::customEditSpecial(GameObject* obj, cocos2d::CCArray* objs) const
    { if (m_impl->m_editSpecial != nullptr) m_impl->m_editSpecial(obj, objs); }

GameObject* CustomObjectConfig::createCustomObject() const {
    if (!m_impl->m_ctor) return nullptr;

    GameObject* obj = m_impl->m_ctor(this);
    obj->m_objectID = m_impl->m_objectID;

    return obj;
}

CustomObjectConfig* CustomObjectConfig::registerConfig(std::string_view stringID, ObjectConstructor ctor) {
    return CustomObjectsManager::get()->registerObjectConfig(stringID, ctor);
}