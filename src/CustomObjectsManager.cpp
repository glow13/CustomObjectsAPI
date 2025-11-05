#include "CustomObjectsManager.hpp"
#include "CustomSpritesManager.hpp"

CustomObjectsManager* CustomObjectsManager::get() {
    if (!s_manager) s_manager = new CustomObjectsManager();
    return s_manager;
} // get

CustomObjectsMod* CustomObjectsManager::registerCustomObjectsMod(geode::Mod* mod, char offset) {
    auto registeredMod = new CustomObjectsMod(mod, offset);
    registeredMods.emplace_back(registeredMod);
    return registeredMods.back();
} // registerCustomObjectsMod

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
                spriteManager->processCustomObjectSprite(&(obj->mainSprite));
                spriteManager->processCustomObjectSprite(&(obj->detailSprite));
                spriteManager->processCustomObjectSprite(&(obj->glowSprite));
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
            spriteManager->processCustomObjectSprite(spr);
        } // for
    } // for

    spriteManager->processRegisteredSprites();
} // processRegisteredMods

int CustomObjectsManager::getObjectCount() {
    return customObjectsCache.size();
} // getObjectCount

void CustomObjectsManager::printModObjectCount() {
    int modCount = registeredMods.size();
    int objectCount = customObjectsCache.size();
    log::info("A total of {} mods registered {} total custom objects", modCount, objectCount);
} // printModObjectCount

bool CustomObjectsManager::containsCustomObject(int id) {
    return customObjectsCache.contains(id);
} // containsCustomObject

const ICustomObjectConfig* CustomObjectsManager::getCustomObjectByID(int id) {
    return customObjectsCache[id];
} // getCustomObjectByID

void CustomObjectsManager::forEachCustomObject(std::function<void(const ICustomObjectConfig*)> operation) const {
    for (auto [id, obj] : customObjectsCache) operation(obj);
} // forEachCustomObject