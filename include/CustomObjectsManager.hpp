#pragma once

class CustomObjectsMod;
class CustomObjectConfigBase;

class CustomObjectsManager final {
private:
    std::vector<std::unique_ptr<CustomObjectsMod>> registeredMods;
    std::unordered_map<int, std::unique_ptr<CustomObjectConfigBase>> customObjectsCache;

    explicit CustomObjectsManager() = default;
    CustomObjectsManager(const CustomObjectsManager&) = delete;
    CustomObjectsManager& operator=(const CustomObjectsManager&) = delete;

public:
    static CustomObjectsManager* get();

    CustomObjectsMod* registerCustomObjectsMod(geode::Mod* mod, char offset = 0);
    void removeCustomObjectsMod(geode::Mod* mod);
    bool areAllRegisteredModsProcessed();
    void processRegisteredMods();
    void printModObjectCount();

    const CustomObjectConfigBase* getCustomObjectByID(int id);
    void forEachCustomObject(std::function<void(const CustomObjectConfigBase*)> operation) const;
};