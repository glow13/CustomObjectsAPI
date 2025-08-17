#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

template <class T>
class CustomTriggerObject : public EffectGameObject {
public:
    static T* create(CustomObjectConfig config) {
        auto obj = new T();
        if (obj->init(config)) return obj;

        delete obj;
        return nullptr;
    } // create

protected:
    virtual void setupCustomTrigger() {}
    virtual void resetCustomTrigger() {}
    virtual void activateCustomTrigger(GJBaseGameLayer* playLayer) {}

    bool EnhancedGameObjectInit(char const* p0) {
        if (GameObject::init(p0)) {
            CCNode::m_bUnkBool2 = false; // 0x139
            GameObject::m_classType = (GameObjectClassType)4; // 0x4d0
            return true;
        } // if
        return false;
    } // EnhancedGameObjectInit

    bool EffectGameObjectInit(char const* p0) {
        if (EnhancedGameObjectInit(p0)) {
            GameObject::m_classType = (GameObjectClassType)1; // 0x4d0
            EffectGameObject::m_triggerTargetColor.r = 0xff; // 0x598
            EffectGameObject::m_triggerTargetColor.g = 0xff; // 0x599
            EffectGameObject::m_triggerTargetColor.b = 0xff; // 0x59a
            EffectGameObject::m_legacyHSV = true; // 0x654
            EffectGameObject::m_duration = 0.5; // 0x59c
            return true;
        } // if
        return false;
    } // EffectGameObjectInit

    bool init(CustomObjectConfig config) {
        if (!EffectGameObjectInit(config.frame.c_str())) return false;

        m_objectID = config.id;
        m_parentMode = 10;
        m_objectType = GameObjectType::Modifier;

        config.applyBoxSize(this);
        config.applyBoxOffset(this);

        setupCustomTrigger();
        autorelease();

        return true;
    } // init

private:
    void customObjectSetup(gd::vector<gd::string>& p0, gd::vector<void*>& p1) override {
        EffectGameObject::customObjectSetup(p0, p1);
        m_isTrigger = true;
        setupCustomTrigger();
    } // customObjectSetup

    void resetObject() override {
        EffectGameObject::resetObject();
        resetCustomTrigger();
    } // resetObject

    void triggerObject(GJBaseGameLayer* p0, int p1, gd::vector<int> const* p2) override {
        activateCustomTrigger(p0);
    } // triggerObject
};

class CustomTriggerObjectBase : public CustomTriggerObject<CustomTriggerObjectBase> {};