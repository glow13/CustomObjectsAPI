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
};

class CustomObjectsSheet : public CCNode {
public:
    CCArray* m_sprites;
    CCSize m_sheetSize;

    CCImage* createSpritesheetImage() const;
    CCDictionary* createSpritesheetData(gd::string name) const;
    static CustomObjectsSheet* create(CCDictionary* objects, Quality quality);

private:
    static CCSize binPacking(std::vector<CustomObjectSprite*> &sprites);

    gd::string getSizeString() const {
        return "{" + fmt::format("{},{}", m_sheetSize.width, m_sheetSize.height) + "}";
    } // getSizeString
};