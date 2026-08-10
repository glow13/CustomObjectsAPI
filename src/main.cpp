#include "CustomObjectsAPI.hpp"

using namespace geode::prelude;

class $object(TestGameObject, GameObject) {
public:
    static void onRegisterConfig(CustomObjectConfig&& config) {
        config.setMainSprite("square_01_001.png");
        config.setGlowSprite("square_01_glow_001.png");
    }
};

class $object(SpikeObject, GameObject) {
public:
    static void onRegisterConfig(CustomObjectConfig&& config) {
        config.setMainSprite("spike_01_001.png");
        config.setGlowSprite("spike_01_glow_001.png");
        config.setEditorPriority(-12).setBoxSize(6, 20).setObjectType(GameObjectType::Hazard);
    }
};

$execute {
    CustomObjectsAPI::registerCustomObject("weird"_spr)
        .setMainSprite("block001_01_001.png")
        .setObjectType(GameObjectType::Decoration);
}