#include "CustomObjectsAPI.hpp"

CustomObjectsManager* CustomObjectsManager::get() {
    if (!s_manager) {
        s_manager = new CustomObjectsManager();

        s_manager->m_customObjectsDict = CCDictionary::create();
        s_manager->m_modCustomObjectsDict = CCDictionary::create();
    } // if
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

int CustomObjectsManager::getModObjectCount(gd::string id) {
    auto objs = static_cast<CCArray*>(m_modCustomObjectsDict->objectForKey(id));
    return (objs) ? objs->count() : 0;
} // getModObjectCount

ModCustomObject* CustomObjectsManager::getCustomObject(int index) {
    auto id = static_cast<CCInteger*>(m_customObjectsDict->allKeys()->objectAtIndex(index));
    return static_cast<ModCustomObject*>(m_customObjectsDict->objectForKey(id->getValue()));
} // getCustomObject

ModCustomObject* CustomObjectsManager::getCustomObjectByID(int id) {
    return static_cast<ModCustomObject*>(m_customObjectsDict->objectForKey(id));
} // getCustomObjectByID

void CustomObjectsManager::printModObjectCount() {
    CCDictElement* objs = nullptr;
    CCDICT_FOREACH(m_modCustomObjectsDict, objs) {
        int count = static_cast<CCArray*>(objs->getObject())->count();
        log::info("Mod \"{}\" registered {} custom objects", objs->getStrKey(), count);
    } // for
} // printModObjectCount

void CustomObjectsManager::addSpritesheetToCache(gd::string name, Quality quality) {
    auto spritesheet = CustomObjectsSheet::create(m_customObjectsDict, quality);
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
    auto obj = new ModCustomObject(spr, getModObjectCount(mod), size * 30, create);

    m_customObjectsDict->setObject(obj, obj->m_id);

    if (auto modArr = static_cast<CCArray*>(m_modCustomObjectsDict->objectForKey(mod))) modArr->addObject(obj);
    else m_modCustomObjectsDict->setObject(CCArray::createWithObject(obj), mod);

    log::info("Registered custom object with id {}", obj->m_id);
} // registerCustomObject