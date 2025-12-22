#include "data/CustomObjectConfig.hpp"
#include "data/CustomObjectsMod.hpp"

CustomObjectConfigBase::CustomObjectConfigBase(CustomObjectsMod* mod, int id) : mod(mod), objectID(id) {}

std::string CustomObjectConfigBase::getMainSprite() const { return mainSprite.getFrameName(); }
std::string CustomObjectConfigBase::getDetailSprite() const { return detailSprite.getFrameName(); }
std::string CustomObjectConfigBase::getGlowSprite() const { return glowSprite.getFrameName(); }

bool CustomObjectConfigBase::hasMainSprite() const { return !mainSprite.getSourceFrame().empty(); }
bool CustomObjectConfigBase::hasDetailSprite() const { return !detailSprite.getSourceFrame().empty(); }
bool CustomObjectConfigBase::hasGlowSprite() const { return !glowSprite.getSourceFrame().empty(); }

std::string CustomObjectConfigBase::getModID() const { return mod->getModID(); }
int CustomObjectConfigBase:: getObjectID() const { return objectID; }
CCSize CustomObjectConfigBase::getBoxSize() const { return boxSize; }
CCPoint CustomObjectConfigBase::getBoxOffset() const { return boxOffset; }
int CustomObjectConfigBase::getBoxRadius() const { return boxRadius; }
CCPoint CustomObjectConfigBase::getCreateOffset() const { return createOffset; }
GameObjectType CustomObjectConfigBase::getObjectType() const { return objectType; }
int CustomObjectConfigBase::getBatchMode() const { return batchMode; }
bool CustomObjectConfigBase::isBatchRenderDisabled() const { return disableBatch; }
int CustomObjectConfigBase::getFramesCount() const { return framesCount; }
float CustomObjectConfigBase::getFrameTime() const { return frameTime; }
ccColor3B CustomObjectConfigBase::getGlowColor() const { return glowColor; }
ccColor3B CustomObjectConfigBase::getParticleColor() const { return particleColor; }
GLubyte CustomObjectConfigBase::getParticleOpacity() const { return particleOpacity; }
int CustomObjectConfigBase::getEditorTabPriority() const { return editorPriority; }

bool CustomObjectConfigBase::isCustomBatch() const { return !disableBatch && batchMode == BATCH_MODE_DEFAULT; }
bool CustomObjectConfigBase::hasCustomAnimation() const { return framesCount != FRAMES_COUNT_DEFAULT && mainSprite.isAnimationFrame(); }