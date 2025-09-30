#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>

#include "CustomObjectsManager.hpp"

using namespace geode::prelude;

class $modify(GameObject) {
public:
    static GameObject* createWithKey(int key) {
        if (key >= 10000) {
            auto manager = CustomObjectsManager::get();
            if (manager->containsCustomObject(key)) {
                return manager->getCustomObjectByID(key).create();
            }
        } // if
        return GameObject::createWithKey(key);
    } // createWithKey

    bool isSpawnableTrigger() {
        if (m_objectID >= 10000) return m_isTrigger;
        return GameObject::isSpawnableTrigger();
    } // isSpawnableTrigger
};