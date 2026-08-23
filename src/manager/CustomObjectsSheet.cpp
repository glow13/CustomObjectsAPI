#include <Geode/Geode.hpp>
#include "../config/CustomSpriteConfig.hpp"
#include "CustomObjectsSheet.hpp"
#include "CustomObjectsManager.hpp"

#define LODEPNG_NO_COMPILE_DECODER
#define LODEPNG_NO_COMPILE_ANCILLARY_CHUNKS
#define LODEPNG_NO_COMPILE_ERROR_TEXT

#include <finders_interface.h>
#include <lodepng.h>

using namespace geode::prelude;
using namespace rectpack2D;

constexpr int SPRITE_BUFFER = 2;

// Checks sprite frames AND files
inline CCSpriteFrame* getCCSpriteFrame(std::string name) {
    if (auto frame = CCSpriteFrameCache::get()->m_pSpriteFrames->objectForKey(name))
        return static_cast<CCSpriteFrame*>(frame);
    else if (CCSprite spr; spr.initWithFile(name.c_str())) return spr.displayFrame();
    else return nullptr;
}

// Checks sprite frames AND files
inline CCSprite* getCCSprite(std::string name) {
    if (auto frame = CCSpriteFrameCache::get()->m_pSpriteFrames->objectForKey(name))
        return CCSprite::createWithSpriteFrame(static_cast<CCSpriteFrame*>(frame));
    else if (auto spr = CCSprite::create(name.c_str()); !spr->isUsingFallback()) return spr;
    else return nullptr;
}

inline std::string offString(SheetInfo spr) {
    int offsetX = (spr.m_trim.x + spr.m_trim.w * 0.5f) - (spr.m_size.w * 0.5f) + spr.m_offset.w;
    int offsetY = (spr.m_size.h * 0.5f) - (spr.m_trim.y + spr.m_trim.h * 0.5f) + spr.m_offset.h;
    return "{" + fmt::format("{},{}", offsetX, offsetY) + "}";
}

inline std::string sizeString(SheetInfo spr) {
    int width = spr.m_rect.flipped ? spr.m_rect.h : spr.m_rect.w;
    int height = spr.m_rect.flipped ? spr.m_rect.w : spr.m_rect.h;
    return "{" + fmt::format("{},{}", width, height) + "}";
}

inline std::string rectString(SheetInfo spr) {
    return "{{" + fmt::format("{},{}", spr.m_rect.x, spr.m_rect.y) + "}," + sizeString(spr) + "}";
}

inline std::string sourceString(SheetInfo spr) {
    return "{" + fmt::format("{},{}", spr.m_size.w, spr.m_size.h) + "}";
}

inline std::string rotatedString(SheetInfo spr) {
    return spr.m_rect.flipped ? "<true/>" : "<false/>";
}

std::string CustomObjectsSheet::getCacheDirectory() {
    auto path = geode::Mod::get()->getSaveDir().string() + "/cache/";
    if (!std::filesystem::exists(path)) std::filesystem::create_directory(path);
    return geode::utils::string::pathToString(path);
}

Quality CustomObjectsSheet::getTextureQuality() {
    int quality = (int)cocos2d::CCDirector::get()->getLoadedTextureQuality();
    return (quality == 3) ? Quality::HIGH : (Quality)quality;
}

std::string CustomObjectsSheet::getSpritesheetQualityName(Quality quality) {
    switch (quality) {
        case Quality::LOW: return "CustomObjects";
        case Quality::MEDIUM: return "CustomObjects-hd";
        case Quality::HIGH: return "CustomObjects-uhd";
        default: return "CustomObjects-uhd";
    }
}

inline rectpack2D::rect_wh binPacking(std::vector<SheetInfo> &sprites) {
    // Buffer sprites
    long totalArea = 0;
    for (auto& spr : sprites) {
        spr.m_rect.w += SPRITE_BUFFER;
        spr.m_rect.h += SPRITE_BUFFER;
        totalArea += spr.m_rect.w * spr.m_rect.h;
    }

    // These values feel fine based on some very unscientific testing
    const int minWidth = std::sqrt(totalArea) * 2;
    const int maxWidth = std::sqrt(totalArea) * 4;
    const int interval = (maxWidth - minWidth) * 0.2;

    rect_wh size;
    int width = minWidth;
    auto onBinPackingSuccess = [](rect_xywhf&) { return callback_result::CONTINUE_PACKING; };
    auto onBinPackingFailure = [&width, interval](rect_xywhf&) {
        log::warn("Failed to generate the spritesheet at size {}, retrying...", width);
        width += interval; // increase width until success
        return callback_result::ABORT_PACKING;
    };

    while ((size.w == 0 || size.h == 0) && width < maxWidth + 10) {
        int startingWidth = width;
        std::vector<SheetInfo> tempSprites = sprites;

        auto finderInput = make_finder_input(
            width, -4,
            onBinPackingSuccess,
            onBinPackingFailure,
            flipping_option::ENABLED
        );

        size = find_best_packing<empty_spaces<true>>(
            tempSprites,
            finderInput
        );

        if (width > startingWidth) size = {0, 0}; // this means it failed
        else sprites = tempSprites; // success!
    }

    // Remove sprite buffer
    for (auto& spr : sprites) {
        spr.m_rect.w -= SPRITE_BUFFER;
        spr.m_rect.h -= SPRITE_BUFFER;
    }

    return {size.w - SPRITE_BUFFER, size.h - SPRITE_BUFFER};
}

inline void makeModTriggerSprite(CCSprite& sprite, std::string colorString) {
    auto path = CCFileUtils::get()->fullPathForFilename("mod-trigger.png"_spr, false);

    CCImage image;
    if (!image.initWithImageFile(path.c_str())) return;

    uint32_t colorHash = geode::utils::hash(colorString);
    uint8_t colorR = (colorHash >> 8) & 0xFF;
    uint8_t colorG = (colorHash >> 16) & 0xFF;
    uint8_t colorB = (colorHash >> 24) & 0xFF;

    auto data = image.getData();
    for (int i = 0; i < image.getDataLen() * 4; i += 4) {
        if (data[i+0] || !data[i+1] || data[i+2] || !data[i+3]) continue;

        float a = data[i+1] / 255.0f;
        data[i+0] = colorR * a;
        data[i+1] = colorG * a;
        data[i+2] = colorB * a;
    }

    auto texture = new CCTexture2D();
    if (!texture->initWithImage(&image)) return;
    texture->autorelease();

    sprite.initWithTexture(texture);
}

inline bool saveSpritesheetImage(std::vector<SheetInfo>& sprites, rect_wh sheetSize, std::string name, std::string path) {
    if (sheetSize.w <= 0 || sheetSize.h <= 0) return false;

    auto quality = CustomObjectsSheet::getTextureQuality();
    auto render = CCRenderTexture::create(sheetSize.w / quality, sheetSize.h / quality);
    render->begin();

    // Add each sprite to the sheet
    for (auto spr : sprites) {
        CCSprite sprite;
        if (spr.m_sprite->isModTrigger()) {
            if (makeModTriggerSprite(sprite, spr.m_sprite->getModID()); !sprite.getTexture()) continue;
        } else if (!sprite.initWithFile(spr.m_sprite->getSourceFrame().c_str())) {
            sprite.initWithSpriteFrameName(spr.m_sprite->getSourceFrame().c_str());
        }

        int offsetX = spr.m_rect.flipped ? spr.m_size.h - spr.m_trim.h - spr.m_trim.y : spr.m_trim.x;
        int offsetY = spr.m_rect.flipped ? spr.m_trim.x : spr.m_trim.y;

        sprite.setPosition(CCPoint(spr.m_rect.x - offsetX, sheetSize.h - spr.m_rect.y + offsetY) / quality);
        sprite.setAnchorPoint(spr.m_rect.flipped ? CCPoint(0, 0) : CCPoint(0, 1));
        sprite.setScaleX(spr.m_size.w / (sprite.getContentWidth() * (int)quality));
        sprite.setScaleY(spr.m_size.h / (sprite.getContentHeight() * (int)quality));
        sprite.setRotation(spr.m_rect.flipped ? 90 : 0);

        sprite.visit();
    }

    // Save the rendered image pixel data
    auto& s = render->m_pTexture->getContentSizeInPixels();
    uint32_t w = s.width, h = s.height;

    std::vector<uint8_t> data(w * h * 4);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    render->end();
    render->release();

    // Flip the image
    for (int y = 0; y < h / 2; y++) {
        auto begin = data.begin() + y * w * 4;
        auto end = data.end() - (y + 1) * w * 4;
        std::swap_ranges(begin, begin + w * 4, end);
    }

    // Reverse premultiply alpha
    for (uint32_t i = 0; i < w * h * 4; i += 4) {
        float a = data[i+3];
        if (a == 0 || a == 255) continue;

        data[i+0] *= 255 / a;
        data[i+1] *= 255 / a;
        data[i+2] *= 255 / a;
    }

    return lodepng::encode((path + name + ".png"), data, w, h) == 0;
}

inline bool saveSpritesheetPlist(std::vector<SheetInfo>& sprites, rect_wh sheetSize, std::string name, std::string path) {
    auto fullPath = path + name + ".plist";
    auto file = std::ofstream(fullPath);

    if (!file.is_open()) {
        log::error("Failed to create the spritesheet plist file \"{}.plist\"", name);
        return false;
    } // if

    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
    file << "<plist version=\"1.0\">\n";
    file << "<dict>\n\t<key>frames</key>\n\t<dict>\n";

    for (auto spr : sprites) {
        file << "\t\t<key>"+spr.m_sprite->getFrameName()+"</key>\n\t\t<dict>\n";
        file << "\t\t\t<key>spriteOffset</key>\n\t\t\t<string>"+offString(spr)+"</string>\n";
        file << "\t\t\t<key>spriteSize</key>\n\t\t\t<string>"+sizeString(spr)+"</string>\n";
        file << "\t\t\t<key>spriteSourceSize</key>\n\t\t\t<string>"+sourceString(spr)+"</string>\n";
        file << "\t\t\t<key>textureRect</key>\n\t\t\t<string>"+rectString(spr)+"</string>\n";
        file << "\t\t\t<key>textureRotated</key>\n\t\t\t"+rotatedString(spr)+"\n\t\t</dict>\n";
    } // for

    auto sizeString = fmt::format("{},{}", sheetSize.w, sheetSize.h);

    file << "\t</dict>\n\t<key>metadata</key>\n\t<dict>\n";
    file << "\t\t<key>format</key>\n\t\t<integer>3</integer>\n";
    file << "\t\t<key>pixelFormat</key>\n\t\t<string>RGBA8888</string>\n";
    file << "\t\t<key>premultiplyAlpha</key>\n\t\t<false/>\n";
    file << "\t\t<key>realTextureFileName</key>\n\t\t<string>"+name+".png</string>\n";
    file << "\t\t<key>size</key>\n\t\t<string>{"+sizeString+"}</string>\n";
    file << "\t\t<key>textureFileName</key>\n\t\t<string>"+name+".png</string>\n";
    file << "\t</dict>\n</dict>\n</plist>";

    file.close();
    return true;
}

inline void saveSpritesheetDataToCache(const std::vector<CustomSpriteConfig*>& customSprites, std::string name) {
    std::vector<std::string> sprites;
    for (auto spr : customSprites) sprites.emplace_back(spr->getFrameName());
    Mod::get()->setSavedValue<std::vector<std::string>>(name, sprites);
}

void CustomObjectsSheet::addSpritesheetToCache(const std::vector<CustomSpriteConfig*>& customSprites, Quality quality) {
    std::vector<SheetInfo> sprites;
    for (const auto& spr : customSprites) sprites.emplace_back(spr->getSheetInfo(quality));

    auto qualityString = (int)quality == 4 ? "HIGH" : ((int)quality == 2 ? "MEDIUM" : "LOW");
    log::debug("Generating custom objects spritesheet with {} texture quality...", qualityString);

    auto size = binPacking(sprites);
    size.w = std::ceil(size.w / (float)quality) * (int)quality;
    size.h = std::ceil(size.h / (float)quality) * (int)quality;

    if (size.w <= 0 || size.h <= 0) {
        log::error("Spritesheet generation failed, custom objects rendering may not work correctly!");
    } else log::debug("Spritesheet generation was successful, saving spritesheet to the cache...");

    auto path = getCacheDirectory();
    auto name = getSpritesheetQualityName(quality);

    bool saved = saveSpritesheetImage(sprites, size, name, path);
    saved = saved && saveSpritesheetPlist(sprites, size, name, path);

    // Save the frames in the sprite cache
    if (saved) {
        saveSpritesheetDataToCache(customSprites, name);
        log::info("Saved {} quality spritesheet as \"{}\"", qualityString, path + name + ".png");
    } else log::error("Failed to save the custom objects spritesheet!");
}

CCTexture2D* CustomObjectsSheet::getCustomSpritesheetTexture() {
    if (CustomObjectsManager::get()->getCustomObjectsCount() == 0) return nullptr;

    auto png = getSpritesheetQualityName() + ".png";
    return CCTextureCache::get()->textureForKey(png.c_str());
}