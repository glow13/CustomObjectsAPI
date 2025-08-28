#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomPadObject : public CustomObjectUtils<ObjectType, EffectGameObject> {
public:
    bool init(CustomObjectConfig config) {
        if (!EffectGameObject::init(config.frame.c_str())) return false;

        this->m_objectID = config.id;
        this->m_parentMode = 10;
        this->m_objectType = GameObjectType::PinkJumpPad;

        this->m_width = 25;
        this->m_height = 5;
        this->m_duration = 0;
        this->m_unk532 = true; // green hitbox

        config.applyBoxSize(this);
        config.applyBoxOffset(this);
        config.applyCustomRender(this);

        this->setupCustomObject();
        this->autorelease();

        return true;
    } // init

protected:
    virtual void setupCustomObject() override { createPadParticles(); }
    virtual void touchCustomPad(PlayerObject*) {}

    // Returns nullptr if in the editor
    CCParticleSystemQuad* createPadParticles() {
        if (!PlayLayer::get() || this->m_hasNoParticles) return nullptr;
        return this->createAndAddParticle(9, "bumpEffect.plist", 4, tCCPositionType::kCCPositionTypeGrouped);
    } // createPadParticles

private:
    void activatedByPlayer(PlayerObject* player) override {
        EffectGameObject::activatedByPlayer(player);
        touchCustomPad(player);
    } // activatedByPlayer
};

class CustomPadObjectBase : public CustomPadObject<CustomPadObjectBase> {};