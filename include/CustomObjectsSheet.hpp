#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

enum Quality {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 4
};

struct CustomObjectSprite : public CCObject {
    gd::string m_frame;
    gd::string m_sourceFrame;
    CCPoint m_pos;
    CCSize m_size;
    bool m_rotated;

    CustomObjectSprite(CCSize size) {
        this->m_frame = "";
        this->m_sourceFrame = "";
        this->m_pos = CCPoint(0, 0);
        this->m_size = size;
        this->m_rotated = false;
        this->autorelease();
    } // CustomObjectSprite

    CustomObjectSprite(gd::string spr, gd::string mod, CCSize size, Quality quality) {
        this->m_frame = "custom-objects" + fmt::format("/{}/{}/", size.width, size.height) + spr;
        this->m_sourceFrame = fmt::format("{}/{}", mod, spr);
        this->m_pos = CCPoint(0, 0);
        this->m_size = size * quality;
        this->m_rotated = false;
        this->autorelease();
    } // CustomObjectSprite

    gd::string getSizeString() const {
        auto rotSize = m_rotated ? CCSize(m_size.height, m_size.width) : m_size;
        return "{" + fmt::format("{},{}", rotSize.width, rotSize.height) + "}";
    } // getSize

    gd::string getRectString() const {
        return "{{" + fmt::format("{},{}", m_pos.x, m_pos.y) + "}," + getSizeString() + "}";
    } // getRect

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

    gd::string getSizeString() const {
        return "{" + fmt::format("{},{}", m_sheetSize.width, m_sheetSize.height) + "}";
    } // getSizeString

    CCImage* createSpritesheetImage() const;
    CCDictionary* createSpritesheetData(gd::string name) const;
    static CustomObjectsSheet* create(CCArray* objects, Quality quality);

private:
    static CCSize maxRectsBinPacking(std::vector<CustomObjectSprite*> &sprites, CCSize binSize);
    static bool scoreRects(CustomObjectSprite* sprA, CustomObjectSprite* sprB, CCRect a, CCRect b);
};