#include "CustomObjectBase.hpp"
#include "CustomObjectConfig.hpp"
#include "manager/CustomObjectsManager.hpp"

using namespace geode::prelude;

#define $object(NAME, BASE) NAME : public CustomObjectBase<BASE>, public RegisterCustomObject<NAME, #NAME>

class $object(TestGameObject, GameObject) {
public:
    static void onRegisterConfig(CustomObjectConfig&& config) {
        config.setMainSprite("square_01_001.png", false);
        config.setGlowSprite("square_01_glow_001.png", false);
    }
};

class $object(SpikeObject, GameObject) {
public:
    static void onRegisterConfig(CustomObjectConfig&& config) {
        config.setMainSprite("spike_01_001.png", false);
        config.setGlowSprite("spike_01_glow_001.png", false);
        config.setEditorPriority(-12).setBoxSize(6, 20).setObjectType(GameObjectType::Hazard);
    }
};

$on_game(Loaded) {
    log::info("{}", CustomObjectsManager::get()->getCustomSpritesheet());
}