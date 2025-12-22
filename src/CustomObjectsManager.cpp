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

    auto& trigger = registeredMod->registerCustomObject<ModTriggerObject>("mod-trigger.png"_spr);
    trigger.setEditorTabPriority(INT_MIN);
    trigger.setDisableBatchRender();

    auto triggerSprite = new CustomSpriteConfig(registeredMod, &trigger, "mod-trigger.png"_spr, 0, 0, 0, 0);
    registeredMod->sprites.emplace_back(triggerSprite);

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
            if (!obj->isCustomBatch()) {
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
                manager->addGameAnimation(obj->getObjectID(), obj->getFramesCount(), obj->getFrameTime(), mainAnimSprite, detailAnimSprite, 1);
            } // if

            customObjectsCache[obj->getObjectID()] = obj;
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

const CustomObjectConfigBase* CustomObjectsManager::getCustomObjectByID(int id) {
    if (!customObjectsCache.contains(id)) return nullptr;
    return customObjectsCache[id];
} // getCustomObjectByID

void CustomObjectsManager::forEachCustomObject(std::function<void(const CustomObjectConfigBase*)> operation) const {
    for (auto [id, obj] : customObjectsCache) operation(obj);
} // forEachCustomObject