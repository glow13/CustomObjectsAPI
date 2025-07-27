#include "CustomObjectsAPI.hpp"

CustomObjectsManager* CustomObjectsManager::get() {
    if (!s_manager) {
        s_manager = new CustomObjectsManager();

        s_manager->m_customObjectsDict = CCDictionary::create();
        s_manager->m_modCustomObjectsDict = CCDictionary::create();
    } // if

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
    int id = modToObjectId(mod) + getModObjectCount(mod);
    auto obj = new ModCustomObject(spr, id, size * 30, create);

    m_customObjectsDict->setObject(obj, obj->m_id);

    if (auto modArr = static_cast<CCArray*>(m_modCustomObjectsDict->objectForKey(mod))) modArr->addObject(obj);
    else m_modCustomObjectsDict->setObject(CCArray::createWithObject(obj), mod);

    log::info("Registered custom object with id {}", obj->m_id);
} // registerCustomObject