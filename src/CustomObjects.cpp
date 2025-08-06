#include "CustomObjectsAPI.hpp"

#include "object/CustomGameObject.hpp"
#include "object/CustomEffectGameObject.hpp"
#include "object/CustomRingGameObject.hpp"

/*
    m_objectType == 0x388
    m_objectID == 0x3f4
    m_hasNoAudioScale == 0x3b9
    m_hasNoEffects = 0x404
*/

class FrownGameObject : public CustomGameObject {
public:
    static FrownGameObject* create(int id) {
        auto obj = new FrownGameObject();
        if (obj->init(id)) return obj;

        delete obj;
        return nullptr;
    } // create

    bool customInit() {
        m_objectType = GameObjectType::Hazard;
        m_width = 20;
        m_height = 20;
        return true;
    } // customInit
};

class ContainerGameObject : public CustomRingGameObject {
public:
    float m_bouncePower = 1;

    static ContainerGameObject* create(int id) {
        auto obj = new ContainerGameObject();
        if (obj->init(id)) return obj;

        delete obj;
        return nullptr;
    } // create

    void setupCustomRing(CCParticleSystemQuad* particle) override {
        if (!particle) return;

        particle->setStartColor(ccColor4F{ 0, 0, 0, 255 });
        particle->setEndColor(ccColor4F{ 0, 0, 0, 0 });
        particle->setBlendFunc(kCCBlendFuncDisable);
    } // setupCustomRing

    void pressCustomRing(PlayerObject* player) override {
        player->propellPlayer(m_bouncePower * 0.35, true, 12);
        player->animatePlatformerJump(1.0f);
        m_bouncePower += 0.1;
    } // onJump

    void resetCustomRing() override { m_bouncePower = 1; }
};

class SmileGameObject : public CustomEffectGameObject {
public:
    int m_bouncePower;

    static SmileGameObject* create(int id) {
        auto obj = new SmileGameObject();
        if (obj->init(id)) return obj;

        delete obj;
        return nullptr;
    } // create

    bool customInit() override {
        m_objectType = GameObjectType::Modifier;
        m_isTouchTriggered = true;
        m_isMultiTriggered = true;
        m_isTrigger = true;

        m_unk532 = true;

        return true;
    } // customInit

    void setToDefault() override {
        m_bouncePower = 8;
    } // setToDefault

    void onTrigger(GJBaseGameLayer* playLayer) override {
        playLayer->m_player1->setYVelocity(m_bouncePower, 1);
        m_bouncePower++;
    } // onTrigger
};

$execute {
    auto manager = CustomObjectsManager::get();
    auto mod = manager->registerCustomObjectsMod(Mod::get(), 4);

    mod->registerCustomObject("frown-block.png"_spr, FrownGameObject::create);
    mod->registerCustomObject("smile-block.png"_spr, SmileGameObject::create);
    mod->registerCustomObject("container.png"_spr, ContainerGameObject::create);

    mod->registerCustomObject("block-1.png"_spr);
    mod->registerCustomObject("block-2.png"_spr);
    mod->registerCustomObject("block-3.png"_spr);
    mod->registerCustomObject("block-4.png"_spr);
    mod->registerCustomObject("block-5.png"_spr);
    mod->registerCustomObject("block-6.png"_spr);
    mod->registerCustomObject("block-7.png"_spr, CCSize(2, 1));
    mod->registerCustomObject("block-8.png"_spr, CCSize(2, 1));
    mod->registerCustomObject("block-9.png"_spr, CCSize(2, 1));
    mod->registerCustomObject("block-10.png"_spr, CCSize(2, 2));
    mod->registerCustomObject("block-11.png"_spr, CCSize(2, 2));
    mod->registerCustomObject("block-12.png"_spr, CCSize(2, 2));
}