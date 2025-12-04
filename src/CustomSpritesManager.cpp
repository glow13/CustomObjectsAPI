#include "CustomSpritesManager.hpp"

CustomSpritesManager* CustomSpritesManager::get() {
    if (!s_manager) s_manager = new CustomSpritesManager();
    return s_manager;
} // get

std::string CustomSpritesManager::getCacheDirectory() {
    auto path = Mod::get()->getSaveDir().string() + "/cache/";
    if (!std::filesystem::exists(path)) std::filesystem::create_directory(path);
    return path;
} // getCacheDirectory

Quality CustomSpritesManager::getTextureQuality() {
    int quality = (int)CCDirector::get()->getLoadedTextureQuality();
    return (quality == 3) ? Quality::HIGH : (Quality)quality;
} // getTextureQuality

std::string CustomSpritesManager::getSpritesheetQualityName() {
    switch (getTextureQuality()) {
        case Quality::LOW: return "CustomObjects";
        case Quality::MEDIUM: return "CustomObjects-hd";
        case Quality::HIGH: return "CustomObjects-uhd";
        default: return "CustomObjects-uhd";
    } // switch
} // getSpritesheetImagePath

void CustomSpritesManager::registerCustomObjectSprite(CustomSpriteConfig* spr) {
    spr->generateFrame();
    if (*spr) customSpritesCache.emplace_back(spr);
} // registerCustomObjectSprite

void CustomSpritesManager::processRegisteredSprites() {
    std::unordered_set<std::string> sprites;
    auto it = std::remove_if(customSpritesCache.begin(), customSpritesCache.end(), [&sprites](auto spr) {
        return !sprites.insert(spr->frame).second;
    });
    customSpritesCache.erase(it, customSpritesCache.end());
} // processRegisteredSprites

bool CustomSpritesManager::isTheSpritesheetCacheUpToDate() {
    auto cache = Mod::get()->getSavedValue<std::vector<std::string>>(getSpritesheetQualityName());

    if (customSpritesCache.size() != cache.size()) return false;

    for (int i = 0; i < customSpritesCache.size(); i++) {
        if (customSpritesCache[i]->frame == cache[i]) continue;
        if (std::find(cache.begin(), cache.end(), customSpritesCache[i]->frame) != cache.end()) continue;
        return false;
    } // for

    return true;
} // isTheSpritesheetCacheUpToDate

void CustomSpritesManager::saveSpritesheetDataToCache(std::string name) {
    std::vector<std::string> sprites;
    for (auto spr : customSpritesCache) sprites.emplace_back(spr->frame);
    Mod::get()->setSavedValue<std::vector<std::string>>(name, sprites);
} // saveSpritesheetDataToCache

void CustomSpritesManager::addSpritesheetToCache(std::string name, Quality quality) {
    auto spritesheet = CustomObjectsSheet::create(customSpritesCache, quality);
    if (!spritesheet) {
        log::error("Failed to create spritesheet!!!");
        return;
    } // if

    auto path = getCacheDirectory();
    bool saved = spritesheet->saveSpritesheetImage(name, path);
    saved = saved && spritesheet->saveSpritesheetPlist(name, path);

    // Save the frames in the sprite cache
    if (saved) {
        saveSpritesheetDataToCache(name);
        log::info("Saved spritesheet to \"{}\"", path + name + ".png");
    } else log::error("Failed to save custom spritesheet!");
} // addSpritesheetToCache