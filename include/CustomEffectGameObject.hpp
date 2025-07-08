#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomEffectGameObject : public EffectGameObject {
public:
    static CustomEffectGameObject* create(int id);
    bool init(int id) {
        auto toolbox = ObjectToolbox::sharedState();
        auto spr = toolbox->intKeyToFrame(id);

        if (EffectGameObject::init(spr) && customInit()) {
            m_objectID = id;
            m_parentMode = 10;

            setToDefault();
            autorelease();
            return true;
        } // if
        return false;
    } // init

    virtual bool customInit();
    virtual void setToDefault();
    virtual void onTrigger(GJBaseGameLayer* playLayer);
    int m_bouncePower;

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