#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

enum Quality {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 4
};

struct CustomObjectSprite : public CCObject {
    gd::string m_spr;
    gd::string m_mod;
    CCPoint m_pos;
    CCSize m_size;
    bool m_rotated;

    CustomObjectSprite(gd::string spr, gd::string mod, CCSize size) {
        this->m_spr = spr;
        this->m_mod = mod;
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

    CCImage* createSpritesheetImage() const;
    static CustomObjectsSheet* create(CCArray* objects, Quality scale);

private:
    static CCSize maxRectsBinPacking(std::vector<CustomObjectSprite*> &sprites, CCSize binSize);
    static bool scoreRects(CustomObjectSprite* sprA, CustomObjectSprite* sprB, CCRect a, CCRect b);
};