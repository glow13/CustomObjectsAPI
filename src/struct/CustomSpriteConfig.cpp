#include "struct/CustomSpriteConfig.hpp"

CustomSpriteConfig::CustomSpriteConfig() : frame(""), sourceFrame(""), mod(""), rect(CCRect(0, 0, 0, 0)), custom(true) {}
CustomSpriteConfig::CustomSpriteConfig(std::string frame, std::string mod, CCRect size) : sourceFrame(frame), mod(mod), rect(size), custom(true) {}

bool CustomSpriteConfig::isAnimationFrame() const { return sourceFrame.find("_001") != std::string::npos; }

void CustomSpriteConfig::generateFrame() {
    if (sourceFrame.empty()) return;
    auto frameName = sourceFrame.substr(sourceFrame.find("/") + 1);
    frame = fmt::format("custom-objects/{}/{}.{}.{}.{}/{}", mod, (int)rect.origin.x, (int)rect.origin.y, (int)rect.size.width, (int)rect.size.height, frameName);
} // generateFrame