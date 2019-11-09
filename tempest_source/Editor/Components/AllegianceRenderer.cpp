/*!***************************************************************************************
\file       AllegianceRenderer.cpp
\author     Charles Boudousquie
\date       11/5/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Renders the allegiance component.
*****************************************************************************************/
#include "AllegianceRenderer.hpp"
#include <imgui.h>

#include "Allegiance.hpp"
#include <map>

static std::map<const char*, Allegiance::Faction> factions =
{
    {"PLAYER", Allegiance::Faction::PLAYER},
    {"PLAYER_MINION", Allegiance::Faction::PLAYER_MINION},
    {"ENEMY", Allegiance::Faction::ENEMY},
    {"NONE", Allegiance::Faction::NONE}
};

Editor::allegianceRenderer::allegianceRenderer(editorWindow * p_parent_window) 
    : componentRenderer(p_parent_window)
{
}

bool Editor::allegianceRenderer::onRender(typeRT & p_type_data, objID p_editor_object_id)
{

    // create imgui combo
    if (ImGui::BeginMenu("Factions"))
    {

        for (auto faction : factions)
        {
            if (ImGui::MenuItem(faction.first))
            {
                p_type_data.member("faction").setInt((int)faction.second);
            }
        }

        ImGui::EndMenu();
    }

    return true;
}
