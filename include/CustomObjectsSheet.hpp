#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct ModCustomObject;
enum Quality : unsigned int;

struct CustomObjectSprite {
    std::string frame;
    std::string sourceFrame;
    CCPoint pos;
    CCSize size;
    bool rotated;

    CustomObjectSprite() : frame(""), sourceFrame(""), pos(CCPoint(0, 0)), size(CCSize(30, 30)), rotated(false) {}
    CustomObjectSprite(std::string sourceFrame, CCSize size, Quality quality) : sourceFrame(sourceFrame), pos(CCPoint(0, 0)), size(size * quality), rotated(false) {
        auto mod = sourceFrame.substr(0, sourceFrame.find("/"));
        auto spr = sourceFrame.substr(sourceFrame.find("/") + 1);
        this->frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, size.width, size.height, spr);
    } // CustomObjectSprite

    std::string sizeString() const {
        auto rotSize = rotated ? CCSize(size.height, size.width) : size;
        return "{" + fmt::format("{},{}", rotSize.width, rotSize.height) + "}";
    } // sizeString

    std::string rectString() const {
        return "{{" + fmt::format("{},{}", pos.x, pos.y) + "}," + sizeString() + "}";
    } // rectString

    std::string rotatedString() const {
        return rotated ? "<true/>" : "<false/>";
    } // rotatedString
};

class CustomObjectsSheet : public CCNode {
public:
    std::vector<CustomObjectSprite> spritesCache;
    CCSize sheetSize;

    bool saveSpritesheetImage(std::string name, std::string path) const;
    bool saveSpritesheetPlist(std::string name, std::string path) const;
    static CustomObjectsSheet* create(std::map<int, ModCustomObject> objects, Quality quality);

private:
    static CCSize binPacking(std::vector<CustomObjectSprite> &sprites);
    std::string sizeString() const { return "{" + fmt::format("{},{}", sheetSize.width, sheetSize.height) + "}"; }
};