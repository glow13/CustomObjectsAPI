#include "data/CustomSpriteConfig.hpp"

CustomSpriteConfig::CustomSpriteConfig() : frame(""), sourceFrame(""), modId(""), rect(CCRect(0, 0, 0, 0)), custom(true), mod(false) {}
CustomSpriteConfig::CustomSpriteConfig(std::string frame, std::string modId, CCRect size) : sourceFrame(frame), modId(modId), rect(size), custom(true), mod(false) {}

bool CustomSpriteConfig::isAnimationFrame() const { return sourceFrame.find("_001") != std::string::npos; }
bool CustomSpriteConfig::isModTrigger() const { return mod; }

void CustomSpriteConfig::generateFrame() {
    if (sourceFrame.empty()) return;
    auto frameName = sourceFrame.substr(sourceFrame.find("/") + 1);
    frame = fmt::format("custom-objects/{}/{}.{}.{}.{}/{}", modId, (int)rect.origin.x, (int)rect.origin.y, (int)rect.size.width, (int)rect.size.height, frameName);
} // generateFrame