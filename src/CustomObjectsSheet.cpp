#include "CustomObjectsSheet.hpp"
#include "CustomObjectsAPI.hpp"

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
    auto size = CCSize(0, 0);
    float totalArea = 0;
    float minWidth = 0;
    float maxWidth = 0;
    float minSide = 0;

    // Initialize sprites vector and find side lengths
    for (int i = 0; i < objects->count(); i++) {
        auto obj = static_cast<ModCustomObject*>(objects->objectAtIndex(i));
        auto spr = new CustomObjectSprite(obj->m_spr, obj->m_mod, obj->m_spriteSize, quality);
        if (std::find_if(sprites.begin(), sprites.end(), [spr](CustomObjectSprite* other) {
            return spr->m_sourceFrame == other->m_sourceFrame && spr->m_size == other->m_size;
        }) < sprites.end()) continue;
        else sprites.push_back(spr);

        totalArea += sprites[i]->m_size.width * sprites[i]->m_size.height;
        float side = std::max(sprites[i]->m_size.width, sprites[i]->m_size.height);
        if (side > minWidth) minWidth = side;
        if (side < minSide || minSide == 0) minSide = side;
    } // for
    maxWidth = std::ceil(std::sqrt(totalArea));

    // Sort the sprites from largest size to smallest size
    std::sort(sprites.begin(), sprites.end(), [](CustomObjectSprite* a, CustomObjectSprite* b) {
        float maxA = std::max(a->m_size.width, a->m_size.height);
        float minA = std::min(a->m_size.width, a->m_size.height);
        float maxB = std::max(b->m_size.width, b->m_size.height);
        float minB = std::min(b->m_size.width, b->m_size.height);

        if (maxA != maxB) return maxA > maxB;
        else return minA > minB;
    });

    // Brute force generate multiple spritesheets with different widths
    // This almost always guarantees the most efficient option
    float bestArea = totalArea * 2;
    float bestWidth = maxWidth * 2;
    for (int width = minWidth; width < maxWidth + minSide; width += minSide / 2) {

        // Try running the MaxRects algorithm with this width
        size = CustomObjectsSheet::maxRectsBinPacking(sprites, CCSize(width, maxWidth * 2));

        // Was the spritesheet able to generate?
        if (size.isZero()) continue;

        // Once we have run the algorithm, test to see if the area is smaller
        int area = size.width * size.height;
        if (area < bestArea) {
            bestArea = area;
            bestWidth = width;
        } else if (width == bestWidth) break; // We just went back and generated the best spritesheet, so the algorithm is done

        // Once we have reached the end of the loop, go back to the best width to generate the best spritesheet
        if (width >= maxWidth) width = bestWidth - minSide / 2;
    } // for

    // Make the size a multiple of 4 so that we can render it properly
    size.width = std::ceil(size.width / 4) * 4;
    size.height = std::ceil(size.height / 4) * 4;

    // Create the spritesheet object
    auto sheet = new CustomObjectsSheet();
    sheet->m_sprites = CCArray::create();
    sheet->m_sheetSize = size;
    sheet->autorelease();

    // Add the finished sprites to the sheet
    for (CustomObjectSprite* spr : sprites) sheet->m_sprites->addObject(spr);

    // Calculate total coverage
    float coverage = totalArea / (size.width * size.height);
    log::info("Found the smallest spritesheet with dimensions {} x {}", size.width, size.height);
    log::info("Generated the spritesheet with {}% coverage", std::min(coverage * 100, 100.0f));

    // Return the final spritesheet object
    return sheet;
} // create

/*
    A rudimentary implementation of the MaxRects algorithm for 2D bin packing.
    Can consistently produce spritesheets with 90-95% total coverage of the entire
    spritesheet. Generating the spritesheet at runtime allows for batch rendering
    in-game and dynamic sprite scaling for custom mod objects.

    Algorithm written entirely by me, with some help from ChatGPT :)
*/
CCSize CustomObjectsSheet::maxRectsBinPacking(std::vector<CustomObjectSprite*> &sprites, CCSize binSize) {
    std::vector<CCRect> freeRects = {CCRect(0, 0, binSize.width, binSize.height)};
    auto size = CCSize(0, 0);

    // Simplified MaxRects algorithm
    for (CustomObjectSprite* spr : sprites) {
        if (spr->m_rotated) {
            spr->m_size.swap();
            spr->m_rotated = false;
        } // if

        // Find the best placement
        auto best = std::min_element(freeRects.begin(), freeRects.end(), [spr](CCRect a, CCRect b) {
            return CustomObjectsSheet::scoreRects(spr, spr, a, b);
        });

        // Try rotated
        if (spr->m_size.width != spr->m_size.height) {
            auto sprRot = new CustomObjectSprite(spr->m_size);
            sprRot->m_size.swap();

            // Find the best placement
            auto bestRot = std::min_element(freeRects.begin(), freeRects.end(), [sprRot](CCRect a, CCRect b) {
                return CustomObjectsSheet::scoreRects(sprRot, sprRot, a, b);
            });

            // Is the rotated version a better placement?
            if (CustomObjectsSheet::scoreRects(sprRot, spr, *bestRot, *best)) {
                spr->m_size.swap();
                spr->m_rotated = true;
                best = bestRot;
            } // if
            delete sprRot;
        } // Try rotated

        // Test if the sprite fits in the chosen rect
        if (!spr->fits(*best)) return CCSize(0, 0);

        // Remove the old rect
        CCRect rect = *best;
        freeRects.erase(best);

        // Split the rect
        auto rect1 = CCRect(rect.origin.x + spr->m_size.width + 2, rect.origin.y, rect.size.width - spr->m_size.width - 2, spr->m_size.height);
        auto rect2 = CCRect(rect.origin.x, rect.origin.y + spr->m_size.height + 2, rect.size.width, rect.size.height - spr->m_size.height - 2);
        if (rect1.size.width > 0 && rect1.size.height > 0) freeRects.push_back(rect1);
        if (rect2.size.width > 0 && rect2.size.height > 0) freeRects.push_back(rect2);

        // Prune any redundant rects that are fully contained within another rect
        for (int i = 0; i < freeRects.size(); i++) for (int j = i + 1; j < freeRects.size();) {
            CCRect r1 = freeRects[i], r2 = freeRects[j];
            if (r1.getMinX() >= r2.getMinX() && r1.getMinY() >= r2.getMinY() && r1.getMaxX() <= r2.getMaxX() && r1.getMaxY() <= r2.getMaxY()) {
                freeRects.erase(freeRects.begin() + i);
                i--;
                break;
            } else if (r2.getMinX() >= r1.getMinX() && r2.getMinY() >= r1.getMinY() && r2.getMaxX() <= r1.getMaxX() && r2.getMaxY() <= r1.getMaxY()) {
                freeRects.erase(freeRects.begin() + j);
            } else j++;
        } // Prune rects

        // Update the sprite position
        spr->m_pos = rect.origin;

        // Expand the size if needed
        size.width = std::max(size.width, spr->m_pos.x + spr->m_size.width);
        size.height = std::max(size.height, spr->m_pos.y + spr->m_size.height);
    } // MaxRects algorithm

    return size;
} // maxRectsBinPacking

// Test used by the MaxRects algorithm to score the free rects
bool CustomObjectsSheet::scoreRects(CustomObjectSprite* sprA, CustomObjectSprite* sprB, CCRect a, CCRect b) {
    if (sprA->fits(a) ^ sprB->fits(b)) return sprA->fits(a);
    else if (sprA->bssf(a) != sprB->bssf(b)) return sprA->bssf(a) < sprB->bssf(b);
    else if (sprA->baf(a) != sprB->baf(b)) return sprA->baf(a) < sprB->baf(b);
    else return sprA->distance(a) < sprB->distance(b);
} // scoreRects