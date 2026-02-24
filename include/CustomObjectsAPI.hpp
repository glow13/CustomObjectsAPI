#pragma once

#include "data/CustomSpriteConfig.hpp"
#include "data/CustomObjectConfig.hpp"
#include "data/CustomObjectsMod.hpp"

#include "CustomObjectBase.hpp"
#include "object/CustomGameObject.hpp"

class CUSTOM_OBJECTS_DLL CustomObjectsUtils final {
private:
    static geode::Mod* currentGeodeMod;
    static CustomObjectsMod* currentMod;
    static int currentOffset;

    static CustomObjectsMod* getMod(geode::Mod* mod);
    static void setCollisionOffset(geode::Mod* mod, uint8_t offset);

    friend class CustomObjectsAPI;
};

class CustomObjectsAPI final {
public:

    /**
     * Registers a new custom object.
     * Automatically registers a new sprite with a custom size and a custom offset.
     *
     * @tparam ObjectType The object class that this object should use. Defaults to CustomGameObject.
     * @param spr The name of the sprite to be used as the main sprite of this object.
     * @param sprOffsetX The custom horizontal offset of the main sprite.
     * @param sprOffsetY The custom vertical offset of the main sprite.
     * @param sprWidth The custom width of the main sprite.
     * @param sprHeight The custom height of the main sprite.
     * @return The CustomObjectConfig for the new custom object.
     */
    template <class ObjectType = CustomGameObject>
    static CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight) {
        return CustomObjectsUtils::getMod(geode::Mod::get())->registerCustomObject<ObjectType>(spr, sprOffsetX, sprOffsetY, sprWidth, sprHeight);
    }

    /**
     * Registers a new custom object.
     * Automatically registers a new sprite with a custom size and no custom offset.
     *
     * @tparam ObjectType The object class that this object should use. Defaults to CustomGameObject.
     * @param spr The name of the sprite to be used as the main sprite of this object.
     * @param sprWidth The custom width of the main sprite.
     * @param sprHeight The custom height of the main sprite.
     * @return The CustomObjectConfig for the new custom object.
     */
    template <class ObjectType = CustomGameObject>
    static CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr, int sprWidth, int sprHeight) {
        return registerCustomObject<ObjectType>(spr, 0, 0, sprWidth, sprHeight);
    }

    /**
     * Registers a new custom object.
     * Automatically registers a new sprite with a custom size and no custom offset.
     *
     * @tparam ObjectType The object class that this object should use. Defaults to CustomGameObject.
     * @param spr The name of the sprite to be used as the main sprite of this object.
     * @param sprSize The custom size of the main sprite. This value is used as both the width and height.
     * @return The CustomObjectConfig for the new custom object.
     */
    template <class ObjectType = CustomGameObject>
    static CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr, int sprSize) {
        return registerCustomObject<ObjectType>(spr, 0, 0, sprSize, sprSize);
    }

    /**
     * Registers a new custom object.
     * Automatically registers a new sprite with its actual size and no custom offset.
     *
     * @tparam ObjectType The object class that this object should use. Defaults to CustomGameObject.
     * @param spr The name of the sprite to be used as the main sprite of this object.
     * @return The CustomObjectConfig for the new custom object.
     */
    template <class ObjectType = CustomGameObject>
    static CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr) {
        return registerCustomObject<ObjectType>(spr, 0, 0, 0, 0);
    }

    /**
     * Registers a new custom object.
     * This function does NOT automatically add a sprite to this object!
     * If you register a custom object this way, make sure to handle the sprite creation yourself or your object will not be visible!
     *
     * @tparam ObjectType The object class that this object should use. Defaults to CustomGameObject.
     * @return The CustomObjectConfig for the new custom object.
     */
    template <class ObjectType = CustomGameObject>
    static CustomObjectConfig<ObjectType>& registerCustomObject() {
        return registerCustomObject<ObjectType>("", 0, 0, 0, 0);
    }

    /**
     * Registers a sprite to be added to the custom object spritesheet.
     * 
     * @param spr The name of the sprite to register.
     * @param sprOffsetX The custom horizontal offset of the sprite.
     * @param sprOffsetY The custom vertical offset of the sprite.
     * @param sprWidth The custom width of the sprite.
     * @param sprHeight The custom height of the sprite.
     * @return The CustomSpriteConfig for the sprite.
     */
    static CustomSpriteConfig& registerCustomSprite(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight) {
        return CustomObjectsUtils::getMod(geode::Mod::get())->registerCustomSprite(spr, sprOffsetX, sprOffsetY, sprWidth, sprHeight);
    }

    /**
     * Registers a sprite to be added to the custom object spritesheet.
     * The sprite is registered with no custom offset.
     * 
     * @param spr The name of the sprite to register.
     * @param sprWidth The custom width of the sprite.
     * @param sprHeight The custom height of the sprite.
     * @return The CustomSpriteConfig for the sprite.
     */
    static CustomSpriteConfig& registerCustomSprite(std::string spr, int sprWidth, int sprHeight) {
        return registerCustomSprite(spr, 0, 0, sprWidth, sprHeight);
    }

    /**
     * Registers a sprite to be added to the custom object spritesheet.
     * The sprite is registered with no custom offset.
     * 
     * @param spr The name of the sprite to register.
     * @param sprSize The custom size of the sprite. This value is used for both the width and height.
     * @return The CustomSpriteConfig for the sprite.
     */
    static CustomSpriteConfig& registerCustomSprite(std::string spr, int sprSize) {
        return registerCustomSprite(spr, 0, 0, sprSize, sprSize);
    }

    /**
     * Registers a sprite to be added to the custom object spritesheet.
     * The sprite is registered with no custom offset.
     * The size in the spritesheet not changed from the sprite's actual size.
     * 
     * @param spr The name of the sprite to register.
     * @return The CustomSpriteConfig for the sprite.
     */
    static CustomSpriteConfig& registerCustomSprite(std::string spr) {
        return registerCustomSprite(spr, 0, 0, 0, 0);
    }

    /**
     * An easy way to register multiple sprites used in an animation, intended to be used for a custom animated object.
     * The sprites for the animation must all have the correct naming scheme for Geometry Dash to handle them correctly: 
     * they must all start with the same name, and have the frame number appended to the end, for example "cat_001.png", "cat_002.png", etc.
     * Pass the name of the first sprite of the animation into this function, ie "cat_001.png", and it will automatically register the rest.
     * 
     * @param spr The name of the first sprite in the animation.
     * @param sprOffsetX The custom horizontal offset of the animation.
     * @param sprOffsetY The custom vertical offset of the animation.
     * @param sprWidth The custom width of each frame of the animation.
     * @param sprHeight The custom height of each frame of the animation.
     * @param frames The total number of frames in the animation.
     */
    static void registerCustomAnimationSprites(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight, int frames) {
        return CustomObjectsUtils::getMod(geode::Mod::get())->registerCustomAnimationSprites(spr, sprOffsetX, sprOffsetY, sprWidth, sprHeight, frames);
    }

    /**
     * An easy way to register multiple sprites used in an animation, intended to be used for a custom animated object.
     * The sprites for the animation must all have the correct naming scheme for Geometry Dash to handle them correctly: 
     * they must all start with the same name, and have the frame number appended to the end, for example "cat_001.png", "cat_002.png", etc.
     * Pass the name of the first sprite of the animation into this function, ie "cat_001.png", and it will automatically register the rest.
     * 
     * This function applies no custom offset to the animation.
     * 
     * @param spr The name of the first sprite in the animation.
     * @param sprWidth The custom width of each frame of the animation.
     * @param sprHeight The custom height of each frame of the animation.
     * @param frames The total number of frames in the animation.
     */
    static void registerCustomAnimationSprites(std::string spr, int sprWidth, int sprHeight, int frames) {
        return registerCustomAnimationSprites(spr, 0, 0, sprWidth, sprHeight, frames);
    }

    /**
     * An easy way to register multiple sprites used in an animation, intended to be used for a custom animated object.
     * The sprites for the animation must all have the correct naming scheme for Geometry Dash to handle them correctly: 
     * they must all start with the same name, and have the frame number appended to the end, for example "cat_001.png", "cat_002.png", etc.
     * Pass the name of the first sprite of the animation into this function, ie "cat_001.png", and it will automatically register the rest.
     * 
     * This function applies no custom offset to the animation.
     * 
     * @param spr The name of the first sprite in the animation.
     * @param sprSize The custom size of each frame of the animation. This value is used for both the width and the height.
     * @param frames The total number of frames in the animation.
     */
    static void registerCustomAnimationSprites(std::string spr, int sprSize, int frames) {
        return registerCustomAnimationSprites(spr, 0, 0, sprSize, sprSize, frames);
    }

    /**
     * An easy way to register multiple sprites used in an animation, intended to be used for a custom animated object.
     * The sprites for the animation must all have the correct naming scheme for Geometry Dash to handle them correctly: 
     * they must all start with the same name, and have the frame number appended to the end, for example "cat_001.png", "cat_002.png", etc.
     * Pass the name of the first sprite of the animation into this function, ie "cat_001.png", and it will automatically register the rest.
     * 
     * This function applies no custom offset to the animation, and each frame is registered using the actual size of the sprite.
     * 
     * @param spr The name of the first sprite in the animation.
     * @param frames The total number of frames in the animation.
     */
    static void registerCustomAnimationSprites(std::string spr, int frames) {
        return registerCustomAnimationSprites(spr, 0, 0, 0, 0, frames);
    }

    /**
     * Offsets the object IDs of the custom objects added by your mod.
     * This should only be used as a last resort way to avoid object ID collisions with other mods that use this API.
     * 
     * This should realistically NEVER HAVE TO BE USED!!
     * Please make sure you know what you are doing before using it!
     * 
     * @param offset The number used to determine the new object ID offset, it can be any number you want.
     */
    static void setCollisionOffset(uint8_t offset) {
        CustomObjectsUtils::setCollisionOffset(geode::Mod::get(), offset);
    }
};

#define CUSTOM_SPRITE1(name) CUSTOM_SPRITE5(name, 0, 0, 0, 0)
#define CUSTOM_SPRITE2(name, s) CUSTOM_SPRITE5(name, 0, 0, s, s)
#define CUSTOM_SPRITE3(name, w, h) CUSTOM_SPRITE5(name, 0, 0, w, h)

#define CUSTOM_SPRITE5(name,x,y,w,h) GEODE_CONCAT("custom-objects/",GEODE_MOD_ID)GEODE_STR(/x.y.w.h/)name##".png"

#define $sprite(...) (GEODE_INVOKE(GEODE_CONCAT(CUSTOM_SPRITE,GEODE_NUMBER_OF_ARGS(__VA_ARGS__)),__VA_ARGS__))

#define $object(ObjectType, ObjectBase) ObjectType final : public ObjectBase##Base<ObjectType>
#define $object2(ObjectType, ObjectBase) ObjectType final : public CustomObjectBase<ObjectType, ObjectBase>