#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectBase.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomRingObjectBase : public CustomObjectBase<ObjectType, RingObject> {
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!this->commonSetup(config)) return false;

        this->m_objectType = GameObjectType::CustomRing;
        this->m_width = 36;
        this->m_height = 36;
        this->m_duration = 0;
        this->m_usesAudioScale = true;
        this->m_greenDebugDraw = true;
        this->m_baseColor->m_defaultColorID = 0;

        return true;
    } // init

private:
    void activatedByPlayer(PlayerObject* player) override {
        RingObject::activatedByPlayer(player);
        this->activateCustomObject(player->m_gameLayer, player);
    } // activatedByPlayer

    void customSetup() override {
        if (!this->m_editorEnabled && !this->m_hasNoParticles) {
            this->createAndAddParticle(36, "ringEffect.plist", 4, tCCPositionType::kCCPositionTypeGrouped);
            this->claimParticle();
        } // if
        CustomObjectBase<ObjectType, RingObject>::customSetup();
    } // customSetup

    bool canAllowMultiActivate() override {
        return true;
    } // canAllowMultiActivate
};

class CustomRingObject : public CustomRingObjectBase<CustomRingObject> {};