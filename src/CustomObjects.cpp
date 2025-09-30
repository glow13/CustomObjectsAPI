#include "CustomObjectsAPI.hpp"

#include "object/CustomGameObject.hpp"
#include "object/CustomTriggerObject.hpp"
#include "object/CustomRingObject.hpp"
#include "object/CustomPadObject.hpp"
#include "object/CustomPortalObject.hpp"
#include "object/CustomRotateObject.hpp"
#include "object/CustomCollectibleObject.hpp"

/*
    m_objectType == 0x388
    m_objectID == 0x3f4
    m_hasNoAudioScale == 0x3b9
    m_hasNoEffects = 0x404
    m_classType = 0x4d0
    m_colorSprite = 0x368
    m_isTouchTriggered = 0x5b0
    m_isMultiTriggered = 0x670
*/

class $object(ContainerGameObject, CustomRingObject) {
public:
    float m_bouncePower = 1;

    void setupCustomObject() override {
        if (auto particle = createRingParticles()) {
            particle->setStartColor(ccColor4F{ 0, 0, 0, 255 });
            particle->setEndColor(ccColor4F{ 0, 0, 0, 0 });
            particle->setBlendFunc(kCCBlendFuncDisable);
        } // if
    } // setupCustomObject

    void resetCustomObject() override { m_bouncePower = 1; }

    void pressCustomRing(GJBaseGameLayer* level, PlayerObject* player) override {
        player->propellPlayer(m_bouncePower * 0.35, true, 12);
        player->animatePlatformerJump(1.0f);
        m_bouncePower += 0.1;
    } // pressCustomRing
};

class $object(SmileGameObject, CustomTriggerObject) {
public:
    int m_bouncePower;

    void setupCustomObject() override {
        m_isTouchTriggered = true;
        m_isMultiTriggered = true;
        m_duration = 0;
    } // setupCustomObject

    void resetCustomObject() override {
        m_bouncePower = getSavedValue<int>("bounce", 8);
    } // resetCustomObject

    void activateCustomTrigger(GJBaseGameLayer* level, PlayerObject* player) override {
        player->setYVelocity(m_bouncePower, 1);
        m_bouncePower++;
    } // activateCustomTrigger
};

class $object(TestPad, CustomPadObject) {
    void setupCustomObject() override {
        srand(time(0));
        if (auto particle = createPadParticles()) {
            particle->setStartColor(ccColor4F{ 255, 0, 255, 255 });
            particle->setEndColor(ccColor4F{ 255, 0, 255, 255 });
        } // if

        setGlowColor(ccColor3B{ 255, 0, 255 });
    } // setupCustomObject

    void touchCustomPad(GJBaseGameLayer* level, PlayerObject* player) override {
        bumpPlayer(player, 0.65f, GameObjectType::PinkJumpPad);
        if (rand() % 50 == 0) GJBaseGameLayer::get()->destroyPlayer(player, this);
    } // touchCustomPad
};

class $object(TestPortal, CustomPortalObject) {
    void setupCustomObject() override {
        srand(time(0));
        m_cameraIsFreeMode = true;

        if (auto particle = createPortalParticles()) {
            particle->setStartColor(ccColor4F{ 255, 255, 0, 255 });
            particle->setEndColor(ccColor4F{ 255, 255, 0, 255 });
        } // if
    } // setupCustomObject

    void touchCustomPortal(GJBaseGameLayer* level, PlayerObject* player) override {
        GameObjectType type;
        switch (rand() % 6) {
            case 0: type = GameObjectType::ShipPortal; break;
            case 1: type = GameObjectType::CubePortal; break;
            case 2: type = GameObjectType::BallPortal; break;
            case 3: type = GameObjectType::UfoPortal; break;
            case 4: type = GameObjectType::RobotPortal; break;
            case 5: type = GameObjectType::SpiderPortal; break;
            default: type = GameObjectType::CubePortal; break;
        } // switch

        switchPlayerMode(level, player, type);
        playShineEffect(type);
    } // touchCustomPortal
};

class $object(TestCollectible, CustomCollectibleObject) {
    void touchCustomCollectible(PlayerObject* player) {
        log::info("COLLECTED ME!!!!!!!");
    } // touchCustomCollectible
};

$execute {
    auto manager = CustomObjectsManager::get();
    auto mod = manager->registerCustomObjectsMod(Mod::get(), 4);

    mod->registerCustomObject("frown-block.png"_spr).setBoxSize(20, 20).setObjectType(GameObjectType::Hazard);
    mod->registerCustomObject("smile-block.png"_spr, SmileGameObject::create);
    mod->registerCustomObject("container.png"_spr, ContainerGameObject::create);

    mod->registerCustomObject("block-1.png"_spr);
    mod->registerCustomObject("block-2.png"_spr);
    mod->registerCustomObject("block-3.png"_spr);
    mod->registerCustomObject("block-4.png"_spr);
    mod->registerCustomObject("block-5.png"_spr);
    mod->registerCustomObject("block-6.png"_spr);
    mod->registerCustomObject("block-7.png"_spr, 60, 30);
    mod->registerCustomObject("block-8.png"_spr, 60, 30);
    mod->registerCustomObject("block-9.png"_spr, 60, 30);
    mod->registerCustomObject("block-10.png"_spr, 60);
    mod->registerCustomObject("block-11.png"_spr, 60);
    mod->registerCustomObject("block-12.png"_spr, 60);

    mod->registerCustomObject("spike_01_001.png").setGlowSprite("spike_01_glow_001.png").setBoxSize(5, 20).setObjectType(GameObjectType::Hazard);
    mod->registerCustomObject("block005_02_001.png", 60).setDetailSprite("block005_02_color_001.png").setObjectType(GameObjectType::Decoration);
    mod->registerCustomObject("player_134_001.png").setDetailSprite("player_134_2_001.png").setObjectType(GameObjectType::Decoration).setCustomRender();

    mod->registerCustomObject("bump_03_001.png", TestPad::create).setGlowSprite("bump_03_glow_001.png").setCustomRender(0).setCreateOffset(0, -13);
    mod->registerCustomObject("blade_02_001.png", CustomRotateObject::create).setGlowSprite("blade_02_glow_001.png").setBoxRadius(22).setObjectType(GameObjectType::Hazard).setCustomRender(0);

    mod->registerCustomObject("portal_18_front_001.png", TestPortal::create).setDetailSprite("portal_18_back_001.png").setCustomRender(1);
    mod->registerCustomObject("d_key01_001.png", TestCollectible::create).setDetailSprite("d_key01_color_001.png").setCustomRender(0);
}