#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>

#include "CustomObjectsManager.hpp"
#include "data/CustomObjectConfig.hpp"

using namespace geode::prelude;

class $modify(GameObject) {
public:
    static GameObject* createWithKey(int key) {
        if (key >= 10000) {
            auto manager = CustomObjectsManager::get();
            auto obj = manager->getCustomObjectByID(key);
            if (obj) return obj->create();
        } // if
        return GameObject::createWithKey(key);
    } // createWithKey

    bool isTrigger() {
        if (m_objectID >= 10000) return m_isTrigger;
        return GameObject::isTrigger();
    } // isTrigger

    bool isSpawnableTrigger() {
        if (m_objectID >= 10000) return m_isTrigger;
        return GameObject::isSpawnableTrigger();
    } // isSpawnableTrigger
};