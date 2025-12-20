#include "CustomSpritesManager.hpp"
#include "CustomObjectsSheet.hpp"
#include "data/CustomSpriteConfig.hpp"

CustomSpritesManager* CustomSpritesManager::get() {
    if (!s_manager) s_manager = new CustomSpritesManager();
    return s_manager;
} // get

std::string CustomSpritesManager::getCacheDirectory() {
    auto path = Mod::get()->getSaveDir().string() + "/cache/";
    if (!std::filesystem::exists(path)) std::filesystem::create_directory(path);
    return utils::string::pathToString(path);
} // getCacheDirectory

Quality CustomSpritesManager::getTextureQuality() {
    int quality = (int)CCDirector::get()->getLoadedTextureQuality();
    return (quality == 3) ? Quality::HIGH : (Quality)quality;
} // getTextureQuality

std::string CustomSpritesManager::getSpritesheetQualityName() {
    switch (getTextureQuality()) {
        case Quality::LOW: return "CustomObjects";
        case Quality::MEDIUM: return "CustomObjects-hd";
        case Quality::HIGH: return "CustomObjects-uhd";
        default: return "CustomObjects-uhd";
    } // switch
} // getSpritesheetImagePath

void CustomSpritesManager::registerCustomObjectSprite(CustomSpriteConfig* spr) {
    spr->generateFrame();
    if (*spr) customSpritesCache.emplace_back(spr);
} // registerCustomObjectSprite

void CustomSpritesManager::processRegisteredSprites() {
    std::unordered_set<std::string> sprites;
    auto it = std::remove_if(customSpritesCache.begin(), customSpritesCache.end(), [&sprites](auto spr) {
        return !sprites.insert(spr->frame).second;
    });
    customSpritesCache.erase(it, customSpritesCache.end());
} // processRegisteredSprites

bool CustomSpritesManager::isTheSpritesheetCacheUpToDate() {
    auto sheetName = getSpritesheetQualityName();
    auto cache = Mod::get()->getSavedValue<std::vector<std::string>>(sheetName);

    if (customSpritesCache.size() != cache.size()) return false;

    for (int i = 0; i < customSpritesCache.size(); i++) {
        if (customSpritesCache[i]->frame == cache[i]) continue;
        if (std::find(cache.begin(), cache.end(), customSpritesCache[i]->frame) != cache.end()) continue;
        return false;
    } // for

    auto png = sheetName + ".png";
    if (png == CCFileUtils::get()->fullPathForFilename(png.c_str(), false)) return false;

    auto plist = sheetName + ".plist";
    if (plist == CCFileUtils::get()->fullPathForFilename(plist.c_str(), false)) return false;

    return true;
} // isTheSpritesheetCacheUpToDate

void CustomSpritesManager::saveSpritesheetDataToCache(std::string name) {
    std::vector<std::string> sprites;
    for (auto spr : customSpritesCache) sprites.emplace_back(spr->frame);
    Mod::get()->setSavedValue<std::vector<std::string>>(name, sprites);
} // saveSpritesheetDataToCache

void CustomSpritesManager::addSpritesheetToCache(std::string name, Quality quality) {
    auto spritesheet = CustomObjectsSheet::create(customSpritesCache, quality);
    if (!spritesheet) {
        log::error("Failed to create spritesheet!!!");
        return;
    } // if

    log::info("Spritesheet generation was successful, saving spritesheet to the cache...");
    auto path = getCacheDirectory();
    bool saved = spritesheet->saveSpritesheetImage(name, path);
    saved = saved && spritesheet->saveSpritesheetPlist(name, path);

    // Save the frames in the sprite cache
    if (saved) {
        saveSpritesheetDataToCache(name);
        log::info("Saved spritesheet as \"{}\"", path + name + ".png");
    } else log::error("Failed to save custom objects spritesheet!");
} // addSpritesheetToCache

CCSize CustomSpritesManager::getPixelDataFromSprite(CCSprite* spr, ByteVector& data) {
    auto frame = spr->displayFrame();
    auto texture = frame->getTexture();
    auto rect = frame->getRectInPixels();

    auto keys = CCTextureCache::get()->m_pTextures->allKeysForObject(texture);
    if (keys->count() == 0) return CCSizeZero;

    CCImage image;
    auto filePath = keys->stringAtIndex(0)->getCString();
    if (!image.initWithImageFile(filePath)) return CCSizeZero;

    if (frame->isRotated()) rect.size.swap();
    data.resize(rect.size.width * rect.size.height * 4);

    for (int y = 0; y < rect.size.height; y++) {
        auto start = image.getData() + (int)((rect.origin.y + y) * image.getWidth()+ rect.origin.x) * 4;
        std::memcpy(data.data() + (int)(y * rect.size.width) * 4, start, rect.size.width * 4);
    } // for

    return rect.size;
} // getPixelDataFromSprite

CCSprite* CustomSpritesManager::getSpriteFromPixelData(ByteVector& data, CCSize size) {
    if (size.isZero()) return nullptr;

    auto texture = new CCTexture2D();
    texture->autorelease();
    texture->initWithData(
        data.data(),
        kCCTexture2DPixelFormat_RGBA8888,
        size.width,
        size.height, 
        size
    );

    return CCSprite::createWithTexture(texture);
} // getSpriteFromPixelData