#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <alphalaneous.editortab_api/include/EditorTabAPI.hpp>

#include "../manager/CustomObjectsManager.hpp"
#include "CustomObjectConfig.hpp"

using namespace geode::prelude;

class $modify(EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;
        if (CustomObjectsManager::get()->getTotalCustomObjectsCount() == 0) return true;

        // if (!Mod::get()->getSavedValue<bool>("editortab-api-enabled")) return true;
        // if (!Mod::get()->getSettingValue<bool>("editor-tab")) return true;

        alpha::editor_tabs::addTab("custom-objects"_spr, alpha::editor_tabs::BUILD, [this] {
            std::vector<Ref<CCNode>> buttons;
            for (auto [mod, objs] : CustomObjectsManager::get()->getEditorTabLayout())
                for (auto [p, id] : objs) buttons.push_back(static_cast<CCNode*>(getCreateBtn(id, 4)));
            return alpha::editor_tabs::createEditButtonBar(buttons);
        }, [] {
            return CCSprite::create("tab-icon.png"_spr);
        });

        return true;
    }

    int getSelectedObjectID() {
        if (m_selectedObject) return m_selectedObject->m_objectID;
        else if (m_selectedObjects && m_selectedObjects->count() > 0) {
            int id = static_cast<GameObject*>(m_selectedObjects->firstObject())->m_objectID;
            for (int i = 1; i < m_selectedObjects->count(); i++) {
                auto obj = static_cast<GameObject*>(m_selectedObjects->objectAtIndex(i));
                if (!obj || id != obj->m_objectID) return 0;
            }
            return id;
        } else return 0;
    }

    bool editButtonUsable() {
        if (int objectID = getSelectedObjectID(); objectID >= BASE_OBJECT_ID) {
            auto obj = CustomObjectsManager::get()->getCustomObjectByID(objectID);
            if (obj && obj->hasEditObjectCallback()) return true;
        }
        return EditorUI::editButtonUsable();
    }

    bool editButton2Usable() {
        if (int objectID = getSelectedObjectID(); objectID >= BASE_OBJECT_ID) {
            auto obj = CustomObjectsManager::get()->getCustomObjectByID(objectID);
            if (obj && obj->hasEditSpecialCallback()) return true;
        }
        return EditorUI::editButton2Usable();
    }

    void editObject(CCObject* p0) {
        if (int objectID = getSelectedObjectID(); objectID >= BASE_OBJECT_ID)
            if (CustomObjectsManager::get()->customEditObjectForID(objectID, m_selectedObject, m_selectedObjects)) return;
        EditorUI::editObject(p0);
    }

    void editObjectSpecial(int p0) {
       if (int objectID = getSelectedObjectID(); objectID >= BASE_OBJECT_ID)
            if (CustomObjectsManager::get()->customEditSpecialForID(objectID, m_selectedObject, m_selectedObjects)) return;
        EditorUI::editObjectSpecial(p0);
    }

    // CCPoint offsetForKey(int id) {
    //     if (id < 10000) return EditorUI::offsetForKey(id);
    //     return CustomObjectsManager::get()->getCustomObjectByID(id)->objectOffset;
    // } // offsetForKey
};