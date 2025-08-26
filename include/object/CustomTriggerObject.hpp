#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomTriggerObject : public CustomObjectUtils<ObjectType, EffectGameObject> {
public:
    bool init(CustomObjectConfig config) {
        if (!EffectGameObject::init(config.frame.c_str())) return false;

        this->m_objectID = config.id;
        this->m_parentMode = 10;
        this->m_objectType = GameObjectType::Modifier;
        this->m_isTrigger = true;

        config.applyBoxSize(this);
        config.applyBoxOffset(this);
        config.applyCustomRender(this);

        this->setupCustomObject();
        this->autorelease();

        return true;
    } // init

protected:
    virtual void activateCustomTrigger(GJBaseGameLayer*) {}

private:
    void customObjectSetup(gd::vector<gd::string>& p0, gd::vector<void*>& p1) override {
        CustomObjectUtils<ObjectType, EffectGameObject>::customObjectSetup(p0, p1);
        this->setDontDraw(PlayLayer::get());
        this->m_isTrigger = true;
        this->setupCustomObject();
    } // customObjectSetup

    void triggerObject(GJBaseGameLayer* p0, int, gd::vector<int> const*) override {
        activateCustomTrigger(p0);
    } // triggerObject
};

class CustomTriggerObjectBase : public CustomTriggerObject<CustomTriggerObjectBase> {};