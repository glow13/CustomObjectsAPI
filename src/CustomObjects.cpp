#include "CustomObjectsAPI.hpp"

#include "object/CustomGameObject.hpp"
#include "object/CustomEffectGameObject.hpp"

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

class ContainerGameObject : public CustomGameObject {
public:
    static ContainerGameObject* create(int id) {
        auto obj = new ContainerGameObject();
        if (obj->init(id)) return obj;

        delete obj;
        return nullptr;
    } // create

    bool customInit() {
        m_objectType = GameObjectType::Solid;
        return true;
    } // customInit
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

        m_uniqueID = 22;
        m_unk390 = 20;
        m_unk40C = true;
        m_unk532 = true;

        // addCustomColorChild("custom-objects/glow12.custom-objects-api/30/30/smile-block.png");

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
    manager->setModCustomObjectGenerationValue(4);

    manager->registerCustomObject("frown-block.png"_spr, FrownGameObject::create);
    manager->registerCustomObject("smile-block.png"_spr, SmileGameObject::create);
    manager->registerCustomObject("container.png"_spr, ContainerGameObject::create);

    manager->registerCustomObject("block-1.png"_spr);
    manager->registerCustomObject("block-2.png"_spr);
    manager->registerCustomObject("block-3.png"_spr);
    manager->registerCustomObject("block-4.png"_spr);
    manager->registerCustomObject("block-5.png"_spr);
    manager->registerCustomObject("block-6.png"_spr);
    manager->registerCustomObject("block-7.png"_spr, CCSize(2, 1));
    manager->registerCustomObject("block-8.png"_spr, CCSize(2, 1));
    manager->registerCustomObject("block-9.png"_spr, CCSize(2, 1));
    manager->registerCustomObject("block-10.png"_spr, CCSize(2, 2));
    manager->registerCustomObject("block-11.png"_spr, CCSize(2, 2));
    manager->registerCustomObject("block-12.png"_spr, CCSize(2, 2));
}