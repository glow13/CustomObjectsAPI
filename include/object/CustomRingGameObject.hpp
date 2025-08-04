#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomRingGameObject : public RingObject {
public:
    static CustomRingGameObject* create(int id);
    virtual bool customInit() { return true; }
    virtual void onJump() {}

    bool init(int id) {
        auto toolbox = ObjectToolbox::sharedState();
        auto spr = toolbox->intKeyToFrame(id);

        if (EffectGameObject::init(spr) && customInit()) {
            m_objectID = id;
            m_parentMode = 10;
            m_objectType = GameObjectType::CustomRing;

            m_claimTouch = false;
            m_isSpawnOnly = true;

            autorelease();
            return true;
        } // if

        return false;
    } // init
};