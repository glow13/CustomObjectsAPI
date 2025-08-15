#include "CustomObjectsAPI.hpp"

#include "object/CustomGameObject.hpp"
#include "object/CustomTriggerObject.hpp"
#include "object/CustomRingObject.hpp"

/*
    m_objectType == 0x388
    m_objectID == 0x3f4
    m_hasNoAudioScale == 0x3b9
    m_hasNoEffects = 0x404
*/

class ContainerGameObject : public CustomRingObject<ContainerGameObject> {
public:
    float m_bouncePower = 1;

    void setupCustomRing() override {
        if (auto particle = createRingParticles()) {
            particle->setStartColor(ccColor4F{ 0, 0, 0, 255 });
            particle->setEndColor(ccColor4F{ 0, 0, 0, 0 });
            particle->setBlendFunc(kCCBlendFuncDisable);
        } // if
    } // setupCustomRing

    void pressCustomRing(PlayerObject* player) override {
        player->propellPlayer(m_bouncePower * 0.35, true, 12);
        player->animatePlatformerJump(1.0f);
        m_bouncePower += 0.1;
    } // onJump

    void resetCustomRing() override { m_bouncePower = 1; }
};

class SmileGameObject : public CustomTriggerObject<SmileGameObject> {
public:
    int m_bouncePower;

    void setupCustomTrigger() override {
        m_isTouchTriggered = true;
        m_isMultiTriggered = true;
    } // setupCustomTrigger

    void resetCustomTrigger() override {
        m_bouncePower = 8;
    } // resetCustomTrigger

    void activateCustomTrigger(GJBaseGameLayer* playLayer) override {
        playLayer->m_player1->setYVelocity(m_bouncePower, 1);
        m_bouncePower++;
    } // activateCustomTrigger
};

$execute {
    auto manager = CustomObjectsManager::get();
    auto mod = manager->registerCustomObjectsMod(Mod::get(), 4);

    mod->registerCustomObject(CustomObject("frown-block.png"_spr).setBoxSize(20, 20).setObjectType(GameObjectType::Hazard));
    mod->registerCustomObject("smile-block.png"_spr, SmileGameObject::create);
    mod->registerCustomObject("container.png"_spr, ContainerGameObject::create);

    mod->registerCustomObject("block-1.png"_spr);
    mod->registerCustomObject("block-2.png"_spr);
    mod->registerCustomObject("block-3.png"_spr);
    mod->registerCustomObject("block-4.png"_spr);
    mod->registerCustomObject("block-5.png"_spr);
    mod->registerCustomObject("block-6.png"_spr);
    mod->registerCustomObject("block-7.png"_spr, CCSize(60, 30));
    mod->registerCustomObject("block-8.png"_spr, CCSize(60, 30));
    mod->registerCustomObject("block-9.png"_spr, CCSize(60, 30));
    mod->registerCustomObject("block-10.png"_spr, CCSize(60, 60));
    mod->registerCustomObject("block-11.png"_spr, CCSize(60, 60));
    mod->registerCustomObject("block-12.png"_spr, CCSize(60, 60));

    mod->registerCustomObject(CustomObject("spike_01_001.png").setBoxSize(5, 20).setObjectType(GameObjectType::Hazard));
}