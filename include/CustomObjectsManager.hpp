#pragma once
#include <Geode/Geode.hpp>

struct CustomObjectsMod;
struct CustomObjectConfigBase;

class CustomObjectsManager final {
private:
    std::vector<CustomObjectsMod*> registeredMods;
    std::unordered_map<int, CustomObjectConfigBase*> customObjectsCache;

    explicit CustomObjectsManager() = default;
    CustomObjectsManager(const CustomObjectsManager&) = delete;
    CustomObjectsManager& operator=(const CustomObjectsManager&) = delete;

public:
    static CustomObjectsManager* get();

    CustomObjectsMod* registerCustomObjectsMod(geode::Mod* mod, char offset = 0);
    bool areAllRegisteredModsProcessed();
    void processRegisteredMods();
    void printModObjectCount();

    const CustomObjectConfigBase* getCustomObjectByID(int id);
    void forEachCustomObject(std::function<void(const CustomObjectConfigBase*)> operation) const;
};