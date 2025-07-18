#pragma once
#include <Geode/Geode.hpp>

#include "CustomObjectsSheet.hpp"
#include "object/CustomGameObject.hpp"

using namespace geode::prelude;

struct ModCustomObject : public CCObject {
    gd::string m_frame;
    gd::string m_spr;
    gd::string m_mod;
    int m_id;
    CCSize m_spriteSize;
    std::function<GameObject*(int)> m_createFunction;

    ModCustomObject(gd::string spr, int offset, CCSize size, std::function<GameObject*(int)> create) {
        int pos = spr.find("/");
        this->m_spr = spr.substr(pos + 1);
        this->m_mod = spr.substr(0, pos);
        this->m_frame = "custom-objects" + fmt::format("/{}/{}/", size.width, size.height) + m_spr;
        this->m_id =  modToID(m_mod) + offset;
        this->m_spriteSize = size;
        this->m_createFunction = create;
        this->autorelease();
    } // ModCustomObject

    int modToID(gd::string modID) {
        int pos = modID.find(".");
        gd::string dev = modID.substr(0, pos);
        gd::string mod = modID.substr(pos + 1);

        int devNum = 0;
        for (int i = 0, f = 0; i < dev.length() && f < 3; i++) {
            if (dev[i] < 'a' || dev[i] > 'z') continue;
            devNum = (devNum * 26) + (dev[i] - 'a'), f++;
        } // for

        int modNum = 0;
        for (int i = 0, f = 0; i < mod.length() && f < 3; i++) {
            if (mod[i] < 'a' || mod[i] > 'z') continue;
            modNum = (modNum * 26) + (mod[i] - 'a'), f++;
        } // for

        srand(devNum);
        for (int i = 0; i < modNum; i++) rand();
        int uniqueID = (float)rand() / RAND_MAX * 95000;
        return uniqueID + 4600;
    } // modToID

    GameObject* create() {
        return m_createFunction(m_id);
    } // create
};

class CustomObjectsManager : public CCNode {
private:
    static inline CustomObjectsManager* s_manager;

    int m_customObjectsCount;
    Ref<CCArray> m_customObjects;

    Ref<CCDictionary> m_modCustomObjectsDict;
    Ref<CCDictionary> m_modCustomObjectsCount;
public:
    static CustomObjectsManager* get() {
        if (!s_manager) {
            s_manager = new CustomObjectsManager();

            s_manager->m_customObjectsCount = 0;
            s_manager->m_customObjects = CCArray::create();
            s_manager->m_modCustomObjectsDict = CCDictionary::create();
            s_manager->m_modCustomObjectsCount = CCDictionary::create();
        } // if
        return s_manager;
    } // get

    gd::string getCacheDirectory() {
        auto path = Mod::get()->getSaveDir().string() + "/custom-objects/";
        if (!std::filesystem::exists(path)) std::filesystem::create_directory(path);
        return path;
    } // getCacheDirectory

    gd::string getSpritesheetQualityName() {
        switch (GameManager::sharedState()->m_texQuality) {
            case 1:
                return "CustomObjects";
            case 2:
                return "CustomObjects-hd";
            case 3:
                return "CustomObjects-uhd";
        } // switch
        return "";
    } // getSpritesheetImagePath

    int getObjectCount() {
        return m_customObjectsCount;
    } // getCustomObjectCount

    int getModObjectCount(gd::string id) {
        auto count = static_cast<CCInteger*>(m_modCustomObjectsCount->objectForKey(id));
        if (!count) m_modCustomObjectsCount->setObject(CCInteger::create(0), id);
        return (count) ? count->getValue() : 0;
    } // getModObjectCount

    void incrementModObjectCount(gd::string id) {
        auto count = static_cast<CCInteger*>(m_modCustomObjectsCount->objectForKey(id));
        if (!count) m_modCustomObjectsCount->setObject(CCInteger::create(1), id);
        else m_modCustomObjectsCount->setObject(CCInteger::create(count->getValue() + 1), id);
    } // incrementModObjectCount

    ModCustomObject* getCustomObject(int index) {
        auto obj = m_customObjects->objectAtIndex(index);
        return static_cast<ModCustomObject*>(obj);
    } // getCustomObject

    ModCustomObject* getCustomObjectByID(int id) {
        auto obj = m_modCustomObjectsDict->objectForKey(id);
        return static_cast<ModCustomObject*>(obj);
    } // getCustomObjectByID

    void printModObjectCount() {
        auto keys = m_modCustomObjectsCount->allKeys();
        for (int i = 0; i < keys->count(); i++) {
            gd::string mod = static_cast<CCString*>(keys->objectAtIndex(i))->m_sString;
            int count = static_cast<CCInteger*>(m_modCustomObjectsCount->objectForKey(mod))->getValue();
            log::info("Mod \"{}\" registered {} custom objects", mod, count);
        } // for
    } // printModObjectCount

    void addSpritesheetToCache(gd::string name, Quality quality) {
        auto spritesheet = CustomObjectsSheet::create(m_customObjects, quality);

        auto image = spritesheet->createSpritesheetImage();
        auto sheetPath = getCacheDirectory() + name + ".png";
        bool saved = image->saveToFile(sheetPath.c_str(), false);

        auto dict = spritesheet->createSpritesheetData(name);
        auto dataPath = getCacheDirectory() + name + ".plist";
        saved = saved && dict->writeToFile(dataPath.c_str());

        if (saved) log::info("Saved spritesheet to \"{}\"", sheetPath);
        else log::error("Failed to save spritesheet!!!");
    } // addSpritesheetToCache

    void registerCustomObject(gd::string spr, CCSize size, std::function<GameObject*(int)> create = CustomGameObject::create) {
        gd::string mod = spr.substr(0, spr.find("/"));
        auto obj = new ModCustomObject(spr, getModObjectCount(mod), size * 30, create);

        m_customObjects->addObject(obj);
        m_modCustomObjectsDict->setObject(obj, obj->m_id);

        m_customObjectsCount++;
        incrementModObjectCount(mod);

        log::info("Registered custom object with id {}", obj->m_id);
    } // registerCustomObject

    void registerCustomObject(gd::string spr, std::function<GameObject*(int)> create = CustomGameObject::create) {
        registerCustomObject(spr, CCSize(1, 1), create);
    } // registerCustomObject
};