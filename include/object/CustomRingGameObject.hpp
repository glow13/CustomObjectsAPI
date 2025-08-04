#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomRingGameObject : public RingObject {
public:
    static CustomRingGameObject* create(int id);
    virtual void setupCustomRing(CCParticleSystemQuad* particle) {}
    virtual void pressCustomRing(PlayerObject* player) {}
    virtual void resetCustomRing() {}

    bool init(int id) {
        auto toolbox = ObjectToolbox::sharedState();
        auto spr = toolbox->intKeyToFrame(id);

        if (EffectGameObject::init(spr)) {
            m_objectID = id;
            m_parentMode = 10;
            m_objectType = GameObjectType::CustomRing;

            m_width = 36;
            m_height = 36;
            m_usesAudioScale = true; // audio pulse
            m_unk532 = true; // green hitbox

            m_claimTouch = false;
            m_isSpawnOnly = true;

            autorelease();
            return true;
        } // if

        return false;
    } // init

    void customObjectSetup(gd::vector<gd::string>& p0, gd::vector<void*>& p1) override {
        RingObject::customObjectSetup(p0, p1);

        CCParticleSystemQuad* particle = nullptr;
        if (PlayLayer::get() && !m_hasNoParticles) {
            particle= createAndAddParticle(36, "ringEffect.plist", 4, tCCPositionType::kCCPositionTypeGrouped);
        } // if

        setupCustomRing(particle);
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