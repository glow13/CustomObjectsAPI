#include "struct/CustomSpriteConfig.hpp"

CustomSpriteConfig::CustomSpriteConfig() : frame(""), sourceFrame(""), mod(""), size(CCSize(0, 0)), custom(true) {}
CustomSpriteConfig::CustomSpriteConfig(std::string frame, std::string mod, CCSize size) : sourceFrame(frame), mod(mod), size(size), custom(true) {}

bool CustomSpriteConfig::isAnimationFrame() const { return sourceFrame.find("_001") != std::string::npos; }

void CustomSpriteConfig::generateFrame() {
    if (sourceFrame.empty()) return;
    auto frameName = sourceFrame.substr(sourceFrame.find("/") + 1);
    frame = fmt::format("custom-objects/{}/{}/{}/{}", mod, (int)size.width, (int)size.height, frameName);
} // generateFrame