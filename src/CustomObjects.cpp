#include "CustomObjectsAPI.hpp"

#include "object/CustomGameObject.hpp"
#include "object/CustomTriggerObject.hpp"
#include "object/CustomRingObject.hpp"
#include "object/CustomPadObject.hpp"
#include "object/CustomRotateObject.hpp"

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

    void pressCustomRing(PlayerObject* player) override {
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

    void activateCustomTrigger(GJBaseGameLayer* playLayer) override {
        playLayer->m_player1->setYVelocity(m_bouncePower, 1);
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

        if (!PlayLayer::get() || m_hasNoGlow) return;
        createGlow("bump_03_glow_001.png");
        setGlowColor(ccColor3B{ 255, 0, 255 });
    } // setupCustomObject

    void touchCustomPad(PlayerObject* player) override {
        bumpPlayer(player, 0.65f, GameObjectType::PinkJumpPad);
        if (rand() % 50 == 0) GJBaseGameLayer::get()->destroyPlayer(player, this);
    } // touchCustomPad
};

class $object(SawbladeObject, CustomRotateObject) {
    void setupCustomObject() override {
        setDontDraw(true);
        addCustomChild("blade_02_001.png", CCPoint(0, 0), 0);

        if (!PlayLayer::get() || m_hasNoGlow) return;

        createGlow("blade_02_glow_001.png");
        auto glow = addInternalGlowChild("blade_02_glow_001.png", CCPoint(0, 0));
        m_glowSprite->setDontDraw(true);
    } // setupCustomObject
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
    mod->registerCustomObject("block-7.png"_spr).setSize(60, 30);
    mod->registerCustomObject("block-8.png"_spr).setSize(60, 30);
    mod->registerCustomObject("block-9.png"_spr).setSize(60, 30);
    mod->registerCustomObject("block-10.png"_spr).setSize(60, 60);
    mod->registerCustomObject("block-11.png"_spr).setSize(60, 60);
    mod->registerCustomObject("block-12.png"_spr).setSize(60, 60);

    mod->registerCustomObject("spike_01_001.png").setBoxSize(5, 20).setObjectType(GameObjectType::Hazard);
    mod->registerCustomObject("block005_02_001.png", "block005_02_color_001.png").setSize(60, 60).setObjectType(GameObjectType::Decoration);
    mod->registerCustomObject("player_134_001.png", "player_134_2_001.png").setObjectType(GameObjectType::Decoration).useCustomRender();

    mod->registerCustomObject("bump_03_001.png", TestPad::create).setSize(25, 5).useCustomRender(0);
    mod->registerCustomObject("blade_02_001.png", SawbladeObject::create).setBoxRadius(22).setObjectType(GameObjectType::Hazard).useCustomRender(0);
}