#include "data/CustomSpriteConfig.hpp"
#include "data/CustomObjectConfig.hpp"
#include "data/CustomObjectsMod.hpp"

using namespace geode::prelude;

CustomSpriteConfig::CustomSpriteConfig(const CustomObjectsMod* mod, const CustomObjectConfigBase* object, std::string frame, int offsetX, int offsetY, int sizeW, int sizeH) : 
    frameName(""), sourceFrame(frame), mod(mod), object(object), offset(CCPoint(offsetX, offsetY)), size(CCSize(sizeW, sizeH)) {}

CustomSpriteConfig& CustomSpriteConfig::operator=(const CustomSpriteConfig& spr) {
    set(spr.sourceFrame, spr.offset.x, spr.offset.y, spr.size.width, spr.size.height);
    return *this;
} // operator=

CustomSpriteConfig& CustomSpriteConfig::set(std::string frame, int offsetX, int offsetY, int width, int height) {
    sourceFrame = frame;
    offset = CCPoint(offsetX, offsetY);
    size = CCSize(width, height);
    return *this;
} // set

std::string CustomSpriteConfig::getModID() const {
    return mod ? mod->getModID() : "";
} // getModID

std::string CustomSpriteConfig::getFrameName() const {
    return frameName.empty() ? sourceFrame : frameName;
} // getFrameName

std::string CustomSpriteConfig::getSourceFrame() const {
    return sourceFrame;
} // getSourceFrame

CCPoint CustomSpriteConfig::getOffset() const {
    return offset;
} // getOffset

CCSize CustomSpriteConfig::getSize() const {
    return size;
} // getSize

bool CustomSpriteConfig::isCustomSprite() const {
    return frameName != sourceFrame && !sourceFrame.empty();
} // isCustomSprite

bool CustomSpriteConfig::isAnimationFrame() const {
    return sourceFrame.find("_001") != std::string::npos;
} // isAnimationFrame

bool CustomSpriteConfig::isModTrigger() const {
    return object && object->getObjectID() == mod->getBaseObjectID();
} // isModTrigger

void CustomSpriteConfig::generateFrame() {
    if (sourceFrame.empty()) return;

    auto sourceFrameName = sourceFrame.substr(sourceFrame.find("/") + 1);
    auto rectString = fmt::format("{}.{}.{}.{}", offset.x, offset.y, size.width, size.height);
    frameName = fmt::format("custom-objects/{}/{}/{}", getModID(), rectString, sourceFrameName);
} // generateFrame