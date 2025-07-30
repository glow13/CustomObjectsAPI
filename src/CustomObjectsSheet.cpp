#include "CustomObjectsSheet.hpp"
#include "CustomObjectsAPI.hpp"

#include "rectpack2D/finders_interface.hpp"

CCImage* CustomObjectsSheet::createSpritesheetImage() const {
    int csf = CCDirector::get()->getContentScaleFactor();
    auto render = CCRenderTexture::create(sheetSize.width / csf, sheetSize.height / csf);
    render->begin();

    for (auto obj : spritesCache) {
        auto rotatedSize = (obj.rotated ? CCSize(obj.size.height, obj.size.width) : obj.size) / csf;

        auto spr = CCSprite::createWithSpriteFrameName(obj.sourceFrame.c_str());
        spr->setPosition(CCPoint(obj.pos.x, sheetSize.height - obj.pos.y) / csf);
        spr->setAnchorPoint(obj.rotated ? CCPoint(0, 0) : CCPoint(0, 1));
        spr->setScaleX((rotatedSize.width) / spr->getContentWidth());
        spr->setScaleY((rotatedSize.height) / spr->getContentHeight());
        spr->setRotation(obj.rotated ? 90 : 0);
        spr->visit();
    } // for

    render->end();
    auto image = render->newCCImage();
    image->autorelease();
    return image;
} // createSpritesheetImage

CCDictionary* CustomObjectsSheet::createSpritesheetData(std::string name) const {
    auto data = CCDictionary::create();

    // Add the sprite frames
    auto frames = CCDictionary::create();
    for (auto spr : spritesCache) {
        auto sprData = CCDictionary::create();
        sprData->setObject(CCString::create("{0,0}"), "spriteOffset");
        sprData->setObject(CCString::create(spr.getSizeString()), "spriteSize");
        sprData->setObject(CCString::create(spr.getSizeString()), "spriteSourceSize");
        sprData->setObject(CCString::create(spr.getRectString()), "textureRect");
        sprData->setObject(CCString::create(spr.rotated ? "true" : "false"), "textureRotated");
        frames->setObject(sprData, spr.frame);
    } // for

    // Add the spritesheet metadata
    auto metadata = CCDictionary::create();
    metadata->setObject(CCString::create("3"), "format");
    metadata->setObject(CCString::create("custom-objects/" + name + ".png"), "realTextureFileName");
    metadata->setObject(CCString::create(getSizeString()), "size");
    metadata->setObject(CCString::create("custom-objects/" + name + ".png"), "textureFileName");

    // Add them to the data dict
    data->setObject(frames, "frames");
    data->setObject(metadata, "metadata");

    return data;
} // createSpritesheetData

CustomObjectsSheet* CustomObjectsSheet::create(std::map<int, ModCustomObject> objects, Quality quality) {
    std::vector<CustomObjectSprite> sprites;
    float totalArea = 0;

    // Initialize sprites vector and find side lengths
    for (auto [id, obj] : objects) {
        auto spr = CustomObjectSprite(obj.sourceFrame, obj.spriteSize, quality);

        // Check if this sprite is already present
        if (std::find_if(sprites.begin(), sprites.end(), [spr](CustomObjectSprite other) {
            return spr.sourceFrame == other.sourceFrame && spr.size == other.size;
        }) < sprites.end()) continue;

        sprites.emplace_back(spr);
        totalArea += spr.size.width * spr.size.height;
    } // for

    // Run the bin packing algorithm
    auto size = CustomObjectsSheet::binPacking(sprites);

    // Create the spritesheet object
    auto sheet = new CustomObjectsSheet();
    sheet->spritesCache = sprites;
    sheet->sheetSize = size;
    sheet->autorelease();

    // Calculate total coverage
    float coverage = std::min(totalArea / (size.width * size.height) * 100, 100.0f);
    log::debug("Generated the spritesheet with dimensions ({} x {}) and {}% total coverage", size.width, size.height, coverage);

    // Return the final spritesheet object
    return sheet;
} // create

CCSize CustomObjectsSheet::binPacking(std::vector<CustomObjectSprite> &sprites) {
    using namespace rectpack2D;
    auto rectangles = std::vector<rect_xywhf>(sprites.size());

    float totalWidth = 0;
    for (auto spr : sprites) {
        rectangles.emplace_back(rect_xywhf(0, 0, spr.size.width + 2, spr.size.height + 2, false));
        totalWidth += std::max(spr.size.width, spr.size.height);
    } // for

    auto size = find_best_packing<empty_spaces<true>>(rectangles, make_finder_input(
        totalWidth / 2, -4,
        [](rect_xywhf&) { return callback_result::CONTINUE_PACKING; },
        [](rect_xywhf&) { return callback_result::ABORT_PACKING; },
        flipping_option::ENABLED
    ));

    for (auto &spr : sprites) for (int i = 0; i < rectangles.size(); i++) {
        if (spr.size.width == rectangles[i].w - 2 && spr.size.height == rectangles[i].h - 2) {
            spr.pos = CCPoint(rectangles[i].x, rectangles[i].y);
        } else if (spr.size.width == rectangles[i].h - 2 && spr.size.height == rectangles[i].w - 2) {
            spr.pos = CCPoint(rectangles[i].x, rectangles[i].y);
            spr.size.swap();
            spr.rotated = true;
        } else continue;

        rectangles.erase(rectangles.begin() + i);
        break;
    } // for

    return CCSize(std::ceil((size.w - 2) / 4), std::ceil((size.h - 2) / 4)) * 4;
} // binPacking