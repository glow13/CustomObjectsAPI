#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#define SPRITE1(frame) SPRITE3(frame, 0, 0)
#define SPRITE2(frame, size) SPRITE3(frame, size, size)
#define SPRITE3(frame, w, h) "custom-objects/" + Mod::get()->getID() + "/"#w"/"#h"/" frame
#define $sprite(...) GEODE_INVOKE(GEODE_CONCAT(SPRITE, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__)

struct CustomSpriteConfig {
public:
    std::string frame;
    std::string sourceFrame;
    std::string mod;
    CCSize size;
    bool custom;

    CustomSpriteConfig();
    CustomSpriteConfig(std::string frame, std::string mod, CCSize size);

    inline operator gd::string() const { return frame; }
    inline operator const char*() const { return frame.c_str(); }
    inline operator bool() const { return !frame.empty() && (!custom || !size.isZero()); }

    bool isAnimationFrame() const;
    CustomSpriteConfig generateAnimationFrame(int i) const;

    void generateFrame();
};