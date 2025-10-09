#include "CustomObjectsManager.hpp"
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
    customSpritesCache.clear();
    customObjectsCache.clear();

    for (auto& mod : registeredMods) {
        for (auto& obj : mod.objects) {
            if (obj.isCustomRender()) {
                obj.mainSprite.frame = obj.mainSprite.sourceFrame;
                obj.detailSprite.frame = obj.detailSprite.sourceFrame;
                obj.glowSprite.frame = obj.glowSprite.sourceFrame;

                obj.mainSprite.custom = false;
                obj.detailSprite.custom = false;
                obj.glowSprite.custom = false;
            } else {

                // Register animation frames
                if (obj.hasCustomAnimation()) {
                    for (int i = 2; !obj.mainSprite.sourceFrame.empty() && i <= obj.framesCount; i++) 
                        customSpritesCache.emplace_back(obj.mainSprite.generateAnimationFrame(i));
                    for (int i = 2; !obj.detailSprite.sourceFrame.empty() && i <= obj.framesCount; i++) 
                        customSpritesCache.emplace_back(obj.detailSprite.generateAnimationFrame(i));
                } // if

                obj.mainSprite.generateFrame();
                obj.detailSprite.generateFrame();
                obj.glowSprite.generateFrame();

                if (obj.mainSprite) customSpritesCache.emplace_back(obj.mainSprite);
                if (obj.detailSprite) customSpritesCache.emplace_back(obj.detailSprite);
                if (obj.glowSprite) customSpritesCache.emplace_back(obj.glowSprite);
            } // if

            // Setup object animation
            if (obj.hasCustomAnimation()) {
                std::string mainAnimSprite = obj.mainSprite.frame;
                std::string detailAnimSprite = obj.detailSprite ? obj.detailSprite.frame : obj.mainSprite.frame;

                mainAnimSprite = mainAnimSprite.substr(0, mainAnimSprite.find("_001"));
                detailAnimSprite = detailAnimSprite.substr(0, detailAnimSprite.find("_001"));

                auto manager = GameManager::sharedState();
                manager->addGameAnimation(obj.id, obj.framesCount, obj.frameTime, mainAnimSprite, detailAnimSprite, 1);
            } // if

            customObjectsCache[obj.id] = obj;
        } // for

        for (auto& spr : mod.sprites) {
            spr.generateFrame();
            if (spr) customSpritesCache.emplace_back(spr);
        } // for
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

void CustomObjectsManager::printModObjectCount() {
    for (auto mod : registeredMods) log::info("Mod \"{}\" registered {} custom objects", mod.modID, mod.objects.size());
} // printModObjectCount

void CustomObjectsManager::forEachCustomObject(std::function<void(const CustomObjectConfig)> operation) const {
    for (auto [id, obj] : customObjectsCache) operation(obj);
} // forEachCustomObject

bool CustomObjectsManager::isTheSpritesheetCacheUpToDate() {
    auto cache = Mod::get()->getSavedValue<std::vector<std::string>>(getSpritesheetQualityName());

    if (customSpritesCache.size() != cache.size()) return false;

    for (auto spr : customSpritesCache) {
        if (std::find(cache.begin(), cache.end(), spr.frame) == cache.end()) return false;
    } // for

    return true;
} // isTheSpritesheetCacheUpToDate

void CustomObjectsManager::addSpritesheetToCache(std::string name, Quality quality) {
    auto spritesheet = CustomObjectsSheet::create(customSpritesCache, quality);
    if (!spritesheet) {
        log::error("Failed to create spritesheet!!!");
        return;
    } // if

    auto path = getCacheDirectory();
    bool saved = spritesheet->saveSpritesheetImage(name, path);
    saved = saved && spritesheet->saveSpritesheetPlist(name, path);

    // Save the frames in the sprite cache
    std::vector<std::string> sprites;
    for (auto spr : customSpritesCache) sprites.emplace_back(spr.frame);
    Mod::get()->setSavedValue<std::vector<std::string>>(name, sprites);

    if (saved) log::info("Saved spritesheet to \"{}\"", path + name + ".png");
    else log::error("Failed to save spritesheet!!!");
} // addSpritesheetToCache