#include "CustomObjectsAPI.hpp"
#include "CustomObjectsSheet.hpp"

CustomObjectsManager* CustomObjectsManager::get() {
    if (!s_manager) s_manager = new CustomObjectsManager();
    return s_manager;
} // get

CustomObjectsMod* CustomObjectsManager::registerCustomObjectsMod(geode::Mod* mod, char offset) {
    auto registeredMod = CustomObjectsMod(mod, offset);
    registeredMods.emplace_back(registeredMod);
    return &registeredMods.back();
} // registerCustomObjectsMod

void CustomObjectsManager::processRegisteredMods() {
    customObjectsCache.clear();
    modCustomObjectsCache.clear();

    for (auto mod : registeredMods) {
        std::vector<CustomObjectConfig> objects;

        for (auto obj : mod.objects) {
            obj.generateFrames();

            objects.emplace_back(obj);
            customObjectsCache[obj.id] = obj;
        } // for

        modCustomObjectsCache[mod.modID] = objects;
    } // for
} // processRegisteredMods

std::string CustomObjectsManager::getCacheDirectory() {
    auto path = Mod::get()->getSaveDir().string() + "/cache/";
    if (!std::filesystem::exists(path)) std::filesystem::create_directory(path);
    return path;
} // getCacheDirectory

std::string CustomObjectsManager::getSpritesheetQualityName() {
    switch (getTextureQuality()) {
        case Quality::LOW: return "CustomObjects";
        case Quality::MEDIUM: return "CustomObjects-hd";
        case Quality::HIGH: return "CustomObjects-uhd";
        default: return "CustomObjects-uhd";
    } // switch
} // getSpritesheetImagePath

Quality CustomObjectsManager::getTextureQuality() {
    int quality = (int)CCDirector::get()->getLoadedTextureQuality();
    return (quality == 3) ? Quality::HIGH : (Quality)quality;
} // getTextureQuality

int CustomObjectsManager::getModObjectCount(std::string id) {
    return (modCustomObjectsCache.contains(id)) ? modCustomObjectsCache[id].size() : 0;
} // getModObjectCount

void CustomObjectsManager::printModObjectCount() {
    for (auto [mod, objs] : modCustomObjectsCache) log::info("Mod \"{}\" registered {} custom objects", mod, objs.size());
    for (auto [id, obj] : customObjectsCache) log::info("{} {}", obj.frame, obj.sourceFrame);
} // printModObjectCount

void CustomObjectsManager::forEachCustomObject(std::function<void(const CustomObjectConfig)> operation) const {
    for (auto [id, obj] : customObjectsCache) operation(obj);
} // forEachCustomObject

bool CustomObjectsManager::isTheSpritesheetCacheUpToDate() {
    auto cache = Mod::get()->getSavedValue<std::vector<std::string>>(getSpritesheetQualityName());

    if (customObjectsCache.size() != cache.size()) return false;

    for (auto [id, obj] : customObjectsCache) {
        if (std::find(cache.begin(), cache.end(), obj.frame) == cache.end()) return false;
    } // for

    return true;
} // isTheSpritesheetCacheUpToDate

void CustomObjectsManager::addSpritesheetToCache(std::string name, Quality quality) {
    auto spritesheet = CustomObjectsSheet::create(customObjectsCache, quality);
    if (!spritesheet) {
        log::error("Failed to create spritesheet!!!");
        return;
    } // if

    auto path = getCacheDirectory();
    bool saved = spritesheet->saveSpritesheetImage(name, path);
    saved = saved && spritesheet->saveSpritesheetPlist(name, path);

    if (saved) log::info("Saved spritesheet to \"{}\"", path + name + ".png");
    else log::error("Failed to save spritesheet!!!");
} // addSpritesheetToCache