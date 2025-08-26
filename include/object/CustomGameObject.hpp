#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomGameObject : public CustomObjectUtils<ObjectType, GameObject> {
public:
    bool init(CustomObjectConfig config) {
        if (!GameObject::init(config.frame.c_str())) return false;

        this->m_objectID = config.id;
        this->m_parentMode = 10;
        this->m_objectType = GameObjectType::Solid;

        config.applyBoxSize(this);
        config.applyBoxOffset(this);
        config.applyObjectType(this);
        config.applyCustomRender(this);

        if (!config.detailFrame.empty()) this->addCustomColorChild(config.detailFrame);

        setupCustomObject();
        this->autorelease();

        return true;
    } // init

protected:
    virtual void setupCustomObject() {}
    virtual void resetCustomObject() {}

    void customObjectSetup(gd::vector<gd::string>& p0, gd::vector<void*>& p1) override {
        GameObject::customObjectSetup(p0, p1);
        setupCustomObject();
    } // customObjectSetup

    void resetObject() override {
        GameObject::resetObject();
        resetCustomObject();
    } // resetObject

    void addMainSpriteToParent(bool p0) override {
        bool disableBlend = (this->m_parentMode == 4);
        this->m_colorZLayerRelated = this->m_colorZLayerRelated || disableBlend;

        GameObject::addMainSpriteToParent(p0);

        this->m_shouldBlendBase = this->m_shouldBlendBase && !disableBlend;
        this->m_shouldBlendDetail = this->m_shouldBlendDetail && !disableBlend;
    } // addMainSpriteToParent
};

class CustomGameObjectBase : public CustomGameObject<CustomGameObjectBase> {};