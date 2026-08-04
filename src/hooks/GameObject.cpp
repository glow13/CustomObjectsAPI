#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>

#include "../manager/CustomObjectsManager.hpp"
#include "CustomObjectConfig.hpp"

using namespace geode::prelude;

constexpr int BASE_OBJECT_ID = 10000;

class $modify(GameObject) {
public:
    static GameObject* createWithKey(int key) {
        if (key >= BASE_OBJECT_ID) if (auto obj = CustomObjectsManager::get()->createCustomObjectWithID(key)) return obj;
        return GameObject::createWithKey(key);
    }
};