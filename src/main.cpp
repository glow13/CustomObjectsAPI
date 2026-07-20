#include <Geode/Geode.hpp>
#include "CustomObjectBase.hpp"
#include "CustomObjectConfig.hpp"
#include "manager/CustomObjectsManager.hpp"

using namespace geode::prelude;

#define $object(NAME, BASE) NAME : public CustomObjectBase<BASE>, ConfigObject<NAME, #NAME>

class $object(TestGameObject, GameObject) {};
class $object(TestEffectObject, EffectGameObject) {};

$on_game(ModsLoaded) {
    auto manager = CustomObjectsManager::get();
    log::info("registered {} custom objects", manager->getTotalCustomObjectsCount());

    manager->forEachCustomObject([manager](const auto config) {
        auto id = config->getObjectID();
        auto obj = manager->createCustomObjectWithID(id);
        log::info("{} {}", id, obj);
    });
};