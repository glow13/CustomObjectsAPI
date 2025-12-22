#include "data/CustomSpriteConfig.hpp"
#include "data/CustomObjectConfig.hpp"
#include "data/CustomObjectsMod.hpp"

CustomSpriteConfig::CustomSpriteConfig(CustomObjectsMod* mod, CustomObjectConfigBase* object, std::string frame, int offX, int offY, int sizeW, int sizeH) : 
    frameName(""), sourceFrame(frame), mod(mod), object(object), offset(CCPoint(offX, offY)), size(CCSize(sizeW, sizeH)) {}

std::string CustomSpriteConfig::getModID() const { return mod->getModID(); }
std::string CustomSpriteConfig::getFrameName() const { return frameName.empty() ? sourceFrame : frameName; }
std::string CustomSpriteConfig::getSourceFrame() const { return sourceFrame; }
CCPoint CustomSpriteConfig::getOffset() const { return offset; }
CCSize CustomSpriteConfig::getSize() const { return size; }

bool CustomSpriteConfig::isCustomSprite() const { return frameName != sourceFrame && !sourceFrame.empty(); }
bool CustomSpriteConfig::isAnimationFrame() const { return sourceFrame.find("_001") != std::string::npos; }
bool CustomSpriteConfig::isModTrigger() const { return object && object->getObjectID() == mod->getBaseObjectID(); }

void CustomSpriteConfig::generateFrame() {
    if (sourceFrame.empty()) return;

    auto sourceFrameName = sourceFrame.substr(sourceFrame.find("/") + 1);
    auto rectString = fmt::format("{}.{}.{}.{}", offset.x, offset.y, size.width, size.height);
    frameName = fmt::format("custom-objects/{}/{}/{}", getModID(), rectString, sourceFrameName);
} // generateFrame