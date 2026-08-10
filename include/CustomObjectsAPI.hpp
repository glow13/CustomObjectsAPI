#pragma once

#include "CustomObjectConfig.hpp"
#include "CustomObjectBase.hpp"

class CustomObjectsUtils final {
    static CustomObjectConfig&& registerCustomObject(std::string, CustomObjectConfig::ObjectConstructor, geode::Mod* = geode::Mod::get());
    static void registerCustomSprite(std::string, int, int, int, int, geode::Mod* = geode::Mod::get());
    static void registerCustomAnimationSprites(std::string, int, int, int, int, int, geode::Mod* = geode::Mod::get());
    friend class CustomObjectsAPI;
};

class CustomObjectsAPI final {
public:

    /**
     * Registers a new custom object.
     * Automatically registers a new sprite with a custom size and a custom offset.
     *
     * @tparam ObjectType The object class that this object should use. Defaults to CustomGameObject.
     * @param objectID The string used to identify this custom object, should use "_spr" to include your mod's id.
     * @return The CustomObjectConfig for the new custom object.
     */
    template <class ObjectType>// = CustomGameObject>
    requires (!std::derived_from<ObjectType, RegisteredObjectByClass>)
    static CustomObjectConfig&& registerCustomObject(std::string objectID) {
        return CustomObjectsUtils::registerCustomObject(objectID, ObjectType::template createWithConfig<ObjectType>);
    }

    /**
     * Registers a sprite to be added to the custom object spritesheet.
     * 
     * @param frameName The name of the sprite to register.
     * @param offsetX The custom horizontal offset of the sprite.
     * @param offsetY The custom vertical offset of the sprite.
     * @param width The custom width of the sprite.
     * @param height The custom height of the sprite.
     */
    static void registerCustomSprite(std::string frameName, int offsetX, int offsetY, int width, int height) {
        CustomObjectsUtils::registerCustomSprite(frameName, offsetX, offsetY, width, height);
    }

    /**
     * Registers a sprite to be added to the custom object spritesheet.
     * The sprite is registered with no custom offset.
     * 
     * @param frameName The name of the sprite to register.
     * @param width The custom width of the sprite.
     * @param height The custom height of the sprite.
     */
    static void registerCustomSprite(std::string frameName, int width, int height) {
        CustomObjectsUtils::registerCustomSprite(frameName, 0, 0, width, height);
    }

    /**
     * Registers a sprite to be added to the custom object spritesheet.
     * The sprite is registered with no custom offset.
     * 
     * @param frameName The name of the sprite to register.
     * @param size The custom size of the sprite. This value is used for both the width and height.
     */
    static void registerCustomSprite(std::string frameName, int size) {
        CustomObjectsUtils::registerCustomSprite(frameName, 0, 0, size, size);
    }

    /**
     * Registers a sprite to be added to the custom object spritesheet.
     * The sprite is registered with no custom offset.
     * The size in the spritesheet is not changed from the sprite's actual size.
     * 
     * @param frameName The name of the sprite to register.
     * @return The CustomSpriteConfig for the sprite.
     */
    static void registerCustomSprite(std::string frameName) {
        CustomObjectsUtils::registerCustomSprite(frameName, 0, 0, 0, 0);
    }

    /**
     * An easy way to register multiple sprites used in an animation, intended to be used for the `CustomAnimatedObject` class.
     * The sprites for the animation must all have the correct naming scheme for Geometry Dash to handle them correctly: 
     * they must all start with the same name, and have the frame number appended to the end, for example "cat_001.png", "cat_002.png", etc.
     * Pass the name of the first sprite of the animation into this function, ie "cat_001.png", and it will automatically register the rest.
     * 
     * @param firstFrame The name of the first sprite in the animation.
     * @param offsetX The custom horizontal offset of the animation.
     * @param offsetY The custom vertical offset of the animation.
     * @param width The custom width of each frame of the animation.
     * @param height The custom height of each frame of the animation.
     * @param frames The total number of frames in the animation.
     */
    static void registerCustomAnimationSprites(std::string firstFrame, int offsetX, int offsetY, int width, int height, int frames) {
        CustomObjectsUtils::registerCustomAnimationSprites(firstFrame, offsetX, offsetY, width, height, frames);
    }

    /**
     * An easy way to register multiple sprites used in an animation, intended to be used for the `CustomAnimatedObject` class.
     * The sprites for the animation must all have the correct naming scheme for Geometry Dash to handle them correctly: 
     * they must all start with the same name, and have the frame number appended to the end, for example "cat_001.png", "cat_002.png", etc.
     * Pass the name of the first sprite of the animation into this function, ie "cat_001.png", and it will automatically register the rest.
     * 
     * This function applies no custom offset to the animation.
     * 
     * @param firstFrame The name of the first sprite in the animation.
     * @param width The custom width of each frame of the animation.
     * @param height The custom height of each frame of the animation.
     * @param frames The total number of frames in the animation.
     */
    static void registerCustomAnimationSprites(std::string firstFrame, int width, int height, int frames) {
        CustomObjectsUtils::registerCustomAnimationSprites(firstFrame, 0, 0, width, height, frames);
    }

    /**
     * An easy way to register multiple sprites used in an animation, intended to be used for the `CustomAnimatedObject` class.
     * The sprites for the animation must all have the correct naming scheme for Geometry Dash to handle them correctly: 
     * they must all start with the same name, and have the frame number appended to the end, for example "cat_001.png", "cat_002.png", etc.
     * Pass the name of the first sprite of the animation into this function, ie "cat_001.png", and it will automatically register the rest.
     * 
     * This function applies no custom offset to the animation.
     * 
     * @param firstFrame The name of the first sprite in the animation.
     * @param size The custom size of each frame of the animation. This value is used for both the width and the height.
     * @param frames The total number of frames in the animation.
     */
    static void registerCustomAnimationSprites(std::string firstFrame, int size, int frames) {
        CustomObjectsUtils::registerCustomAnimationSprites(firstFrame, 0, 0, size, size, frames);
    }

    /**
     * An easy way to register multiple sprites used in an animation, intended to be used for the `CustomAnimatedObject` class.
     * The sprites for the animation must all have the correct naming scheme for Geometry Dash to handle them correctly: 
     * They must all start with the same name, and have the frame number appended to the end, for example "cat_001.png", "cat_002.png", etc.
     * Pass the name of the first sprite of the animation into this function, ie "cat_001.png", and it will automatically register the rest.
     * 
     * This function applies no custom offset to the animation, and each frame is registered using the actual size of the sprite.
     * 
     * @param firstFrame The name of the first sprite in the animation.
     * @param frames The total number of frames in the animation.
     */
    static void registerCustomAnimationSprites(std::string firstFrame, int frames) {
        CustomObjectsUtils::registerCustomAnimationSprites(firstFrame, 0, 0, 0, 0, frames);
    }
};