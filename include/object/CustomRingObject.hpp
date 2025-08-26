#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomRingObject : public CustomObjectUtils<ObjectType, RingObject> {
public:
    bool init(CustomObjectConfig config) {
        if (!RingObject::init(config.frame.c_str())) return false;

        this->m_objectID = config.id;
        this->m_parentMode = 10;
        this->m_objectType = GameObjectType::CustomRing;

        this->m_width = 36;
        this->m_height = 36;
        this->m_duration = 0;
        this->m_usesAudioScale = true;
        this->m_unk532 = true; // green hitbox

        config.applyBoxSize(this);
        config.applyBoxOffset(this);
        config.applyCustomRender(this);

        this->setupCustomObject();
        this->autorelease();

        return true;
    } // init

protected:
    virtual void setupCustomObject() override { createRingParticles(); }
    virtual void pressCustomRing(PlayerObject*) {}

    // Returns nullptr if in the editor
    CCParticleSystemQuad* createRingParticles() {
        if (!PlayLayer::get() && this->m_hasNoParticles) return nullptr;
        return this->createAndAddParticle(36, "ringEffect.plist", 4, tCCPositionType::kCCPositionTypeGrouped);
    } // createRingParticle

private:
    void activatedByPlayer(PlayerObject* player) override {
        RingObject::activatedByPlayer(player);
        pressCustomRing(player);
    } // activatedByPlayer
};

class CustomRingObjectBase : public CustomRingObject<CustomRingObjectBase> {};