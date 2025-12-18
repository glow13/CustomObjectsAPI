#pragma once
#include <Geode/Geode.hpp>

#include "data/CustomObjectConfig.hpp"
#include "data/CustomSheetSprite.hpp"

using namespace geode::prelude;

#define SPRITE_BUFFER 2

enum Quality : int;

class CustomObjectsSheet : public CCNode {
public:
    std::vector<CustomSheetSprite> spritesCache;
    rectpack2D::rect_wh sheetSize;
    Quality quality;

    bool saveSpritesheetImage(std::string name, std::string path) const;
    bool saveSpritesheetPlist(std::string name, std::string path) const;
    static CustomObjectsSheet* create(const std::vector<CustomSpriteConfig*> customSprites, Quality quality);

private:
    static rectpack2D::rect_wh binPacking(std::vector<CustomSheetSprite> &sprites);
};