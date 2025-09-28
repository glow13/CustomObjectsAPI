#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#define SPRITE1(frame) SPRITE3(frame, 30, 30)
#define SPRITE2(frame, size) SPRITE3(frame, size, size)
#define SPRITE3(frame, w, h) "custom-objects/" + Mod::get()->getID() + "/"#w"/"#h"/" frame
#define $sprite(...) GEODE_INVOKE(GEODE_CONCAT(SPRITE, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__)

struct CustomSpriteConfig {
public:
    std::string mod;
    std::string frame;
    std::string sourceFrame;
    CCSize size;

    CustomSpriteConfig() {}
    CustomSpriteConfig(std::string frame, std::string mod, CCSize size) : sourceFrame(frame), mod(mod), size(size) {}

    operator std::string() const { return frame; }
    operator const char*() const { return frame.c_str(); }
    operator bool() const { return !frame.empty(); }

    bool isCustomRender() const { return frame == sourceFrame; }

    void generateFrame() {
        if (sourceFrame.empty()) return;
        auto frameName = sourceFrame.substr(sourceFrame.find("/") + 1);
        frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, size.width, size.height, frameName);
    } // generateFrame
};