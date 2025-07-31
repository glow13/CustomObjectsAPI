#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomEffectGameObject : public EffectGameObject {
public:
    static CustomEffectGameObject* create(int id);
    bool init(int id) {
        auto toolbox = ObjectToolbox::sharedState();
        auto spr = toolbox->intKeyToFrame(id);

        if (GameObject::init(spr) && customInit()) {
            log::info("EffectGameObject::init success!");

            m_objectID = id;
            m_parentMode = 10;

            setToDefault();
            autorelease();
            return true;
        } // if

        log::error("EffectGameObject::init failure!");

        return false;
    } // init

    virtual bool customInit() { return true; }
    virtual void setToDefault() { return; }
    virtual void onTrigger(GJBaseGameLayer* playLayer) { return; }

private:
    void triggerObject(GJBaseGameLayer* p0, int p1, gd::vector<int> const* p2) override {
        if (p0->m_attempts != m_currentAttempt) {
            m_currentAttempt = p0->m_attempts;
            this->setToDefault();
        } // if
        this->onTrigger(p0);
    } // triggerObject

    int m_currentAttempt = 0;
};