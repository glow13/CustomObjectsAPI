# Custom Objects API

> #### UPDATE 6/28/2026:
> So apparently this mod idea is \**so good*\* that someone on the Geode staff is already developing a "custom objects API" that I didn't know about when I was developing this mod. I was told that their version of the API will be officially endorsed by the Geode team when it eventually releases, and therefore my mod will never be accepted to the official index. This means that this API is effectively worthless and the many hours of work I put into this project were completely wasted.  
> #### FUTURE PLANS:
> While the official API was the main reason my mod will never be accepted, I also recieved some feedback and was shown that there were several critical issues with my code that I didn't know about. I am currently planning to spend some time making a *full rewrite* of this API, reusing a lot of the code but fixing many of the design flaws in my original mod. I want to create a good and usable version of this mod that is consistent with my original vision for a custom obects API, even if it will never be officially accepted or used by anyone. And who knows, maybe if I don't prefer the official API I can revive this in some capacity for my own personal mods, I'll see whenever that happens I guess.  
> 
> Thanks for reading if you actually read all of that lol, please look forward to the official version of the API whenever it releases, and hopefully I can come back soon with another cool mod idea :D

***

### ***Add new custom objects to Geometry Dash!***

This mod aims to be the universal solution for any developers wanting to add their own custom objects to the game, with a focus on compatability and ease of use. The API features a class-based approach to object creation, where instead of hooking vanilla objects to do what you want, you create completely new classes that inherit from GameObject and that you have complete control over. The goal is to give developers a much higher level of freedom in what they can implement, while also eliminating the problem of having too many hooks that may conflict with other custom objects from other mods.

This API comes with several pre-made object classes for you to use and inherit from (made by me)! You have several options on how to register a custom object, including an easy-ish-to-use config object that you can configure inline.

For more in-depth deocumentation, there are several comments in the API headers that attempt to explain what each class does. Also please check out the GitHub [wiki page](https://github.com/glow13/CustomObjectsAPI/wiki)! It's still incomplete and probably doesn't cover everything, but hopefully it makes this API a little easier to use :)

***

#### Here is an example of one of the simplest ways to register an object:

```cpp
#include <glow12.custom-objects-api/include/CustomObjectsAPI.hpp>

// You need to register any custom objects when your mod loads
$execute {
    // Single line object registration
    // Provide the main sprite and change config options inline
    CustomObjectsAPI::registerCustomObject("my-custom-block.png"_spr)
        .setBoxSize(20, 20).setObjectType(GameObjectType::Solid);
}
```

#### Advanced object registration using a more complex object class:

```cpp
#include <glow12.custom-objects-api/include/CustomObjectsAPI.hpp>
#include <glow12.custom-objects-api/include/object/CustomRingObject.hpp>

$execute {
    // Advanced custom object registration using the CustomRingObject class
    auto customOrb = CustomObjectsAPI::registerCustomObject<CustomRingObject>("my-custom-orb.png"_spr, 36, 36)
        .setGlowSprite("custom-orb-glow.png"_spr).setGlowColor(255, 255, 0).setParticleColor(200, 150, 0);

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
class $object(MyCustomPad, CustomPadObject) {
    // Because this is your own class, you can just add a class variable normally
    // You don't have to use Geode Fields or anything more complex
    float bouncePower;

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

// You still have to register the object
$execute {
    // Simply provide your custom class as the template parameter
    CustomObjectsAPI::registerCustomObject<MyCustomPad>("custom-pad.png"_spr).setObjectOffset(0, -13);
}
```

***

# Libraries Used

- [rectpack2D](https://github.com/TeamHypersomnia/rectpack2D) by [TeamHypersomnia](https://github.com/TeamHypersomnia) for sprite sheet generation
- [lodepng](https://github.com/lvandeve/lodepng) by [lvandeve](https://github.com/lvandeve) for saving the sprite sheet as a PNG image
