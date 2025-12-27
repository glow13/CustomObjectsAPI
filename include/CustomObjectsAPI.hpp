#pragma once

#include "data/CustomSpriteConfig.hpp"
#include "data/CustomObjectConfig.hpp"
#include "data/CustomObjectsMod.hpp"

#include "object/CustomGameObject.hpp"

namespace CustomObjectsAPI {
    CustomObjectsMod* getMod();

    void setCollisionOffset(uint8_t offset);

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

    inline CustomSpriteConfig& registerCustomSprite(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight) {
        return getMod()->registerCustomSprite(spr, sprOffsetX, sprOffsetY, sprWidth, sprHeight);
    }

    inline CustomSpriteConfig& registerCustomSprite(std::string spr, int sprWidth, int sprHeight) {
        return registerCustomSprite(spr, 0, 0, sprWidth, sprHeight);
    }

    inline CustomSpriteConfig& registerCustomSprite(std::string spr, int sprSize) {
        return registerCustomSprite(spr, 0, 0, sprSize, sprSize);
    }

    inline CustomSpriteConfig& registerCustomSprite(std::string spr) {
        return registerCustomSprite(spr, 0, 0, 0, 0);
    }

    inline void registerCustomAnimationSprites(std::string spr, int sprOffsetX, int sprOffsetY, int sprWidth, int sprHeight, int frames) {
        return getMod()->registerCustomAnimationSprites(spr, sprOffsetX, sprOffsetY, sprWidth, sprHeight, frames);
    }

    inline void registerCustomAnimationSprites(std::string spr, int sprWidth, int sprHeight, int frames) {
        return registerCustomAnimationSprites(spr, 0, 0, sprWidth, sprHeight, frames);
    }

    inline void registerCustomAnimationSprites(std::string spr, int sprSize, int frames) {
        return registerCustomAnimationSprites(spr, 0, 0, sprSize, sprSize, frames);
    }

    inline void registerCustomAnimationSprites(std::string spr, int frames) {
        return registerCustomAnimationSprites(spr, 0, 0, 0, 0, frames);
    }
}