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

    bool init(CustomObjectConfig config) {
        if (!EffectGameObject::init(config.frame.c_str())) return false;

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