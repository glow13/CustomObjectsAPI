#include "CustomObjectBase.hpp"
#include "CustomObjectConfig.hpp"

using namespace geode::prelude;

#define $object(NAME, BASE) NAME : public CustomObjectBase<BASE>, public RegisterCustomObject<NAME, #NAME>

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