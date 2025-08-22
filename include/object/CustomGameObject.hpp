#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

template <class T>
class CustomGameObject : public GameObject {
public:
    static T* create(CustomObjectConfig config) {
        auto obj = new T();
        if (obj->init(config)) return obj;

        delete obj;
        return nullptr;
    } // create

protected:
    virtual void setupCustomObject() {}
    virtual void resetCustomObject() {}

    bool init(CustomObjectConfig config) {
        if (!GameObject::init(config.frame.c_str())) return false;

        m_objectID = config.id;
        m_parentMode = 10;
        m_objectType = GameObjectType::Solid;

        config.applyBoxSize(this);
        config.applyBoxOffset(this);
        config.applyObjectType(this);
        config.applyCustomRender(this);

        if (!config.detailFrame.empty()) addCustomColorChild(config.detailFrame);

        setupCustomObject();
        autorelease();

        return true;
    } // init

private:
    void customObjectSetup(gd::vector<gd::string>& p0, gd::vector<void*>& p1) override {
        GameObject::customObjectSetup(p0, p1);
        setupCustomObject();
    } // customObjectSetup

    void resetObject() override {
        GameObject::resetObject();
        resetCustomObject();
    } // resetObject

    void addMainSpriteToParent(bool p0) override {
        bool disableBlend = (m_parentMode == 4);
        m_colorZLayerRelated = m_colorZLayerRelated || disableBlend;

        GameObject::addMainSpriteToParent(p0);

        m_shouldBlendBase = m_shouldBlendBase && !disableBlend;
        m_shouldBlendDetail = m_shouldBlendDetail && !disableBlend;
    } // addMainSpriteToParent
};

class CustomGameObjectBase : public CustomGameObject<CustomGameObjectBase> {};