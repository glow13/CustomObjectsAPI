#include "data/CustomObjectConfig.hpp"
#include "data/CustomObjectsMod.hpp"

using namespace geode::prelude;

constexpr cocos2d::CCSize BOX_SIZE_DEFAULT = {0, 0};
constexpr cocos2d::CCPoint BOX_OFFSET_DEFAULT = {0, 0};
constexpr int BOX_RADIUS_DEFAULT = 0;
constexpr cocos2d::CCPoint OBJECT_OFFSET_DEFAULT = {0, 0};
constexpr GameObjectType OBJECT_TYPE_DEFAULT = (GameObjectType)(-1);
constexpr int BATCH_MODE_DEFAULT = 10;
constexpr bool DISABLE_BATCH_DEFAULT = false;
constexpr int FRAMES_COUNT_DEFAULT = 0;
constexpr float FRAME_TIME_DEFAULT = 1.0f;
constexpr cocos2d::ccColor3B GLOW_COLOR_DEFAULT = {255, 255, 255};
constexpr cocos2d::ccColor3B PARTICLE_COLOR_DEFAULT = {255, 255, 255};
constexpr GLubyte PARTICLE_OPACITY_DEFUALT = 255;
constexpr bool PARTICLE_BLENDING_DEFAULT = true;
constexpr int EDITOR_PRIORITY_DEFAULT = 0;

CustomObjectConfigBase::CustomObjectConfigBase(CustomObjectsMod* mod, int id) : 
    mod(mod),
    objectID(id),
    boxSize(BOX_SIZE_DEFAULT),
    boxOffset(BOX_OFFSET_DEFAULT),
    boxRadius(BOX_RADIUS_DEFAULT),
    objectOffset(OBJECT_OFFSET_DEFAULT),
    objectType(OBJECT_TYPE_DEFAULT),
    batchMode(BATCH_MODE_DEFAULT),
    disableBatch(DISABLE_BATCH_DEFAULT),
    framesCount(FRAMES_COUNT_DEFAULT),
    frameTime(FRAME_TIME_DEFAULT),
    glowColor(GLOW_COLOR_DEFAULT),
    particleColor(PARTICLE_COLOR_DEFAULT),
    particleOpacity(PARTICLE_OPACITY_DEFUALT),
    particleBlending(PARTICLE_BLENDING_DEFAULT),
    editorPriority(EDITOR_PRIORITY_DEFAULT) {}

std::string CustomObjectConfigBase::getModID() const { return mod->getModID(); }
std::string CustomObjectConfigBase::getModName() const { return mod->getModName(); }
int CustomObjectConfigBase::getObjectID() const { return objectID; }

std::string CustomObjectConfigBase::getMainSprite() const { return mainSprite.getFrameName(); }
std::string CustomObjectConfigBase::getDetailSprite() const { return detailSprite.getFrameName(); }
std::string CustomObjectConfigBase::getGlowSprite() const { return glowSprite.getFrameName(); }

bool CustomObjectConfigBase::hasMainSprite() const { return !mainSprite.getSourceFrame().empty(); }
bool CustomObjectConfigBase::hasDetailSprite() const { return !detailSprite.getSourceFrame().empty(); }
bool CustomObjectConfigBase::hasGlowSprite() const { return !glowSprite.getSourceFrame().empty(); }

bool CustomObjectConfigBase::isCustomBatch() const { return !disableBatch && batchMode == BATCH_MODE_DEFAULT; }
bool CustomObjectConfigBase::hasCustomAnimation() const { return framesCount != FRAMES_COUNT_DEFAULT && mainSprite.isAnimationFrame(); }

void CustomObjectConfigBase::applyConfigValues(GameObject* obj) const {
    obj->m_parentMode = 10;
    obj->m_objectID = objectID;

    if (boxSize != BOX_SIZE_DEFAULT) { obj->m_width = boxSize.width; obj->m_height = boxSize.height; }
    if (boxOffset != BOX_OFFSET_DEFAULT) obj->m_customBoxOffset = boxOffset;
    if (boxRadius != BOX_RADIUS_DEFAULT) obj->m_objectRadius = boxRadius;
    if (objectType != OBJECT_TYPE_DEFAULT) obj->m_objectType = objectType;
    if (batchMode != BATCH_MODE_DEFAULT) obj->m_parentMode = batchMode;
    if (disableBatch) { obj->m_parentMode = 4; obj->m_addToNodeContainer = true; }
    if (glowColor != GLOW_COLOR_DEFAULT) obj->setGlowColor(glowColor);
} // applyConfigValues