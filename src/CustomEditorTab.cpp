#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

#include <alphalaneous.editortab_api/include/EditorTabs.hpp>

#include "CustomObjectsAPI.hpp"

using namespace geode::prelude;

class $modify(EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        EditorTabs::addTab(this, TabType::BUILD, "custom-objects"_spr, [this](EditorUI* ui, CCMenuItemToggler* toggler) -> CCNode* {
            CCLabelBMFont* textLabel = CCLabelBMFont::create("+", "bigFont.fnt");
            textLabel->setScale(0.5f);
            EditorTabUtils::setTabIcon(toggler, textLabel);

            auto buttons = CCArray::create();
            auto manager = CustomObjectsManager::get();
            manager->forEachCustomObject([this, buttons](auto obj) {
                buttons->addObject(getCreateBtn(obj.m_id, 4));
            });
 
            return EditorTabUtils::createEditButtonBar(buttons, ui);
        });

        return true;
    } // init
};