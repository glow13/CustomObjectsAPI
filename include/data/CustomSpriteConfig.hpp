#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#ifdef GEODE_MOD_ID

#define SPRITE1(name) SPRITE5(name, 0, 0, 0, 0)
#define SPRITE2(name, s) SPRITE5(name, 0, 0, s, s)
#define SPRITE3(name, w, h) SPRITE5(name, 0, 0, w, h)

#define SPRITE5(name,x,y,w,h) GEODE_CONCAT("custom-objects/",GEODE_MOD_ID)GEODE_STR(/x.y.w.h/)name##".png"
#define $sprite(...) (GEODE_INVOKE(GEODE_CONCAT(SPRITE,GEODE_NUMBER_OF_ARGS(__VA_ARGS__)),__VA_ARGS__))

#endif

class CustomObjectsManager;

struct CustomSpriteConfig {
public:
    std::string frame;
    std::string sourceFrame;
    std::string modId;
    CCRect rect;
    bool custom;
private:
    bool mod;
    friend CustomObjectsManager;
public:
    CustomSpriteConfig();
    CustomSpriteConfig(std::string frame, std::string modId, CCRect rect);

    inline operator gd::string() const { return frame; }
    inline operator const char*() const { return frame.c_str(); }
    inline operator bool() const { return !frame.empty(); }

    bool isAnimationFrame() const;
    bool isModTrigger() const;
    void generateFrame();
};