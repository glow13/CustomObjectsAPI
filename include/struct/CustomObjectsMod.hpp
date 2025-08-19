#pragma once
#include <Geode/Geode.hpp>

#include "object/CustomGameObject.hpp"

using namespace geode::prelude;

struct CustomObjectsMod {
    geode::Mod* mod;
    std::string modID;

    int objectID;
    std::vector<CustomObjectConfig> objects;

    CustomObjectsMod(geode::Mod* mod, short offset) : mod(mod), modID(mod->getID()) {
        uint32_t min = 10000;
        uint32_t max = INT32_MAX - 100;

        // Hash the mod id for a good objectID
        uint64_t hash = geode::utils::hash(modID);
        uint64_t transform = min + (hash * (max - min)) / UINT32_MAX;
        objectID = transform - (transform % 100);
    } // CustomObjectsMod

    CustomObjectConfig& registerCustomObject(std::string spr, CCSize size, std::function<GameObject*(CustomObjectConfig)> create = CustomGameObjectBase::create) {
        auto sprName = spr.substr(spr.find("/") + 1);
        int id = objectID + objects.size();

        auto& obj = objects.emplace_back(CustomObjectConfig(spr, modID, id, size, create));
        log::debug("Registered custom object with id {}", id);
        return obj;
    } // registerCustomObject

    CustomObjectConfig& registerCustomObject(std::string spr, std::function<GameObject*(CustomObjectConfig)> create = CustomGameObjectBase::create) { return registerCustomObject(spr, CCSize(30, 30), create); }
};