#include <Geode/Geode.hpp>
// #include "CustomObjectConfig.hpp"
// #include "CustomObjectBase.hpp"

using namespace geode::prelude;

$execute {
    if (auto editorTabMod = Loader::get()->getInstalledMod("alphalaneous.editortab_api")) {
        Mod::get()->setSavedValue("editortab-api-enabled", editorTabMod->isOrWillBeEnabled());
    } else Mod::get()->setSavedValue("editortab-api-enabled", false);
}