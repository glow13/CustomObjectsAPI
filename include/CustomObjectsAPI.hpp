#pragma once

#include "data/CustomSpriteConfig.hpp"
#include "data/CustomObjectConfig.hpp"
#include "data/CustomObjectsMod.hpp"

#include "CustomObjectBase.hpp"
#include "object/CustomGameObject.hpp"

#ifdef GEODE_MOD_ID

#define GEODE_SPRITE1(name) SPRITE5(name, 0, 0, 0, 0)
#define GEODE_SPRITE2(name, s) SPRITE5(name, 0, 0, s, s)
#define GEODE_SPRITE3(name, w, h) SPRITE5(name, 0, 0, w, h)

#define GEODE_SPRITE5(name,x,y,w,h) GEODE_CONCAT("custom-objects/",GEODE_MOD_ID)GEODE_STR(/x.y.w.h/)name##".png"
#define $sprite(...) (GEODE_INVOKE(GEODE_CONCAT(GEODE_SPRITE,GEODE_NUMBER_OF_ARGS(__VA_ARGS__)),__VA_ARGS__))

#endif

#define $object(ObjectType, ObjectBase) ObjectType final : public ObjectBase##Base<ObjectType>
#define $object2(ObjectType, ObjectBase) ObjectType final : public CustomObjectBase<ObjectType, ObjectBase>

class CUSTOM_OBJECTS_DLL CustomObjectsUtils {
private:
    static geode::Mod* currentGeodeMod;
    static CustomObjectsMod* currentMod;
    static int currentOffset;

    static CustomObjectsMod* getMod(geode::Mod* mod);
    static void setCollisionOffset(geode::Mod* mod, uint8_t offset);

    friend class CustomObjectsAPI;
};

class CustomObjectsAPI {
public:
    static void setCollisionOffset(uint8_t offset) {
        CustomObjectsUtils::setCollisionOffset(Mod::get(), offset);
    }

    template <class ObjectType = CustomGameObject>
    static CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight) {
        return CustomObjectsUtils::getMod(Mod::get())->registerCustomObject<ObjectType>(spr, sprOffsetX, sprOffsetY, sprWidth, sprHeight);
    }

    template <class ObjectType = CustomGameObject>
    static CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr, int sprWidth, int sprHeight) {
        return registerCustomObject<ObjectType>(spr, 0, 0, sprWidth, sprHeight);
    }

    template <class ObjectType = CustomGameObject>
    static CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr, int sprSize) {
        return registerCustomObject<ObjectType>(spr, 0, 0, sprSize, sprSize);
    }

    template <class ObjectType = CustomGameObject>
    static CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr) {
        return registerCustomObject<ObjectType>(spr, 0, 0, 0, 0);
    }

    static CustomSpriteConfig& registerCustomSprite(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight) {
        return CustomObjectsUtils::getMod(Mod::get())->registerCustomSprite(spr, sprOffsetX, sprOffsetY, sprWidth, sprHeight);
    }

    static CustomSpriteConfig& registerCustomSprite(std::string spr, int sprWidth, int sprHeight) {
        return registerCustomSprite(spr, 0, 0, sprWidth, sprHeight);
    }

    static CustomSpriteConfig& registerCustomSprite(std::string spr, int sprSize) {
        return registerCustomSprite(spr, 0, 0, sprSize, sprSize);
    }

    static CustomSpriteConfig& registerCustomSprite(std::string spr) {
        return registerCustomSprite(spr, 0, 0, 0, 0);
    }

    static void registerCustomAnimationSprites(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight, int frames) {
        return CustomObjectsUtils::getMod(Mod::get())->registerCustomAnimationSprites(spr, sprOffsetX, sprOffsetY, sprWidth, sprHeight, frames);
    }

    static void registerCustomAnimationSprites(std::string spr, int sprWidth, int sprHeight, int frames) {
        return registerCustomAnimationSprites(spr, 0, 0, sprWidth, sprHeight, frames);
    }

    static void registerCustomAnimationSprites(std::string spr, int sprSize, int frames) {
        return registerCustomAnimationSprites(spr, 0, 0, sprSize, sprSize, frames);
    }

    static void registerCustomAnimationSprites(std::string spr, int frames) {
        return registerCustomAnimationSprites(spr, 0, 0, 0, 0, frames);
    }
};