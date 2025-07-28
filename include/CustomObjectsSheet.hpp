#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct ModCustomObject;

enum Quality {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 4
};

struct CustomObjectSprite {
    gd::string m_frame;
    gd::string m_sourceFrame;
    CCPoint m_pos;
    CCSize m_size;
    bool m_rotated;

    CustomObjectSprite() : m_frame(""), m_sourceFrame(""), m_pos(CCPoint(0, 0)), m_size(CCSize(30, 30)), m_rotated(false) {}

    CustomObjectSprite(gd::string spr, gd::string mod, CCSize size, Quality quality) : CustomObjectSprite() {
        m_frame = "custom-objects" + fmt::format("/{}/{}/", size.width, size.height) + spr;
        m_sourceFrame = fmt::format("{}/{}", mod, spr);
        m_size = size * quality;
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
    std::vector<CustomObjectSprite> spritesCache;
    CCSize sheetSize;

    CCImage* createSpritesheetImage() const;
    CCDictionary* createSpritesheetData(gd::string name) const;
    static CustomObjectsSheet* create(std::map<int, ModCustomObject> objects, Quality quality);

private:
    static CCSize binPacking(std::vector<CustomObjectSprite> &sprites);
    gd::string getSizeString() const { return "{" + fmt::format("{},{}", sheetSize.width, sheetSize.height) + "}"; }
};