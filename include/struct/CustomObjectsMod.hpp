#pragma once
#include <Geode/Geode.hpp>

#include "object/CustomGameObject.hpp"

using namespace geode::prelude;

struct CustomObjectsMod {
    geode::Mod* mod;
    std::string modID;

    int objectID;
    std::vector<CustomObjectConfig> objects;

    CustomObjectsMod(geode::Mod* mod, char offset) : mod(mod), modID(mod->getID()) {
        uint32_t min = 10000;
        uint32_t max = INT32_MAX - 100;

        // Hash the mod id for a good objectID
        uint64_t hash = geode::utils::hash(modID + offset);
        uint64_t transform = min + (hash * (max - min)) / UINT32_MAX;
        objectID = transform - (transform % 100);
    } // CustomObjectsMod

    CustomObjectConfig& registerCustomObject(std::string spr, std::string detail, std::function<GameObject*(CustomObjectConfig)> create = CustomGameObjectBase::create) {
        int id = objectID + objects.size();
        log::debug("Registered custom object with id {}", id);
        return objects.emplace_back(CustomObjectConfig(spr, detail, modID, id, create));
    } // registerCustomObject

    CustomObjectConfig& registerCustomObject(std::string spr, std::function<GameObject*(CustomObjectConfig)> create = CustomGameObjectBase::create) { return registerCustomObject(spr, "", create); }
};