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

    bool customInit() {
        m_objectType = GameObjectType::Modifier;
        m_isTouchTriggered = true;
        m_isMultiTriggered = true;
        m_isTrigger = true;
        return true;
    } // customInit

    void setToDefault() {
        m_bouncePower = 8;
    } // setToDefault

    void onTrigger(GJBaseGameLayer* playLayer) {
        playLayer->m_player1->setYVelocity(m_bouncePower, 1);
        m_bouncePower++;
    } // onTrigger
};

$execute {
    auto manager = CustomObjectsManager::get();

    manager->registerCustomObject("frown-block.png"_spr, CCSize(30, 30), FrownGameObject::create);
    manager->registerCustomObject("smile-block.png"_spr, CCSize(30, 30), SmileGameObject::create);
    manager->registerCustomObject("container.png"_spr, CCSize(30, 30), ContainerGameObject::create);

    manager->registerCustomObject("block-1.png"_spr, CCSize(30, 30), CustomGameObject::create);
    manager->registerCustomObject("block-2.png"_spr, CCSize(30, 30), CustomGameObject::create);
    manager->registerCustomObject("block-3.png"_spr, CCSize(30, 30), CustomGameObject::create);
    manager->registerCustomObject("block-4.png"_spr, CCSize(30, 30), CustomGameObject::create);
    manager->registerCustomObject("block-5.png"_spr, CCSize(30, 30), CustomGameObject::create);
    manager->registerCustomObject("block-6.png"_spr, CCSize(30, 30), CustomGameObject::create);
    manager->registerCustomObject("block-7.png"_spr, CCSize(60, 30), CustomGameObject::create);
    manager->registerCustomObject("block-8.png"_spr, CCSize(60, 30), CustomGameObject::create);
    manager->registerCustomObject("block-9.png"_spr, CCSize(60, 30), CustomGameObject::create);
    manager->registerCustomObject("block-10.png"_spr, CCSize(60, 60), CustomGameObject::create);
    manager->registerCustomObject("block-11.png"_spr, CCSize(60, 60), CustomGameObject::create);
    manager->registerCustomObject("block-12.png"_spr, CCSize(60, 60), CustomGameObject::create);
}