#include "data/CustomObjectConfig.hpp"
#include "data/CustomObjectsMod.hpp"

using namespace geode::prelude;

CustomObjectConfigBase::CustomObjectConfigBase(CustomObjectsMod* mod, int id) : mod(mod), objectID(id) {}

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