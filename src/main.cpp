#include "CustomObjectBase.hpp"
#include "CustomObjectConfig.hpp"
#include "manager/CustomObjectsManager.hpp"

using namespace geode::prelude;

#define $object(NAME, BASE) NAME : public CustomObjectBase<BASE>, public RegisterCustomObject<NAME, #NAME>

class $object(TestGameObject, GameObject) {};
class $object(TestEffectObject, EffectGameObject) {};

$on_game(ModsLoaded) {
    auto manager = CustomObjectsManager::get();
    log::info("registered {} custom objects", manager->getTotalCustomObjectsCount());

    manager->forEachCustomObject([manager](const CustomObjectConfig* config) {
        log::info("{} {} {}", config->getObjectID(), config->getID(), config->getModID());
        log::info("{} {}", config->getMainSprite(), config->getGlowSprite());
    });
};