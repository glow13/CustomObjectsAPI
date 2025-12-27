#pragma once

#include "CustomObjectsDLL.hpp"

#include "data/CustomSpriteConfig.hpp"
#include "data/CustomObjectConfig.hpp"
#include "data/CustomObjectsMod.hpp"

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

namespace CustomObjectsAPI {
    CUSTOM_OBJECTS_DLL CustomObjectsMod* getMod();

    CUSTOM_OBJECTS_DLL void setCollisionOffset(uint8_t offset);

    template <class ObjectType = CustomGameObject>
    inline CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight) {
        return getMod()->registerCustomObject<ObjectType>(spr, sprOffsetX, sprOffsetY, sprWidth, sprHeight);
    }

    template <class ObjectType = CustomGameObject>
    inline CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr, int sprWidth, int sprHeight) {
        return registerCustomObject<ObjectType>(spr, 0, 0, sprWidth, sprHeight);
    }

    template <class ObjectType = CustomGameObject>
    inline CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr, int sprSize) {
        return registerCustomObject<ObjectType>(spr, 0, 0, sprSize, sprSize);
    }

    template <class ObjectType = CustomGameObject>
    inline CustomObjectConfig<ObjectType>& registerCustomObject(std::string spr) {
        return registerCustomObject<ObjectType>(spr, 0, 0, 0, 0);
    }

    CUSTOM_OBJECTS_DLL inline CustomSpriteConfig& registerCustomSprite(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight) {
        return getMod()->registerCustomSprite(spr, sprOffsetX, sprOffsetY, sprWidth, sprHeight);
    }

    CUSTOM_OBJECTS_DLL inline CustomSpriteConfig& registerCustomSprite(std::string spr, int sprWidth, int sprHeight) {
        return registerCustomSprite(spr, 0, 0, sprWidth, sprHeight);
    }

    CUSTOM_OBJECTS_DLL inline CustomSpriteConfig& registerCustomSprite(std::string spr, int sprSize) {
        return registerCustomSprite(spr, 0, 0, sprSize, sprSize);
    }

    CUSTOM_OBJECTS_DLL inline CustomSpriteConfig& registerCustomSprite(std::string spr) {
        return registerCustomSprite(spr, 0, 0, 0, 0);
    }

    CUSTOM_OBJECTS_DLL inline void registerCustomAnimationSprites(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight, int frames) {
        return getMod()->registerCustomAnimationSprites(spr, sprOffsetX, sprOffsetY, sprWidth, sprHeight, frames);
    }

    CUSTOM_OBJECTS_DLL inline void registerCustomAnimationSprites(std::string spr, int sprWidth, int sprHeight, int frames) {
        return registerCustomAnimationSprites(spr, 0, 0, sprWidth, sprHeight, frames);
    }

    CUSTOM_OBJECTS_DLL inline void registerCustomAnimationSprites(std::string spr, int sprSize, int frames) {
        return registerCustomAnimationSprites(spr, 0, 0, sprSize, sprSize, frames);
    }

    CUSTOM_OBJECTS_DLL inline void registerCustomAnimationSprites(std::string spr, int frames) {
        return registerCustomAnimationSprites(spr, 0, 0, 0, 0, frames);
    }
}