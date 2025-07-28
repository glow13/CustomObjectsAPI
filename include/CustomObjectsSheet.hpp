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
    gd::string frame;
    gd::string sourceFrame;
    CCPoint pos;
    CCSize size;
    bool rotated;

    CustomObjectSprite() : frame(""), sourceFrame(""), pos(CCPoint(0, 0)), size(CCSize(30, 30)), rotated(false) {}
    CustomObjectSprite(gd::string sourceFrame, CCSize size, Quality quality) : sourceFrame(sourceFrame), pos(CCPoint(0, 0)), size(size * quality), rotated(false) {
        this->frame = fmt::format("custom-objects/{}/{}/", size.width, size.height) + sourceFrame.substr(sourceFrame.find("/") + 1);
    } // CustomObjectSprite

    gd::string getSizeString() const {
        auto rotSize = rotated ? CCSize(size.height, size.width) : size;
        return "{" + fmt::format("{},{}", rotSize.width, rotSize.height) + "}";
    } // getSize

    gd::string getRectString() const {
        return "{{" + fmt::format("{},{}", pos.x, pos.y) + "}," + getSizeString() + "}";
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