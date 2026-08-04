#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>

#include "../manager/CustomObjectsManager.hpp"
#include "CustomObjectConfig.hpp"

using namespace geode::prelude;

class $modify(GameObject) {
public:
    static GameObject* createWithKey(int key) {
        if (key >= BASE_OBJECT_ID) if (auto obj = CustomObjectsManager::get()->createCustomObjectWithID(key)) return obj;
        return GameObject::createWithKey(key);
    }
};