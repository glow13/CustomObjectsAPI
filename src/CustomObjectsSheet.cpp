#include "CustomObjectsSheet.hpp"
#include "CustomObjectsAPI.hpp"

#include "rectpack2D/finders_interface.hpp"

CCImage* CustomObjectsSheet::createSpritesheetImage() const {
    int csf = CCDirector::get()->getContentScaleFactor();
    auto render = CCRenderTexture::create(m_sheetSize.width / csf, m_sheetSize.height / csf);
    render->begin();

    for (int i = 0; i < m_sprites->count(); i++) {
        auto obj = static_cast<CustomObjectSprite*>(m_sprites->objectAtIndex(i));
        auto rotatedSize = (obj->m_rotated ? CCSize(obj->m_size.height, obj->m_size.width) : obj->m_size) / csf;

        auto spr = CCSprite::createWithSpriteFrameName(obj->m_sourceFrame.c_str());
        spr->setPosition(CCPoint(obj->m_pos.x, m_sheetSize.height - obj->m_pos.y) / csf);
        spr->setAnchorPoint(obj->m_rotated ? CCPoint(0, 0) : CCPoint(0, 1));
        spr->setScaleX((rotatedSize.width) / spr->getContentWidth());
        spr->setScaleY((rotatedSize.height) / spr->getContentHeight());
        spr->setRotation(obj->m_rotated ? 90 : 0);
        spr->visit();
    } // for

    render->end();
    auto image = render->newCCImage();
    image->autorelease();
    return image;
} // createSpritesheetImage

CCDictionary* CustomObjectsSheet::createSpritesheetData(gd::string name) const {
    auto data = CCDictionary::create();

    // Add the sprite frames
    auto frames = CCDictionary::create();
    for (int i = 0; i < m_sprites->count(); i++) {
        auto spr = static_cast<CustomObjectSprite*>(m_sprites->objectAtIndex(i));

        auto sprData = CCDictionary::create();
        sprData->setObject(CCString::create("{0,0}"), "spriteOffset");
        sprData->setObject(CCString::create(spr->getSizeString()), "spriteSize");
        sprData->setObject(CCString::create(spr->getSizeString()), "spriteSourceSize");
        sprData->setObject(CCString::create(spr->getRectString()), "textureRect");
        sprData->setObject(CCString::create(spr->m_rotated ? "true" : "false"), "textureRotated");
        frames->setObject(sprData, spr->m_frame);
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

CustomObjectsSheet* CustomObjectsSheet::create(CCArray* objects, Quality quality) {
    std::vector<CustomObjectSprite*> sprites;
    float totalArea = 0;

    // Initialize sprites vector and find side lengths
    for (int i = 0; i < objects->count(); i++) {
        auto obj = static_cast<ModCustomObject*>(objects->objectAtIndex(i));
        auto spr = new CustomObjectSprite(obj->m_spr, obj->m_mod, obj->m_spriteSize, quality);

        // Check if this sprite is already present
        if (std::find_if(sprites.begin(), sprites.end(), [spr](CustomObjectSprite* other) {
            return spr->m_sourceFrame == other->m_sourceFrame && spr->m_size == other->m_size;
        }) < sprites.end()) continue;

        sprites.push_back(spr);
        totalArea += spr->m_size.width * spr->m_size.height;
    } // for

    // Run the bin packing algorithm
    auto size = CustomObjectsSheet::binPacking(sprites);

    // Create the spritesheet object
    auto sheet = new CustomObjectsSheet();
    sheet->m_sprites = CCArray::create();
    sheet->m_sheetSize = size;
    sheet->autorelease();

    // Add the finished sprites to the sheet
    for (CustomObjectSprite* spr : sprites) sheet->m_sprites->addObject(spr);

    // Calculate total coverage
    float coverage = std::min(totalArea / (size.width * size.height) * 100, 100.0f);
    log::info("Generated the spritesheet with dimensions ({} x {}) and {}% total coverage", size.width, size.height, coverage);

    // Return the final spritesheet object
    return sheet;
} // create

CCSize CustomObjectsSheet::binPacking(std::vector<CustomObjectSprite*> &sprites) {
    using namespace rectpack2D;
    auto rectangles = std::vector<rect_xywhf>(sprites.size());

    float totalWidth = 0;
    for (auto spr : sprites) {
        rectangles.emplace_back(rect_xywhf(0, 0, spr->m_size.width + 2, spr->m_size.height + 2, false));
        totalWidth += std::max(spr->m_size.width, spr->m_size.height);
    } // for

    auto size = find_best_packing<empty_spaces<true>>(rectangles, make_finder_input(
        totalWidth / 2, -4,
        [](rect_xywhf&) { return callback_result::CONTINUE_PACKING; },
        [](rect_xywhf&) { return callback_result::ABORT_PACKING; },
        flipping_option::ENABLED
    ));

    for (auto spr : sprites) for (int i = 0; i < rectangles.size(); i++) {
        if (spr->m_size.width == rectangles[i].w - 2 && spr->m_size.height == rectangles[i].h - 2) {
            spr->m_pos = CCPoint(rectangles[i].x, rectangles[i].y);
        } else if (spr->m_size.width == rectangles[i].h - 2 && spr->m_size.height == rectangles[i].w - 2) {
            spr->m_pos = CCPoint(rectangles[i].x, rectangles[i].y);
            spr->m_size.swap();
            spr->m_rotated = true;
        } else continue;

        rectangles.erase(rectangles.begin() + i);
        break;
    } // for

    return CCSize(std::ceil((size.w - 2) / 4), std::ceil((size.h - 2) / 4)) * 4;
} // binPacking