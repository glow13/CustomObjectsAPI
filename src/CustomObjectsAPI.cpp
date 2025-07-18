#include "CustomObjectsAPI.hpp"

CustomObjectsManager* CustomObjectsManager::get() {
    if (!s_manager) {
        s_manager = new CustomObjectsManager();

        s_manager->m_customObjectsCount = 0;
        s_manager->m_customObjects = CCArray::create();
        s_manager->m_modCustomObjectsDict = CCDictionary::create();
        s_manager->m_modCustomObjectsCount = CCDictionary::create();
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
    auto count = static_cast<CCInteger*>(m_modCustomObjectsCount->objectForKey(id));
    if (!count) m_modCustomObjectsCount->setObject(CCInteger::create(0), id);
    return (count) ? count->getValue() : 0;
} // getModObjectCount

void CustomObjectsManager::incrementModObjectCount(gd::string id) {
    auto count = static_cast<CCInteger*>(m_modCustomObjectsCount->objectForKey(id));
    if (!count) m_modCustomObjectsCount->setObject(CCInteger::create(1), id);
    else m_modCustomObjectsCount->setObject(CCInteger::create(count->getValue() + 1), id);
} // incrementModObjectCount

ModCustomObject* CustomObjectsManager::getCustomObject(int index) {
    auto obj = m_customObjects->objectAtIndex(index);
    return static_cast<ModCustomObject*>(obj);
} // getCustomObject

ModCustomObject* CustomObjectsManager::getCustomObjectByID(int id) {
    auto obj = m_modCustomObjectsDict->objectForKey(id);
    return static_cast<ModCustomObject*>(obj);
} // getCustomObjectByID

void CustomObjectsManager::printModObjectCount() {
    auto keys = m_modCustomObjectsCount->allKeys();
    for (int i = 0; i < keys->count(); i++) {
        gd::string mod = static_cast<CCString*>(keys->objectAtIndex(i))->m_sString;
        int count = static_cast<CCInteger*>(m_modCustomObjectsCount->objectForKey(mod))->getValue();
        log::info("Mod \"{}\" registered {} custom objects", mod, count);
    } // for
} // printModObjectCount

void CustomObjectsManager::addSpritesheetToCache(gd::string name, Quality quality) {
    auto spritesheet = CustomObjectsSheet::create(m_customObjects, quality);
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

    m_customObjects->addObject(obj);
    m_modCustomObjectsDict->setObject(obj, obj->m_id);

    m_customObjectsCount++;
    incrementModObjectCount(mod);

    log::info("Registered custom object with id {}", obj->m_id);
} // registerCustomObject