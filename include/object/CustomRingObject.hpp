#pragma once

#include "../CustomObjectBase.hpp"

template <class ObjectType>
class $base(CustomRingObject, RingObject) {
protected:
    using CustomObjectBase = CustomObjectBase<ObjectType, RingObject>::Base;
    using CustomRingObject = CustomRingObjectBase<ObjectType>;
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!CustomObjectBase::init(config)) return false;

        this->m_objectType = GameObjectType::CustomRing;
        this->m_baseColor->m_defaultColorID = 0;
        this->m_usesAudioScale = true;
        this->m_greenDebugDraw = true;
        this->m_width = 36;
        this->m_height = 36;

        return true;
    } // init

    void activatedByPlayer(PlayerObject* player) override final {
        CustomObjectBase::activatedByPlayer(player);
        this->activateCustomObject(player->m_gameLayer, player);
    } // activatedByPlayer

    void customSetup() override final {
        if (!this->m_editorEnabled && !this->m_hasNoParticles) {
            this->createAndAddParticle(36, "ringEffect.plist", 4, tCCPositionType::kCCPositionTypeGrouped);
            this->claimParticle();
        } // if
        CustomObjectBase::customSetup();
        this->m_dontIgnoreDuration = false;
    } // customSetup

    bool canAllowMultiActivate() override {
        return true;
    } // canAllowMultiActivate
};

class CUSTOM_OBJECTS_DLL $generic(CustomRingObject);