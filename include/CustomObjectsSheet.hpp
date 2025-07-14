#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct CustomObjectSprite : public CCObject {
    gd::string m_frame;
    CCPoint m_pos;
    CCSize m_size;
    bool m_rotated;

    CustomObjectSprite(gd::string spr, gd::string mod) {
        this->m_frame = fmt::format("{}/{}", mod, spr);
        this->m_pos = CCPoint(9999, 9999);
        this->m_size = CCSize(0, 0);
        this->m_rotated = false;
        this->autorelease();
    } // CustomObjectSprite

    CustomObjectSprite(CustomObjectSprite* spr, CCPoint pos) {
        this->m_frame = spr->m_frame;
        this->m_pos = pos;
        this->m_size = spr->m_size;
        this->m_rotated = spr->m_rotated;
        this->autorelease();
    } // CustomObjectSprite

    bool inBounds(CCSize bounds) {
        return m_pos.x + m_size.width <= bounds.width && m_pos.y + m_size.height <= bounds.height;
    } // inBounds

    int distance() {
        return m_pos.x + m_pos.y;
    } // distance

    int area() {
        return m_size.width * m_size.height;
    } // area

    int areaInside(CCSize bounds) {
        float leftEdge = std::max(bounds.width - m_pos.x, 0.0f);
        float rightEdge = std::max(bounds.width - m_pos.x - m_size.width, 0.0f);

        float topEdge = std::max(bounds.height - m_pos.y, 0.0f);
        float bottomEdge = std::max(bounds.height - m_pos.y - m_size.height, 0.0f);

        return (leftEdge - rightEdge) * (topEdge - bottomEdge);
    } // areaInside

    // Does the sprite intersect any other sprite?
    bool intersectsAny(std::vector<CustomObjectSprite*> objs) {
        auto rect = CCRect(m_pos, m_size);
        for (CustomObjectSprite* obj : objs) {
            auto objRect = CCRect(obj->m_pos, obj->m_size);
            if (rect.intersectsRect(objRect)) return true;
        } // for
        return false;
    } // intersectsAny
};

class CustomObjectsSheet : public CCNode {
public:
    CCArray* m_sprites;
    CCSize m_sheetSize;

    void printSprites() {
        for (int i = 0; i < m_sprites->count(); i++) {
            auto spr = static_cast<CustomObjectSprite*>(m_sprites->objectAtIndex(i));
            log::info("\"{}\": ({} | {})", spr->m_frame, spr->m_pos, spr->m_size);
        } // for
    } // printSprites

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

    static CustomObjectsSheet* create(CCArray* objects) {
        auto unfinished = std::vector<CustomObjectSprite*>(objects->count());
        auto finished = std::vector<CustomObjectSprite*>(0);

        std::vector<CCPoint> points{CCPoint(0, 0)};
        CCSize size = CCSize(0, 0);

        // Initialize unfinished vector
        float area = 0;
        for (int i = 0; i < objects->count(); i++) {
            auto obj = static_cast<ModCustomObject*>(objects->objectAtIndex(i));
            auto spr = new CustomObjectSprite(obj->m_spr, obj->m_mod);
            spr->m_size = obj->m_spriteSize * 4;
            unfinished[i] = spr;
            area += spr->m_size.width * spr->m_size.height;
        } // for

        // Calculate the minimum size
        int sideLength = std::ceil(std::sqrt(area));
        auto minSize = CCSize(sideLength, sideLength);

        // Main loop
        while (unfinished.size() > 0) {

            // Initialize variables
            bool inBounds = false;
            int bestPointIndex = 0;
            int bestSpriteIndex = 0;
            CustomObjectSprite* bestChoice = new CustomObjectSprite("","");

            // Choose the "best" sprite and point
            for (int s = 0; s < unfinished.size() * 2; s++) for (int p = 0; p < points.size(); p++) {
                CustomObjectSprite* choice = new CustomObjectSprite(unfinished[s / 2], points[p]);
                if (s % 2 == 1) {
                    if (choice->m_size.width == choice->m_size.height) continue;
                    choice->m_rotated = true;
                    choice->m_size.swap();
                } // if

                // Check if the current choice intersects any finished sprites
                if (choice->intersectsAny(finished)) continue;

                // Check if the current choice is in bounds
                bool isTheChoiceInBounds = choice->inBounds(size);
                if (isTheChoiceInBounds && !inBounds) goto newBest;
                else if (!isTheChoiceInBounds && inBounds) continue;

                // Check if the current choice fills more area inside the bounds
                if (choice->areaInside(size) > bestChoice->areaInside(size)) goto newBest;
                else if (choice->areaInside(size) < bestChoice->areaInside(size)) continue;

                // Check if the current choice is closer to the origin
                if (choice->distance() < bestChoice->distance()) goto newBest;
                else if (choice->distance() > bestChoice->distance()) continue;

                // Check if the current choice is larger
                if (choice->area() > bestChoice->area()) goto newBest;
                else continue;

                newBest:
                bestPointIndex = p;
                bestSpriteIndex = s / 2;
                bestChoice = choice;
                inBounds = isTheChoiceInBounds;
            } // for

            // Move the sprite from unfinished to finished and remove the point
            unfinished.erase(unfinished.begin() + bestSpriteIndex);
            points.erase(points.begin() + bestPointIndex);
            finished.push_back(bestChoice);

            // Add the new points
            points.push_back(CCPoint(bestChoice->m_pos.x + bestChoice->m_size.width + 2, bestChoice->m_pos.y));
            points.push_back(CCPoint(bestChoice->m_pos.x + bestChoice->m_size.width + 2, bestChoice->m_pos.y + 2));
            points.push_back(CCPoint(bestChoice->m_pos.x + bestChoice->m_size.width + 2, bestChoice->m_pos.y + 4));
            points.push_back(CCPoint(bestChoice->m_pos.x, bestChoice->m_pos.y + bestChoice->m_size.height + 2));
            points.push_back(CCPoint(bestChoice->m_pos.x + 2, bestChoice->m_pos.y + bestChoice->m_size.height + 2));
            points.push_back(CCPoint(bestChoice->m_pos.x + 4, bestChoice->m_pos.y + bestChoice->m_size.height + 2));

            // Clean the points array
            points.erase(std::unique(points.begin(), points.end()), points.end());
            for (int p = 0; p < points.size();) {
                auto rect = CCRect(bestChoice->m_pos, bestChoice->m_size);
                if (rect.containsPoint(points[p])) points.erase(points.begin() + p);
                else p++;
            } // for

            // Expand the sheet size if the new sprite was out of bounds
            size.width = std::max(size.width, bestChoice->m_pos.x + bestChoice->m_size.width + 10);
            size.height = std::max(size.height, bestChoice->m_pos.y + bestChoice->m_size.height + 10);
        } // while

        // Create the sheet object
        auto sheet = new CustomObjectsSheet();
        sheet->m_sprites = CCArray::create();
        sheet->m_sheetSize = (size -= CCSize(10, 10));
        sheet->autorelease();

        // Add the finished sprites to the array
        for (CustomObjectSprite* spr : finished) sheet->m_sprites->addObject(spr);

        // Calculate efficiency
        float efficiency = (minSize.width * minSize.height) / (size.width * size.height);
        log::info("Generated the spritesheet with {}% efficiency", efficiency * 100);

        // Return the final sprite sheet object
        return sheet;
    } // create
};