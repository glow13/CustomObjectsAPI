#pragma once
#include <Geode/Geode.hpp>
#include <rect_structs.h>

using namespace geode::prelude;

struct CustomSpriteConfig;
struct CustomSheetSprite;

enum Quality : int;

class CustomObjectsSheet final {
public:
    std::vector<CustomSheetSprite> spritesCache;
    rectpack2D::rect_wh sheetSize;
    Quality quality;

    CustomObjectsSheet(const std::vector<CustomSpriteConfig*> customSprites, Quality quality);

    bool saveSpritesheetImage(std::string name, std::string path) const;
    bool saveSpritesheetPlist(std::string name, std::string path) const;

private:
    static rectpack2D::rect_wh binPacking(std::vector<CustomSheetSprite> &sprites);
};