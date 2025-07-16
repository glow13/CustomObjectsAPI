#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct CustomObjectSprite : public CCObject {
    gd::string m_frame;
    CCPoint m_pos;
    CCSize m_size;
    bool m_rotated;

    CustomObjectSprite(gd::string spr, gd::string mod, CCSize size) {
        this->m_frame = fmt::format("{}/{}", mod, spr);
        this->m_pos = CCPoint(0, 0);
        this->m_size = size;
        this->m_rotated = false;
        this->autorelease();
    } // CustomObjectSprite

    bool fits(CCRect rect) const {
        return rect.size.width >= m_size.width && rect.size.height >= m_size.height;
    } // fits

    float distance(CCRect rect) const {
        return std::max(rect.origin.x, rect.origin.y);
    } // distance

    float bssf(CCRect rect) const {
        float leftoverWidth = rect.size.width - m_size.width;
        float leftoverHeight = rect.size.height - m_size.height;
        return std::min(leftoverWidth, leftoverHeight);
    } // bssf

    float baf(CCRect rect) const {
        float area = m_size.width * m_size.height;
        float rectArea = rect.size.width * rect.size.height;
        return rectArea - area;
    } // baf
};

class CustomObjectsSheet : public CCNode {
public:
    CCArray* m_sprites;
    CCSize m_sheetSize;

    CCRenderTexture* generateSpritesheet() {
        auto render = CCRenderTexture::create(m_sheetSize.width / 4, m_sheetSize.height / 4);
        render->begin();
        
        for (int i = 0; i < m_sprites->count(); i++) {
            auto obj = static_cast<CustomObjectSprite*>(m_sprites->objectAtIndex(i));
            auto rotatedSize = obj->m_rotated ? CCSize(obj->m_size.height, obj->m_size.width) : obj->m_size;

            auto spr = CCSprite::createWithSpriteFrameName(obj->m_frame.c_str());
            spr->setPosition(CCPoint(obj->m_pos.x / 4, (m_sheetSize.height - obj->m_pos.y) / 4));
            spr->setAnchorPoint(obj->m_rotated ? CCPoint(0, 0) : CCPoint(0, 1));
            spr->setScaleX((rotatedSize.width / 4) / spr->getContentWidth());
            spr->setScaleY((rotatedSize.height / 4) / spr->getContentHeight());
            spr->setRotation(obj->m_rotated ? 90 : 0);
            spr->visit();
        } // for

        render->end();
        return render;
    } // generateSpritesheet

    /*
        A rudimentary implementation of the MaxRects algorithm for 2D bin packing.
        Can consistently produce spritesheets with 95-99% space efficiency compared
        to the total area of the sprites. Generating the spritesheet at runtime allows
        for batch rendering in-game and dynamic sprite scaling for custom mod objects.

        Algorithm written entirely by me, with some help from ChatGPT :)
    */
    static CustomObjectsSheet* create(CCArray* objects) {
        auto sprites = std::vector<CustomObjectSprite*>(objects->count());
        auto size = CCSize(0, 0);
        float totalArea = 0;
        float minWidth = 0;
        float maxWidth = 0;

        // Initialize sprites vector and find side lengths
        for (int i = 0; i < objects->count(); i++) {
            auto obj = static_cast<ModCustomObject*>(objects->objectAtIndex(i));
            sprites[i] = new CustomObjectSprite(obj->m_spr, obj->m_mod, obj->m_spriteSize * 4);

            totalArea += (sprites[i]->m_size.width + 2) * (sprites[i]->m_size.height + 2); // not entirely accurate but close enough
            float side = std::max(sprites[i]->m_size.width, sprites[i]->m_size.height);
            if (side > minWidth) minWidth = side;
        } // for
        maxWidth = std::sqrt(totalArea);

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
        for (int width = minWidth; width < maxWidth + 120; width += 60) {
            std::vector<CCRect> freeRects = {CCRect(0, 0, width, (totalArea / width) * 2)};
            size = CCSize(0, 0);

            // MaxRects algorithm
            bool success = true;
            for (CustomObjectSprite* spr : sprites) {
                if (spr->m_rotated) {
                    spr->m_size.swap();
                    spr->m_rotated = false;
                } // if

                // Find the best placement
                auto best = std::min_element(freeRects.begin(), freeRects.end(), [spr](CCRect a, CCRect b) {
                    if (spr->fits(a) ^ spr->fits(b)) return spr->fits(a);
                    return CustomObjectsSheet::compareSprites(spr, spr, a, b);
                });

                // Try rotated
                if (spr->m_size.width != spr->m_size.height) {
                    auto sprRot = new CustomObjectSprite("", "", spr->m_size);
                    sprRot->m_size.swap();

                    // Find the best placement
                    auto bestRot = std::min_element(freeRects.begin(), freeRects.end(), [sprRot](CCRect a, CCRect b) {
                        if (sprRot->fits(a) ^ sprRot->fits(b)) return sprRot->fits(a);
                        return CustomObjectsSheet::compareSprites(sprRot, sprRot, a, b);
                    });

                    // Is the rotated version a better placement?
                    if (CustomObjectsSheet::compareSprites(sprRot, spr, *bestRot, *best)) {
                        spr->m_size.swap();
                        spr->m_rotated = true;
                        best = bestRot;
                    } // if
                    delete sprRot;
                } // Try rotated

                // Test if the sprite fits in the chosen rect
                if (!spr->fits(*best)) {
                    success = false;
                    break;
                } // if

                // Remove the old rect
                CCRect rect = *best;
                freeRects.erase(best);

                // Split the rect
                auto rect1 = CCRect(rect.origin.x + spr->m_size.width + 2, rect.origin.y, rect.size.width - spr->m_size.width - 2, spr->m_size.height);
                auto rect2 = CCRect(rect.origin.x, rect.origin.y + spr->m_size.height + 2, rect.size.width, rect.size.height - spr->m_size.height - 2);
                if (rect1.size.width > 0 && rect1.size.height > 0) freeRects.push_back(rect1);
                if (rect2.size.width > 0 && rect2.size.height > 0) freeRects.push_back(rect2);

                // Prune rects
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

            // Was the spritesheet able to generate?
            if (!success) continue;

            // Once we have run the algorithm, test to see if the area is smaller
            int area = size.width * size.height;
            if (area < bestArea) {
                bestArea = area;
                bestWidth = width;
            } else if (width == bestWidth) break; // We just went back and generated the best spritesheet, so the algorithm is done

            // Once we have reached the end of the loop, go back to the best width to generate the best spritesheet
            if (width >= maxWidth) width = bestWidth - 60;
        } // Main loop

        // Create the spritesheet object
        auto sheet = new CustomObjectsSheet();
        sheet->m_sprites = CCArray::create();
        sheet->m_sheetSize = size;
        sheet->autorelease();

        // Add the finished sprites to the sheet
        for (CustomObjectSprite* spr : sprites) sheet->m_sprites->addObject(spr);

        // Calculate space efficiency
        float efficiency = totalArea / (size.width * size.height);
        log::info("Found the smallest spritesheet with dimensions {} x {}", size.width, size.height);
        log::info("Generated the spritesheet with {}% space efficiency", std::min(efficiency * 100, 100.0f));

        // Return the final spritesheet object
        return sheet;
    } // create

    // Comparison used by the MaxRects algorithm to score the free rects
    static bool compareSprites(CustomObjectSprite* sprA, CustomObjectSprite* sprB, CCRect a, CCRect b) {
        if (sprA->bssf(a) != sprB->bssf(b)) return sprA->bssf(a) < sprB->bssf(b);
        else if (sprA->baf(a) != sprB->baf(b)) return sprA->baf(a) < sprB->baf(b);
        else return sprA->distance(a) < sprB->distance(b);
    } // compareSprites
};