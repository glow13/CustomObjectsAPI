#pragma once
#include <Geode/Geode.hpp>

#include "struct/CustomObject.hpp"
#include "struct/CustomObjectSprite.hpp"

using namespace geode::prelude;

class CustomObjectsSheet : public CCNode {
public:
    std::vector<CustomObjectSprite> spritesCache;
    CCSize sheetSize;

    bool saveSpritesheetImage(std::string name, std::string path) const;
    bool saveSpritesheetPlist(std::string name, std::string path) const;
    static CustomObjectsSheet* create(std::map<int, CustomObject> objects, Quality quality);

private:
    static CCSize binPacking(std::vector<CustomObjectSprite> &sprites);
    std::string sizeString() const { return "{" + fmt::format("{},{}", sheetSize.width, sheetSize.height) + "}"; }
};