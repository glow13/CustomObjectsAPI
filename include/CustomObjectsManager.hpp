#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct CustomObjectsMod;
struct CustomObjectConfigBase;

class CustomObjectsManager {
private:
    static inline CustomObjectsManager* s_manager;

    std::vector<CustomObjectsMod*> registeredMods;
    std::unordered_map<int, CustomObjectConfigBase*> customObjectsCache;

public:
    static CustomObjectsManager* get();

    CustomObjectsMod* registerCustomObjectsMod(geode::Mod* mod, char offset = 0);
    bool areAllRegisteredModsProcessed();
    void processRegisteredMods();
    void printModObjectCount();

    const CustomObjectConfigBase* getCustomObjectByID(int id);
    void forEachCustomObject(std::function<void(const CustomObjectConfigBase*)> operation) const;
};