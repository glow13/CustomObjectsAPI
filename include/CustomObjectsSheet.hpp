#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct CustomObjectSprite : public CCObject {
    gd::string m_frame;
    CCPoint m_pos;
    CCSize m_size;

    CustomObjectSprite(gd::string spr, gd::string mod) {
        gd::string frame = fmt::format("{}/{}", mod, spr);
        this->m_frame = frame;
        this->m_pos = CCPoint(0, 0);
        this->m_size = CCSize(120, 120);
        this->autorelease();
    } // CustomObjectSprite

    CustomObjectSprite(CCPoint pos, CCSize size) {
        this->m_pos = pos;
        this->m_size = size;
        this->autorelease();
    } // CustomObjectSprite

    // Is the sprite in bounds?
    bool inBounds(CCSize bounds) {
        return m_pos.x + m_size.width <= bounds.width && m_pos.y + m_size.height <= bounds.height;
    } // inBounds

    // Is the sprite larger than the other sprite?
    bool larger(CustomObjectSprite* other) {
        int size = m_size.width * m_size.height;
        int sizeOther = other->m_size.width * other->m_size.height;
        return size > sizeOther;
    } // larger

    // Is the sprite closer to the origin than the other sprite?
    bool closer(CustomObjectSprite* other) {
        float dist = std::sqrt(std::pow(m_pos.x + m_size.width, 2) + std::pow(m_pos.y + m_size.height, 2));
        float distOther = std::sqrt(std::pow(other->m_pos.x + other->m_size.width, 2) + std::pow(other->m_pos.y + other->m_size.height, 2));
        return dist < distOther;
    } // closer

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
            auto objSprite = static_cast<CustomObjectSprite*>(m_sprites->objectAtIndex(i));
            float y = m_sheetSize.height - objSprite->m_pos.y - objSprite->m_size.height;

            auto spr = CCSprite::createWithSpriteFrameName(objSprite->m_frame.c_str());
            spr->setPosition(CCPoint(objSprite->m_pos.x / 4, y / 4));
            spr->setAnchorPoint(CCPoint(0, 0));
            spr->visit();
        } // for

        render->end();
        return render;
    } // generateSpritesheet

    // TODO still needs some work :(
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
            CustomObjectSprite* bestChoice = new CustomObjectSprite(points[0], CCSize(0, 0));

            // Choose the best sprite and point
            for (int s = 0; s < unfinished.size(); s++) for (int p = 0; p < points.size(); p++) {
                CustomObjectSprite* choice = new CustomObjectSprite(points[p], unfinished[s]->m_size);

                // Check if the current choice intersects any finished sprites
                if (choice->intersectsAny(finished)) continue;

                // Check if the current choice is in bounds
                bool isTheChoiceInBounds = choice->inBounds(size);
                if (!isTheChoiceInBounds && inBounds) continue;

                // Check if the current choice is better
                if ((isTheChoiceInBounds && !inBounds) || choice->larger(bestChoice) || choice->closer(bestChoice)) {
                    inBounds = isTheChoiceInBounds;
                    bestPointIndex = p;
                    bestSpriteIndex = s;
                    bestChoice = unfinished[s];
                    bestChoice->m_pos = points[p];
                } // if
            } // for

            // Move the sprite from unfinished to finished and remove the point
            unfinished.erase(unfinished.begin() + bestSpriteIndex);
            points.erase(points.begin() + bestPointIndex);
            finished.push_back(bestChoice);

            // Add the new points
            points.push_back(CCPoint(bestChoice->m_pos.x + bestChoice->m_size.width + 2, bestChoice->m_pos.y));
            points.push_back(CCPoint(bestChoice->m_pos.x, bestChoice->m_pos.y + bestChoice->m_size.height + 2));

            // Clean the points array
            points.erase(std::unique(points.begin(), points.end()), points.end());
            for (int p = 0; p < points.size();) {
                auto rect = CCRect(bestChoice->m_pos, bestChoice->m_size);
                if (rect.containsPoint(points[p])) points.erase(points.begin() + p);
                else p++;
            } // for

            // Expand the sheet size if the new sprite was out of bounds
            if (!inBounds) {
                size.width = std::max(size.width, bestChoice->m_pos.x + bestChoice->m_size.width);
                size.height = std::max(size.height, bestChoice->m_pos.y + bestChoice->m_size.height);
            } // if
        } // while

        // Create the sheet object
        auto sheet = new CustomObjectsSheet();
        sheet->m_sprites = CCArray::create();
        sheet->m_sheetSize = size;
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