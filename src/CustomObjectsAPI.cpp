#include "CustomObjectsAPI.hpp"
#include "CustomObjectsSheet.hpp"

#include <random>

CustomObjectsManager* CustomObjectsManager::get() {
    if (!s_manager) s_manager = new CustomObjectsManager();

    s_manager->m_generationOffsetValue = 0;
    return s_manager;
} // get

std::string CustomObjectsManager::getCacheDirectory() {
    auto path = Mod::get()->getSaveDir().string() + "/cache/";
    if (!std::filesystem::exists(path)) std::filesystem::create_directory(path);
    return path;
} // getCacheDirectory

std::string CustomObjectsManager::getSpritesheetQualityName() {
    switch (GameManager::sharedState()->m_texQuality) {
        case 1: return "CustomObjects";
        case 2: return "CustomObjects-hd";
        case 3: return "CustomObjects-uhd";
        default: return "CustomObjects-hd";
    } // switch
} // getSpritesheetImagePath

int CustomObjectsManager::modToObjectId(std::string modId) {
    int pos = modId.find(".");
    auto dev = modId.substr(0, pos);
    auto mod = modId.substr(pos + 1);

    int devNum = 0;
    for (int i = 0, f = 0; i < dev.length() && f < 3; i++) {
        if (dev[i] < 'a' || dev[i] > 'z') continue;
        devNum = (devNum * 26) + (dev[i] - 'a'), f++;
    } // for

    int modNum = 0;
    for (int i = 0, f = 0; i < mod.length() && f < 6; i++) {
        if (mod[i] < 'a' || mod[i] > 'z') continue;
        modNum = (modNum * 26) + (mod[i] - 'a'), f++;
    } // for

    std::uniform_int_distribution<int> range(200,19999);
    std::default_random_engine engine(modNum);
    std::function<int()> random = std::bind(range, engine);

    for (int i = 0; i < devNum + m_generationOffsetValue; i++) random();
    return random() * 50;
} // modToObjectId

int CustomObjectsManager::getModObjectCount(std::string id) {
    return (modCustomObjectsCache.contains(id)) ? modCustomObjectsCache[id].size() : 0;
} // getModObjectCount

void CustomObjectsManager::printModObjectCount() {
    for (auto [mod, objs] : modCustomObjectsCache) log::info("Mod \"{}\" registered {} custom objects", mod, objs.size());
} // printModObjectCount

void CustomObjectsManager::forEachCustomObject(std::function<void(const ModCustomObject)> operation) const {
    for (auto [id, obj] : customObjectsCache) operation(obj);
} // forEachCustomObject

bool CustomObjectsManager::isTheSpritesheetCacheUpToDate() {
    auto cache = Mod::get()->getSavedValue<std::vector<std::string>>("custom-objects");
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

void CustomObjectsManager::registerCustomObject(std::string spr, CCSize size, std::function<GameObject*(int)> create) {
    auto mod = spr.substr(0, spr.find("/"));
    int id = modToObjectId(mod) + getModObjectCount(mod);
    auto obj = ModCustomObject(spr, id, size * 30, create);

    customObjectsCache[obj.id] = obj;

    if (modCustomObjectsCache.contains(mod)) modCustomObjectsCache[mod].emplace_back(obj);
    else modCustomObjectsCache[mod] = { obj };

    log::debug("Registered custom object with id {}", obj.id);
} // registerCustomObject