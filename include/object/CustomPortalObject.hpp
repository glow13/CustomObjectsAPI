#pragma once

#include "../CustomObjectBase.hpp"

enum GameModeType : int {
    CUBE = (int)GameObjectType::CubePortal,
    SHIP = (int)GameObjectType::ShipPortal,
    BALL = (int)GameObjectType::BallPortal,
    UFO = (int)GameObjectType::UfoPortal,
    WAVE = (int)GameObjectType::WavePortal,
    ROBOT = (int)GameObjectType::RobotPortal,
    SPIDER = (int)GameObjectType::SpiderPortal,
    SWING = (int)GameObjectType::SwingPortal,
};

template <class ObjectType>
class $base(CustomPortalObject, EffectGameObject) {
protected:
    using CustomObjectBase = CustomObjectBase<ObjectType, EffectGameObject>::Base;
    using CustomPortalObject = CustomPortalObjectBase<ObjectType>;
public:
    bool init(const CustomObjectConfig<ObjectType>* config) {
        if (!EffectGameObject::init(config->getMainSprite().c_str())) return false;

        this->m_objectType = GameObjectType::Modifier;
        this->m_baseColor->m_defaultColorID = 0;
        this->m_width = 34;
        this->m_height = 86;
        this->m_greenDebugDraw = true;
        this->m_zFixedZLayer = true;
        this->m_defaultZLayer = ZLayer::T1;
        this->m_particleOffset = CCPoint(-5, 0);

        this->m_isTrigger = false;
        this->m_isSpawnTriggered = false;
        this->m_isTouchTriggered = true;
        this->m_isMultiTriggered = false;

        if (!config->hasMainSprite()) this->setDontDraw(true);
        if (!this->m_editorEnabled && config->hasDetailSprite()) {
            auto parent = cast::base_cast<CCSprite*>(GJBaseGameLayer::get()->m_game2LayerB0);
            this->m_portalBack = this->addInternalChild(parent, config->getDetailSprite(), CCPoint(0, 0), -90);
        } // if

        this->template setupObjectProperty<bool>(111, this->m_cameraIsFreeMode);
        this->template setupObjectProperty<bool>(112, this->m_cameraEditCameraSettings);
        this->template setupObjectProperty<float>(113, this->m_cameraEasingValue, [this](){return this->m_cameraEditCameraSettings;});
        this->template setupObjectProperty<float>(114, this->m_cameraPaddingValue, [this](){return this->m_cameraEditCameraSettings;});
        this->template setupObjectProperty<bool>(370, this->m_cameraDisableGridSnap);

        return true;
    } // init

    void switchPlayerMode(GJBaseGameLayer* level, PlayerObject* player, GameModeType type) {
        this->m_objectType = (GameObjectType)type;
        this->m_fakeType = (GameObjectType)type;

        level->playerWillSwitchMode(player, this);
        switch (type) {
            case GameModeType::SHIP: level->switchToFlyMode(player, this, false, 5); break;
            case GameModeType::BALL: level->switchToRollMode(player, this, false); break;
            case GameModeType::UFO: level->switchToFlyMode(player, this, false, 19); break;
            case GameModeType::WAVE: level->switchToFlyMode(player, this, false, 26); break;
            case GameModeType::ROBOT: level->switchToRobotMode(player, this, false); break;
            case GameModeType::SPIDER: level->switchToSpiderMode(player, this, false); break;
            case GameModeType::SWING: level->switchToFlyMode(player, this, false, 41); break;
            default: player->switchedToMode((GameObjectType)type); break; // cube
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

        CustomObjectBase::playShineEffect();
        this->m_objectID = id;
    } // playShineEffect

private:
    CCSprite* m_portalBack = nullptr;
    GameObjectType m_fakeType = GameObjectType::Modifier;

public:
    void triggerObject(GJBaseGameLayer* level, int playerID, gd::vector<int> const*) override final {
        auto player = (level->m_player2->m_uniqueID == playerID) ? level->m_player2 : level->m_player1;
        level->m_effectManager->removeTriggeredID(this->m_uniqueID, player->m_uniqueID);

        if (level->canBeActivatedByPlayer(player, this)) {
            player->m_lastActivatedPortal = this;
            player->m_lastPortalPos = this->getPosition();
            this->activatedByPlayer(player);
            this->activateCustomObject(level, player);
        } // if
    } // triggerObject

    void customSetup() override final {
        if (!this->m_editorEnabled && !this->m_hasNoParticles) {
            this->createAndAddParticle(6, "portalEffect02.plist", 4, tCCPositionType::kCCPositionTypeGrouped);
            this->claimParticle();
        } // if
        CustomObjectBase::customSetup();
        this->m_dontIgnoreDuration = false;
    } // customSetup

    void setPosition(cocos2d::CCPoint const& p0) override {
        CustomObjectBase::setPosition(p0);
        if (this->m_portalBack) this->m_portalBack->setPosition(p0);
    } // setPosition

    void setRotation(float p0) override {
        CustomObjectBase::setRotation(p0);
        if (this->m_portalBack) this->m_portalBack->setRotation(p0);
    } // setRotation

    void setScale(float p0) override {
        CustomObjectBase::setScale(p0);
        if (this->m_portalBack) this->m_portalBack->setScale(p0);
    } // setScale

    void setOpacity(unsigned char p0) override {
        CustomObjectBase::setOpacity(p0);
        if (this->m_portalBack) this->m_portalBack->setOpacity(p0);
    } // setOpacity

    void setVisible(bool p0) override {
        CustomObjectBase::setVisible(p0);
        if (this->m_portalBack) this->m_portalBack->setVisible(p0);
    } // setVisible

    bool hasBeenActivated() override {
        return CustomObjectBase::hasBeenActivated() && this->m_isNoMultiActivate;
    } // hasBeenActivated

    GameObjectType getType() const override {
        return this->m_fakeType;
    } // getType

    bool canAllowMultiActivate() override {
        return true;
    } // canAllowMultiActivate
};

class CUSTOM_OBJECTS_DLL $generic(CustomPortalObject);