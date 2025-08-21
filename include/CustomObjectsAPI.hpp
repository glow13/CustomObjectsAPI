#pragma once
#include <Geode/Geode.hpp>

#include "struct/CustomObjectConfig.hpp"
#include "struct/CustomObjectsMod.hpp"

using namespace geode::prelude;

enum Quality : int {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 4
};

class CustomObjectsManager : public CCNode {
private:
    static inline CustomObjectsManager* s_manager;

    std::vector<CustomObjectsMod> registeredMods;

    std::map<int, CustomObjectConfig> customObjectsCache;
    std::map<std::string, std::vector<CustomObjectConfig>> modCustomObjectsCache;

public:
    static CustomObjectsManager* get();

    CustomObjectsMod* registerCustomObjectsMod(geode::Mod* mod, byte offset = 0);
    void processRegisteredMods();

    std::string getCacheDirectory();
    std::string getSpritesheetQualityName();
    Quality getTextureQuality() { int q = (int)CCDirector::get()->getLoadedTextureQuality(); return (q == 3) ? Quality::HIGH : (Quality)q; }

    int getObjectCount() { return customObjectsCache.size(); }
    int getModObjectCount(std::string id);
    void printModObjectCount();

    CustomObjectConfig getCustomObjectByID(int id) { return customObjectsCache[id]; }
    bool containsCustomObject(int id) { return customObjectsCache.contains(id); }
    void forEachCustomObject(std::function<void(const CustomObjectConfig)> operation) const;

    bool isTheSpritesheetCacheUpToDate();
    void addSpritesheetToCache(std::string name, Quality quality);
};