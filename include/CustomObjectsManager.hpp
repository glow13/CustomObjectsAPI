#pragma once

class CustomObjectsMod;
class CustomObjectConfigBase;

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
    void removeCustomObjectsMod(geode::Mod* mod);

    void processRegisteredMods();
    bool areAllRegisteredModsProcessed() const;
    void printModObjectCount() const;

    const CustomObjectConfigBase* getCustomObjectByID(int id) const;

    using ModObjects = std::vector<std::pair<int, int>>;
    std::map<std::string, ModObjects> getEditorTabLayout() const;
};