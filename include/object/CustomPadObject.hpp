#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomPadObjectBase : public CustomObjectUtils<ObjectType, EffectGameObject> {
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!this->commonSetup(config)) return false;

        this->m_objectType = GameObjectType::Modifier;
        this->m_width = 25;
        this->m_height = 5;
        this->m_duration = 0;
        this->m_greenDebugDraw = true;

        this->m_isTrigger = false;
        this->m_isSpawnTriggered = false;
        this->m_isTouchTriggered = true;
        this->m_isMultiTriggered = false;

        return true;
    } // init

protected:
    virtual void setupCustomObject() override { createPadParticles(); }
    virtual void touchCustomPad(GJBaseGameLayer* level, PlayerObject* player) {}

    // Returns nullptr if in the editor
    CCParticleSystemQuad* createPadParticles() {
        if (this->m_editorEnabled || this->m_hasNoParticles) return nullptr;
        return this->createAndAddParticle(9, "bumpEffect.plist", 4, tCCPositionType::kCCPositionTypeGrouped);
    } // createPadParticles

    // Copies the behaviour of a regular pad, with adjustable bump strength
    void bumpPlayer(PlayerObject* player, float power, GameObjectType effectType = GameObjectType::YellowJumpPad) {
        player->bumpPlayer(power, (int)effectType, this->m_hasNoEffects, this);
    } // bumpPlayer

private:
    void triggerObject(GJBaseGameLayer* level, int playerID, gd::vector<int> const*) override {
        auto player = (level->m_player2->m_uniqueID == playerID) ? level->m_player2 : level->m_player1;
        level->m_effectManager->removeTriggeredID(this->m_uniqueID, player->m_uniqueID);

        if (level->canBeActivatedByPlayer(player, this)) {
            player->m_lastActivatedPortal = this;
            player->m_lastPortalPos = this->getPosition();
            this->activatedByPlayer(player);
            touchCustomPad(level, player);
        } // if
    } // triggerObject
};

class CustomPadObject : public CustomPadObjectBase<CustomPadObject> {};