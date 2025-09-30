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

    CustomSpriteConfig() : frame(""), sourceFrame(""), mod(""), size(CCSize(0, 0)), custom(true) {}
    CustomSpriteConfig(std::string frame, std::string mod, CCSize size) : sourceFrame(frame), mod(mod), size(size), custom(true) {}

    operator bool() const { return !frame.empty() && (!custom || !size.isZero()); }
    const char* c_str() const { return frame.c_str(); }

    void generateFrame() {
        if (sourceFrame.empty()) return;
        auto frameName = sourceFrame.substr(sourceFrame.find("/") + 1);
        frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, (int)size.width, (int)size.height, frameName);

        if (!size.isZero()) return;
        auto frames = CCSpriteFrameCache::sharedSpriteFrameCache()->m_pSpriteFrames;
        auto source = static_cast<CCSpriteFrame*>(frames->objectForKey(sourceFrame));
        if (source) size = source->getOriginalSize();
    } // generateFrame
};