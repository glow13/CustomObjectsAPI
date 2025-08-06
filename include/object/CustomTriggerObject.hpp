#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomTriggerObject : public EffectGameObject {
public:
    static CustomTriggerObject* create(int id);

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