#include "CustomObjectsManager.hpp"
#include "CustomSpritesManager.hpp"
#include "data/CustomObjectConfig.hpp"
#include "data/CustomObjectsMod.hpp"
#include "ModTriggerObject.hpp"

CustomObjectsManager* CustomObjectsManager::get() {
    if (!s_manager) s_manager = new CustomObjectsManager();
    return s_manager;
} // get

CustomObjectsMod* CustomObjectsManager::registerCustomObjectsMod(geode::Mod* mod, char offset) {
    auto registeredMod = new CustomObjectsMod(mod, offset);

    auto triggerSprite = new CustomSpriteConfig("mod-trigger.png"_spr, registeredMod->modID, CCRectZero);
    registeredMod->sprites.emplace_back(triggerSprite);
    triggerSprite->mod = true;

    auto& trigger = registeredMod->registerCustomObject<ModTriggerObject>("mod-trigger.png"_spr);
    trigger.setEditorTabPriority(INT_MIN);
    trigger.setDisableBatchRender();

    registeredMods.emplace_back(registeredMod);
    return registeredMods.back();
} // registerCustomObjectsMod

bool CustomObjectsManager::areAllRegisteredModsProcessed() {
    int registeredObjectsCount = 0;
    for (auto mod : registeredMods) registeredObjectsCount += mod->objects.size();
    return customObjectsCache.size() >= registeredObjectsCount;
} // areAllRegisteredModsProcessed

void CustomObjectsManager::processRegisteredMods() {
    auto spriteManager = CustomSpritesManager::get();
    customObjectsCache.clear();

    for (auto mod : registeredMods) {
        for (auto obj : mod->objects) {
            if (obj->isCustomRender()) {
                obj->mainSprite.frame = obj->mainSprite.sourceFrame;
                obj->detailSprite.frame = obj->detailSprite.sourceFrame;
                obj->glowSprite.frame = obj->glowSprite.sourceFrame;

                obj->mainSprite.custom = false;
                obj->detailSprite.custom = false;
                obj->glowSprite.custom = false;
            } else {
                spriteManager->registerCustomObjectSprite(&obj->mainSprite);
                spriteManager->registerCustomObjectSprite(&obj->detailSprite);
                spriteManager->registerCustomObjectSprite(&obj->glowSprite);
            } // if

            if (obj->hasCustomAnimation()) {
                auto mainAnimSprite = obj->mainSprite.frame;
                auto detailAnimSprite = obj->detailSprite ? obj->detailSprite.frame : obj->mainSprite.frame;

                mainAnimSprite = mainAnimSprite.substr(0, mainAnimSprite.find("_001"));
                detailAnimSprite = detailAnimSprite.substr(0, detailAnimSprite.find("_001"));

                auto manager = GameManager::sharedState();
                manager->addGameAnimation(obj->id, obj->framesCount, obj->frameTime, mainAnimSprite, detailAnimSprite, 1);
            } // if

            customObjectsCache[obj->id] = obj;
        } // for

        for (auto spr : mod->sprites) {
            spriteManager->registerCustomObjectSprite(spr);
        } // for
    } // for
} // processRegisteredMods

void CustomObjectsManager::printModObjectCount() {
    int modCount = registeredMods.size();
    int objectCount = customObjectsCache.size();
    log::info("A total of {} mods registered {} total custom objects", modCount, objectCount);
} // printModObjectCount

const ICustomObjectConfig* CustomObjectsManager::getCustomObjectByID(int id) {
    if (!customObjectsCache.contains(id)) return nullptr;
    return customObjectsCache[id];
} // getCustomObjectByID

void CustomObjectsManager::forEachCustomObject(std::function<void(const ICustomObjectConfig*)> operation) const {
    for (auto [id, obj] : customObjectsCache) operation(obj);
} // forEachCustomObject