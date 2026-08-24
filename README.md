# <img src="resources/tab-icon.png" height="40"> Custom Objects API

> #### UPDATE 8/24/2026:
> TODO

***

### ***Add new custom objects to Geometry Dash!***

This mod aims to be the universal solution for any developers wanting to add their own custom objects to the game, with a focus on compatability and ease of use. The API features a class-based approach to object creation, where instead of hooking vanilla objects to do what you want, you create completely new classes that inherit from GameObject and that you have complete control over. The goal is to give developers a much higher level of freedom in what they can implement, while also eliminating the problem of having too many hooks that may conflict with other custom objects from other mods.

This API comes with several pre-made object classes for you to use and inherit from (made by me)! You have several options on how to register a custom object, including an easy-ish-to-use config object that you can configure inline.

***

#### Here is an example of one of the simplest ways to register an object:

```cpp
#include <glow12.custom-objects-api/include/CustomObjectsAPI.hpp>

// You need to register any custom objects when your mod loads
$execute {
    // Object registration using the config
    // Provide the object ID using ""_spr and change any options inline
    CustomObjectsAPI::registerCustomObject("my-custom-block"_spr)
        .setMainSprite("custom-sprite.png"_spr)
        .setObjectType(GameObjectType::Solid)
        .setBoxSize(20, 20);
}
```

#### Advanced object registration using a more complex object class:

```cpp
#include <glow12.custom-objects-api/include/CustomObjectsAPI.hpp>
#include <glow12.custom-objects-api/include/object/CustomRingObject.hpp>

$execute {
    // Advanced custom object registration using the CustomRingObject class
    auto customOrb = CustomObjectsAPI::registerCustomObject<CustomRingObject>("my-custom-orb"_spr)
        .setMainSprite("custom-orb.png"_spr, 36, 36)
        .setGlowSprite("custom-orb-glow.png"_spr)
        .setGlowColor(255, 255, 0)
        .setParticleColor(200, 150, 0);

    // You can also assign the config to a variable reference and use it elsewhere
    customOrb.onSetupCustomObject([](CustomRingObject* obj) { // callback for object setup
        log::info("yo this object just got set up ig {}", obj->m_objectID);
    });

    // onActivateCustomObject callback is called when the player clicks the orb
    customOrb.onActivateCustomObject([](CustomRingObject* obj, auto level, auto player) {
        if (rand() % 100 == 0) level->destroyPlayer(player, obj); // :trollface:
    });
}
```

#### Creating your own class and inheriting from a built-in class:

```cpp
#include <glow12.custom-objects-api/include/CustomObjectsAPI.hpp>
#include <glow12.custom-objects-api/include/object/CustomPadObject.hpp>

// Convenient macro provided by the API, similar to Geode's $modify macro
// This automatically registers your object with its own config
class $registerObject(MyCustomPad, CustomPadObject) {
public:

    // Because this is your own class, you can just add a class variable normally
    // You don't have to use Geode Fields or anything more complex
    float bouncePower;

    // This function is automatically called when your mod loads
    static void onRegisterConfig(CustomObjectConfig&& config) {
        config.setMainSprite("custom-pad.png"_spr)
        config.setObjectOffset(0, -13);
    }

    // Most of the config callbacks work here too
    // Simply override the class's virtual functions to use them
    void resetCustomObject() override { // called at the start of every new attempt
        bouncePower = 0.4f;
    }

    // This function is called when the player touches the pad
    void activateCustomObject(GJBaseGameLayer* level, PlayerObject* player) override {
        // bumpPlayer function provided by the CustomPadObject class
        bumpPlayer(player, bouncePower, GameObjectType::PinkJumpPad);
        bouncePower += 0.1f;
    }

    // Because CustomPadObject fundamentally inherits from EffectGameObject
    // You can also override any EffectGameObject or GameObject virtual functions
    void selectObject(ccColor3B color) override {
        CustomPadObject::selectObject(color);
        log::info("someone just selected me ig!");
    }
};
```

***

# Libraries Used

- [EditorTab API](https://github.com/Alphalaneous/EditorTab-API) by [Alphalaneous](https://github.com/Alphalaneous) for the custom editor tab
- [rectpack2D](https://github.com/TeamHypersomnia/rectpack2D) by [TeamHypersomnia](https://github.com/TeamHypersomnia) for sprite sheet generation
- [lodepng](https://github.com/lvandeve/lodepng) by [lvandeve](https://github.com/lvandeve) for saving the sprite sheet as a PNG image