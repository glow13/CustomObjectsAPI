#include <Geode/Geode.hpp>
#include <Geode/utils/base64.hpp>

#include "CustomObjectsManager.hpp"
#include "CustomSpritesManager.hpp"
#include "data/CustomObjectConfig.hpp"
#include "data/CustomObjectsMod.hpp"
#include "ModTriggerObject.hpp"

using namespace geode::prelude;

CustomObjectsManager* CustomObjectsManager::get() {
    static CustomObjectsManager manager;
    return &manager;
} // get

CustomObjectsMod* CustomObjectsManager::registerCustomObjectsMod(geode::Mod* mod, char offset) {
    if (auto it = std::find_if(registeredMods.begin(), registeredMods.end(), [mod](auto customMod) {
        return customMod->getModID() == mod->getID();
    }); it != registeredMods.end()) return nullptr;

    auto registeredMod = new CustomObjectsMod(mod, offset);

    auto trigger = new CustomObjectConfig<ModTriggerObject>(registeredMod);
    registeredMod->registerCustomObject(trigger, "mod-trigger.png"_spr, 0, 0, 0, 0);
    trigger->setEditorTabPriority(INT_MIN);
    trigger->setDisableBatchRender();

    auto triggerSprite = new CustomSpriteConfig(registeredMod, trigger, "mod-trigger.png"_spr, 0, 0, 0, 0);
    registeredMod->sprites.emplace_back(triggerSprite);

    trigger->onEditObjectButton([registeredMod](auto obj, auto objs) {
        SetupModTriggerPopup::create(obj, objs, registeredMod->getModID(), registeredMod->getModName())->show();
    });

    return registeredMods.emplace_back(registeredMod);
} // registerCustomObjectsMod

void CustomObjectsManager::removeCustomObjectsMod(geode::Mod* mod) {
    if (auto it = std::find_if(registeredMods.begin(), registeredMods.end(), [mod](auto customMod) {
        return customMod->getModID() == mod->getID();
    }); it != registeredMods.end()) registeredMods.erase(it);
} // removeCustomObjectsMod

bool CustomObjectsManager::areAllRegisteredModsProcessed() const {
    int registeredObjectsCount = 0;
    for (auto mod : registeredMods) registeredObjectsCount += mod->objects.size();
    return customObjectsCache.size() >= registeredObjectsCount;
} // areAllRegisteredModsProcessed

void CustomObjectsManager::processRegisteredMods() {
    auto spriteManager = CustomSpritesManager::get();
    auto toolbox = ObjectToolbox::sharedState();
    customObjectsCache.clear();

    for (auto mod : registeredMods) {
        for (auto obj : mod->objects) {
            if (!obj->isCustomBatch() && !obj->mainSprite.isModTrigger()) {
                obj->mainSprite.frameName = obj->mainSprite.sourceFrame;
                obj->detailSprite.frameName = obj->detailSprite.sourceFrame;
                obj->glowSprite.frameName = obj->glowSprite.sourceFrame;
            } else {
                spriteManager->registerCustomObjectSprite(&obj->mainSprite);
                spriteManager->registerCustomObjectSprite(&obj->detailSprite);
                spriteManager->registerCustomObjectSprite(&obj->glowSprite);
            } // if

            if (obj->hasCustomAnimation()) {
                auto mainAnimSprite = obj->getMainSprite();
                auto detailAnimSprite = obj->hasDetailSprite() ? obj->getDetailSprite() : obj->getMainSprite();

                mainAnimSprite = mainAnimSprite.substr(0, mainAnimSprite.find("_001"));
                detailAnimSprite = detailAnimSprite.substr(0, detailAnimSprite.find("_001"));

                auto manager = GameManager::sharedState();
                manager->addGameAnimation(obj->objectID, obj->framesCount, obj->frameTime, mainAnimSprite, detailAnimSprite, 1);
            } // if

            toolbox->m_allKeys.emplace(obj->objectID, obj->getMainSprite());
            customObjectsCache.emplace(obj->objectID, obj);
        } // for

        for (auto spr : mod->sprites) {
            spriteManager->registerCustomObjectSprite(spr);
        } // for
    } // for
} // processRegisteredMods

void CustomObjectsManager::printModObjectCount() const {
    log::info("A total of {} mods registered {} total custom objects", registeredMods.size(), customObjectsCache.size());
} // printModObjectCount

int CustomObjectsManager::getTotalCustomObjectsCount() const {
    return customObjectsCache.size();
} // getTotalCustomObjectsCount

const CustomObjectConfigBase* CustomObjectsManager::getCustomObjectByID(int id) const {
    auto it = customObjectsCache.find(id);
    return it != customObjectsCache.end() ? it->second : nullptr;
} // getCustomObjectByID

std::map<std::string, CustomObjectsManager::ModObjects> CustomObjectsManager::getEditorTabLayout() const {
    std::map<std::string, ModObjects> mods;
    for (auto [id, obj] : customObjectsCache) {
        mods[obj->getModID()].emplace_back(obj->editorPriority, id);
    } // for

    for (auto& [mod, objs] : mods) {
        std::sort(objs.begin(), objs.end(), [](const auto& a, const auto& b) {
            return a.first == b.first ? a.second < b.second : a.first < b.first;
        });
    } // for

    return mods;
} // getEditorTabLayout