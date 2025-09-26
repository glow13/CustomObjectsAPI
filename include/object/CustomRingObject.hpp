#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomRingObjectBase : public CustomObjectUtils<ObjectType, RingObject> {
public:
    bool init(const CustomObjectConfig& config) {
        if (!this->commonSetup(config)) return false;
        
        this->m_objectType = GameObjectType::CustomRing;
        this->m_width = 36;
        this->m_height = 36;
        this->m_duration = 0;
        this->m_usesAudioScale = true;
        this->m_unk532 = true; // green hitbox

        return true;
    } // init

protected:
    virtual void setupCustomObject() override { createRingParticles(); }
    virtual void pressCustomRing(PlayerObject* player) {}

    // Returns nullptr if in the editor
    CCParticleSystemQuad* createRingParticles() {
        if (this->m_editorEnabled || this->m_hasNoParticles) return nullptr;
        return this->createAndAddParticle(36, "ringEffect.plist", 4, tCCPositionType::kCCPositionTypeGrouped);
    } // createRingParticles

private:
    void activatedByPlayer(PlayerObject* player) override {
        RingObject::activatedByPlayer(player);
        pressCustomRing(player);
    } // activatedByPlayer
};

class CustomRingObject : public CustomRingObjectBase<CustomRingObject> {};