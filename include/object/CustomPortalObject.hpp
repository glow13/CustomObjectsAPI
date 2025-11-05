#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectUtils.hpp"

using namespace geode::prelude;

template <class ObjectType>
class CustomPortalObjectBase : public CustomObjectUtils<ObjectType, EffectGameObject> {
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!this->commonSetup(config, false)) return false;
        
        this->m_objectType = GameObjectType::Modifier;
        this->m_width = 34;
        this->m_height = 86;
        this->m_duration = 0;
        this->m_greenDebugDraw = true;
        this->m_zFixedZLayer = true;
        this->m_defaultZLayer = ZLayer::T1;
        this->m_particleOffset = CCPoint(-5, 0);

        this->m_isTrigger = false;
        this->m_isSpawnTriggered = false;
        this->m_isTouchTriggered = true;
        this->m_isMultiTriggered = false;

        if (!config->mainSprite) this->setDontDraw(true);
        if (config->detailSprite) this->addPortalBackSprite(config->detailSprite, CCPoint(0, 0), -90);

        return true;
    } // init

    // Returns nullptr if in the editor
    CCSprite* addPortalBackSprite(gd::string frame, CCPoint offset, int zOrder) {
        if (auto level = PlayLayer::get()) {
            auto parent = static_cast<CCSprite*>(static_cast<CCNode*>(level->m_game2LayerB0));
            this->m_portalBack = this->addInternalChild(parent, frame, offset, zOrder);
            return this->m_portalBack;
        } else return nullptr;
    } // addPortalBackSprite

    // Returns nullptr if in the editor
    CCParticleSystemQuad* createPortalParticles() {
        if (this->m_editorEnabled || this->m_hasNoParticles) return nullptr;
        return this->createAndAddParticle(6, "portalEffect02.plist", 4, tCCPositionType::kCCPositionTypeGrouped);
    } // createPortalParticles

    void switchPlayerMode(GJBaseGameLayer* level, PlayerObject* player, GameObjectType type) {
        this->m_objectType = type;
        this->m_fakeType = type;

        level->playerWillSwitchMode(player, this);
        switch (type) {
            case GameObjectType::ShipPortal: level->switchToFlyMode(player, this, false, 5); break;
            case GameObjectType::BallPortal: level->switchToRollMode(player, this, false); break;
            case GameObjectType::UfoPortal: level->switchToFlyMode(player, this, false, 19); break;
            case GameObjectType::WavePortal: level->switchToFlyMode(player, this, false, 26); break;
            case GameObjectType::RobotPortal: level->switchToRobotMode(player, this, false); break;
            case GameObjectType::SpiderPortal: level->switchToSpiderMode(player, this, false); break;
            case GameObjectType::SwingPortal: level->switchToFlyMode(player, this, false, 41); break;
            default: player->switchedToMode(type); break;
        } // switch

        player->updatePlayerArt();
        player->updateDashArt();

        this->m_objectType = GameObjectType::Modifier;
        this->m_fakeType = GameObjectType::Modifier;
    } // switchPlayerMode

    void playShineEffect(GameObjectType type) {
        int id = this->m_objectID;
        switch (type) {
            case GameObjectType::InverseGravityPortal: this->m_objectID = 11; break;
            case GameObjectType::NormalGravityPortal: this->m_objectID = 10; break;
            case GameObjectType::ShipPortal: this->m_objectID = 13; break;
            case GameObjectType::CubePortal: this->m_objectID = 12; break;
            case GameObjectType::InverseMirrorPortal: this->m_objectID = 45; break;
            case GameObjectType::NormalMirrorPortal: this->m_objectID = 46; break;
            case GameObjectType::BallPortal: this->m_objectID = 47; break;
            case GameObjectType::RegularSizePortal: this->m_objectID = 99; break;
            case GameObjectType::MiniSizePortal: this->m_objectID = 101; break;
            case GameObjectType::UfoPortal: this->m_objectID = 111; break;
            case GameObjectType::DualPortal: this->m_objectID = 286; break;
            case GameObjectType::SoloPortal: this->m_objectID = 287; break;
            case GameObjectType::WavePortal: this->m_objectID = 660; break;
            case GameObjectType::RobotPortal: this->m_objectID = 745; break;
            case GameObjectType::TeleportPortal: this->m_objectID = 747; break;
            case GameObjectType::SpiderPortal: this->m_objectID = 1331; break;
            case GameObjectType::SwingPortal: this->m_objectID = 1933; break;
            case GameObjectType::GravityTogglePortal: this->m_objectID = 2926; break;
            default: break;
        } // switch

        this->GameObject::playShineEffect();
        this->m_objectID = id;
    } // playShineEffect

protected:
    virtual void setupCustomObject() override {
        this->createPortalParticles();
        CustomObjectUtils<ObjectType, EffectGameObject>::setupCustomObject();
    } // setupCustomObject

    virtual void touchCustomPortal(GJBaseGameLayer* level, PlayerObject* player) {
        this->activateCustomObject(level, player);
    } // touchCustomPortal

private:
    CCSprite* m_portalBack = nullptr;
    GameObjectType m_fakeType = GameObjectType::Modifier;

    void triggerObject(GJBaseGameLayer* level, int playerID, gd::vector<int> const*) override {
        auto player = (level->m_player2->m_uniqueID == playerID) ? level->m_player2 : level->m_player1;
        level->m_effectManager->removeTriggeredID(this->m_uniqueID, player->m_uniqueID);

        if (level->canBeActivatedByPlayer(player, this)) {
            player->m_lastActivatedPortal = this;
            player->m_lastPortalPos = this->getPosition();
            this->activatedByPlayer(player);
            touchCustomPortal(level, player);
        } // if
    } // triggerObject

    void setPosition(cocos2d::CCPoint const& p0) override {
        this->EffectGameObject::setPosition(p0);
        if (this->m_portalBack) this->m_portalBack->setPosition(p0);
    } // setPosition

    void setRotation(float p0) override {
        this->EffectGameObject::setRotation(p0);
        if (this->m_portalBack) this->m_portalBack->setRotation(p0);
    } // setRotation

    void setScale(float p0) override {
        this->EffectGameObject::setScale(p0);
        if (this->m_portalBack) this->m_portalBack->setScale(p0);
    } // setScale

    void setOpacity(unsigned char p0) override {
        this->EffectGameObject::setOpacity(p0);
        if (this->m_portalBack) this->m_portalBack->setOpacity(p0);
    } // setOpacity

    void setVisible(bool p0) override {
        this->EffectGameObject::setVisible(p0);
        if (this->m_portalBack) this->m_portalBack->setVisible(p0);
    } // setVisible

    bool hasBeenActivated() override {
        return EffectGameObject::hasBeenActivated() && this->m_isNoMultiActivate;
    } // hasBeenActivated

    GameObjectType getType() const override {
        return this->m_fakeType;
    } // getType
};

class CustomPortalObject : public CustomPortalObjectBase<CustomPortalObject> {};