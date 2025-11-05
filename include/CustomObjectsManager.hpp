#pragma once
#include <Geode/Geode.hpp>

#include "struct/CustomObjectConfig.hpp"
#include "struct/CustomObjectsMod.hpp"

using namespace geode::prelude;

class CustomObjectsManager {
private:
    static inline CustomObjectsManager* s_manager;

    std::vector<CustomObjectsMod*> registeredMods;
    std::map<int, ICustomObjectConfig*> customObjectsCache;

public:
    static CustomObjectsManager* get();

    CustomObjectsMod* registerCustomObjectsMod(geode::Mod* mod, char offset = 0);
    void processRegisteredMods();

    int getObjectCount();
    void printModObjectCount();

    bool containsCustomObject(int id);
    const ICustomObjectConfig* getCustomObjectByID(int id);
    void forEachCustomObject(std::function<void(const ICustomObjectConfig*)> operation) const;
};