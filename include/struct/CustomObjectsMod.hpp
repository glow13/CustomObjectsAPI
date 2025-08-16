#pragma once
#include <Geode/Geode.hpp>

#include "object/CustomGameObject.hpp"

using namespace geode::prelude;

struct CustomObjectsMod {
    geode::Mod* mod;
    std::string modID;

    int objectID;
    std::vector<CustomObject> objects;

    CustomObjectsMod(geode::Mod* mod, short offset) : mod(mod), modID(mod->getID()) {

        // Basic FNV hashing algorithm
        uint32_t hash = 0x811C9DC5u + offset; // FNV offset basis
        for (size_t i = 0; i < modID.length(); i++) {
            hash ^= static_cast<uint8_t>(modID[i]);
            hash *= 0x01000193u; // FNV prime
        } // for

        // Transform the hash value to be a valid object id
        uint32_t min = 10000;
        uint32_t max = INT32_MAX - 100;
        uint64_t transform = min + ((uint64_t)hash * (max - min)) / UINT32_MAX;
        objectID = transform - (transform % 100);
    } // CustomObjectsMod

    void registerCustomObject(CustomObject obj) {
        auto name = obj.sourceFrame.substr(obj.sourceFrame.find("/") + 1);
        obj.frame = fmt::format("custom-objects/{}/{}/{}/{}", modID, obj.spriteSize.width, obj.spriteSize.height, name);
        obj.id = objectID + objects.size();

        if (!obj.createFunction) obj.createFunction = CustomGameObjectBase::create;

        objects.emplace_back(obj);
        log::debug("Registered custom object with id {}", obj.id);
    } // registerCustomObject

    void registerCustomObject(std::string spr, CCSize size, std::function<GameObject*(CustomObject)> create = CustomGameObjectBase::create) {
        auto sprName = spr.substr(spr.find("/") + 1);
        int id = objectID + objects.size();

        objects.emplace_back(CustomObject(spr, modID, id, size, create));
        log::debug("Registered custom object with id {}", id);
    } // registerCustomObject

    void registerCustomObject(std::string spr, std::function<GameObject*(CustomObject)> create = CustomGameObjectBase::create) { registerCustomObject(spr, CCSize(30, 30), create); }
};