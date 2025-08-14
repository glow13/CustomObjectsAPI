#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

template <class T>
class CustomTriggerObjectBase : public EffectGameObject {
public:
    static T* create(int id) {
        auto obj = new T();
        if (obj->init(id)) return obj;

        delete obj;
        return nullptr;
    } // create

    virtual void setupCustomTrigger() {}
    virtual void resetCustomTrigger() {}
    virtual void activateCustomTrigger(GJBaseGameLayer* playLayer) {}

    bool init(int id) {
        auto toolbox = ObjectToolbox::sharedState();
        auto spr = toolbox->intKeyToFrame(id);

        if (EffectGameObject::init(spr)) {
            m_objectID = id;
            m_parentMode = 10;
            m_objectType = GameObjectType::Modifier;

            autorelease();
            return true;
        } // if

        return false;
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

class CustomTriggerObject : public CustomTriggerObjectBase<CustomTriggerObject> {};