#include "CustomObjectsAPI.hpp"
#include "CustomObjectsSheet.hpp"

/*
    For this mod to work you need to make these functions return pointers!!!!
    GameObject::getObjectRect2
    GameObject::getObjectTextureRect
*/

CustomObjectsManager* CustomObjectsManager::get() {
    if (!s_manager) s_manager = new CustomObjectsManager();

    s_manager->m_generationOffsetValue = 0;
    return s_manager;
} // get

gd::string CustomObjectsManager::getCacheDirectory() {
    auto path = Mod::get()->getSaveDir().string() + "/custom-objects/";
    if (!std::filesystem::exists(path)) std::filesystem::create_directory(path);
    return path;
} // getCacheDirectory

gd::string CustomObjectsManager::getSpritesheetQualityName() {
    switch (GameManager::sharedState()->m_texQuality) {
        case 1: return "CustomObjects";
        case 2: return "CustomObjects-hd";
        case 3: return "CustomObjects-uhd";
    } // switch
    return "";
} // getSpritesheetImagePath

int CustomObjectsManager::modToObjectId(gd::string modId) {
    int pos = modId.find(".");
    gd::string dev = modId.substr(0, pos);
    gd::string mod = modId.substr(pos + 1);

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

    srand(modNum);
    for (int i = 0; i < devNum + m_generationOffsetValue; i++) rand();
    int uniqueID = (rand() * 989999.0f) / RAND_MAX;
    return uniqueID + 10000;
} // modToObjectId

int CustomObjectsManager::getModObjectCount(gd::string id) {
    return (modCustomObjectsCache.contains(id)) ? modCustomObjectsCache[id].size() : 0;
} // getModObjectCount

void CustomObjectsManager::printModObjectCount() {
    for (auto [mod, objs] : modCustomObjectsCache) log::info("Mod \"{}\" registered {} custom objects", mod, objs.size());
} // printModObjectCount

void CustomObjectsManager::forEachCustomObject(std::function<void(ModCustomObject)> operation) {
    for (auto [id, obj] : customObjectsCache) operation(obj);
} // forEachCustomObject

void CustomObjectsManager::addSpritesheetToCache(gd::string name, Quality quality) {
    auto spritesheet = CustomObjectsSheet::create(customObjectsCache, quality);
    if (!spritesheet) {
        log::error("Failed to create spritesheet!!!");
        return;
    } // if

    auto image = spritesheet->createSpritesheetImage();
    auto sheetPath = getCacheDirectory() + name + ".png";
    bool saved = image->saveToFile(sheetPath.c_str(), false);

    auto dict = spritesheet->createSpritesheetData(name);
    auto dataPath = getCacheDirectory() + name + ".plist";
    saved = saved && dict->writeToFile(dataPath.c_str());

    if (saved) log::info("Saved spritesheet to \"{}\"", sheetPath);
    else log::error("Failed to save spritesheet!!!");
} // addSpritesheetToCache

void CustomObjectsManager::registerCustomObject(gd::string spr, CCSize size, std::function<GameObject*(int)> create) {
    gd::string mod = spr.substr(0, spr.find("/"));
    int id = modToObjectId(mod) + getModObjectCount(mod);
    auto obj = ModCustomObject(spr, id, size * 30, create);

    customObjectsCache[obj.id] = obj;

    if (modCustomObjectsCache.contains(mod)) modCustomObjectsCache[mod].emplace_back(obj);
    else modCustomObjectsCache[mod] = { obj };

    log::info("Registered custom object with id {}", obj.id);
} // registerCustomObject