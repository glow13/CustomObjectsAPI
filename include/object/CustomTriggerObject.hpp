#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

template <class T>
class CustomTriggerObject : public EffectGameObject {
public:
    static T* create(CustomObject config) {
        auto obj = new T();
        if (obj->init(config)) return obj;

        delete obj;
        return nullptr;
    } // create

protected:
    virtual void setupCustomTrigger() {}
    virtual void resetCustomTrigger() {}
    virtual void activateCustomTrigger(GJBaseGameLayer* playLayer) {}

    bool init(CustomObject config) {
        if (!EffectGameObject::init(config.frame.c_str())) return false;

        m_objectID = config.id;
        m_parentMode = 10;
        m_objectType = GameObjectType::Modifier;

        config.applyBoxSize(this);

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