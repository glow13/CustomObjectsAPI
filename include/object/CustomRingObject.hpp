#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

template <class T>
class CustomRingObject : public RingObject {
public:
    static T* create(CustomObjectConfig config) {
        auto obj = new T();
        if (obj->init(config)) return obj;

        delete obj;
        return nullptr;
    } // create

protected:
    virtual void setupCustomRing() { createRingParticles(); }
    virtual void pressCustomRing(PlayerObject* player) {}
    virtual void resetCustomRing() {}

    bool EnhancedGameObjectInit(char const* p0) {
        if (GameObject::init(p0)) {
            CCNode::m_bUnkBool2 = false; // 0x139
            GameObject::m_classType = (GameObjectClassType)4; // 0x4d0
            return true;
        } // if
        return false;
    } // EnhancedGameObjectInit

    bool EffectGameObjectInit(char const* p0) {
        if (EnhancedGameObjectInit(p0)) {
            GameObject::m_classType = (GameObjectClassType)1; // 0x4d0
            EffectGameObject::m_triggerTargetColor.r = 0xff; // 0x598
            EffectGameObject::m_triggerTargetColor.g = 0xff; // 0x599
            EffectGameObject::m_triggerTargetColor.b = 0xff; // 0x59a
            EffectGameObject::m_legacyHSV = true; // 0x654
            EffectGameObject::m_duration = 0.5; // 0x59c
            return true;
        } // if
        return false;
    } // EffectGameObjectInit

    bool RingObjectInit(char const* p0) {
        if (EffectGameObjectInit(p0)) {
            GameObject::m_unk421 = true; // 0x421
            EffectGameObject::m_isTouchTriggered = true; // 0x5b0
            return true;
        } // if
        return false;
    } // RingObjectInit

    bool init(CustomObjectConfig config) {
        if (!RingObjectInit(config.frame.c_str())) return false;

        m_objectID = config.id;
        m_parentMode = 10;
        m_objectType = GameObjectType::CustomRing;

        m_width = 36;
        m_height = 36;
        m_usesAudioScale = true;
        m_unk532 = true; // green hitbox

        m_claimTouch = false;
        m_isSpawnOnly = true;

        config.applyBoxSize(this);
        config.applyBoxOffset(this);

        setupCustomRing();
        autorelease();

        return true;
    } // init

    // Returns nullptr if in the editor
    CCParticleSystemQuad* createRingParticles() {
        if (PlayLayer::get() && !m_hasNoParticles) {
            return createAndAddParticle(36, "ringEffect.plist", 4, tCCPositionType::kCCPositionTypeGrouped);
        } // if
        return nullptr;
    } // createRingParticle

private:
    void customObjectSetup(gd::vector<gd::string>& p0, gd::vector<void*>& p1) override {
        RingObject::customObjectSetup(p0, p1);
        setupCustomRing();
    } // customObjectSetup

    void resetObject() override {
        RingObject::resetObject();
        resetCustomRing();
    } // resetObject

    void activatedByPlayer(PlayerObject* player) override {
        RingObject::activatedByPlayer(player);
        pressCustomRing(player);
    } // activatedByPlayer
};

class CustomRingObjectBase : public CustomRingObject<CustomRingObjectBase> {};