#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#define SPRITE1(frame) SPRITE5(frame, 0, 0, 0, 0)
#define SPRITE2(frame, s) SPRITE5(frame, 0, 0, s, s)
#define SPRITE3(frame, w, h) SPRITE5(frame, 0, 0, w, h)
#define SPRITE5(frame, x, y, w, h) "custom-objects/" + Mod::get()->getID() + "/"#x"."#y"."#w"."#h"/" frame
#define $sprite(...) (GEODE_INVOKE(GEODE_CONCAT(SPRITE, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__))

struct CustomSpriteConfig {
public:
    std::string frame;
    std::string sourceFrame;
    std::string mod;
    CCRect rect;
    bool custom;

    CustomSpriteConfig();
    CustomSpriteConfig(std::string frame, std::string mod, CCRect rect);

    inline operator gd::string() const { return frame; }
    inline operator const char*() const { return frame.c_str(); }
    inline operator bool() const { return !frame.empty(); }

    bool isAnimationFrame() const;
    void generateFrame();
};