#pragma once
#include <Geode/Geode.hpp>

#include "data/CustomObjectConfig.hpp"
#include "data/CustomObjectsMod.hpp"

using namespace geode::prelude;

class CustomObjectsManager {
private:
    static inline CustomObjectsManager* s_manager;

    std::vector<CustomObjectsMod*> registeredMods;
    std::unordered_map<int, ICustomObjectConfig*> customObjectsCache;

public:
    static CustomObjectsManager* get();

    CustomObjectsMod* registerCustomObjectsMod(geode::Mod* mod, char offset = 0);
    bool areAllRegisteredModsProcessed();
    void processRegisteredMods();
    void printModObjectCount();

    const ICustomObjectConfig* getCustomObjectByID(int id);
    void forEachCustomObject(std::function<void(const ICustomObjectConfig*)> operation) const;
};