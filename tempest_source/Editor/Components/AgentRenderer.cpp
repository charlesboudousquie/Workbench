/*!***************************************************************************************
\file       AgentRenderer.cpp
\author     Charles Boudousquie
\date       10/1/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Renders the agent component.
*****************************************************************************************/
#include "AgentRenderer.hpp"
#include "../EngineController.hpp"
#include <EngineRunner.hpp>
#include <Engine.hpp>

#include <imgui.h>

Editor::agentRenderer::agentRenderer(editorWindow * p_parent_window)
    : componentRenderer(p_parent_window)
{
    
}

void Editor::agentRenderer::LoadTreeList()
{
    auto assetList = getEngineController().getEngineRunner()->getEngine()->getAssetManipulator().lock()->assetList();


    // fill up tree list, by filtering out irrelevant files
    for (auto asset : assetList)
    {
        auto string = getEngineController().getEngineRunner()->getEngine()->getAssetManipulator().lock()->stringFromWide(asset);

        auto extensionPos = string.find(".bht");
        if (extensionPos != std::string::npos)
        {
            treeList.push_back(string.c_str());
        }
    }
}

bool Editor::agentRenderer::onRender(typeRT & p_type_data)
{
    static bool firstTime = true;

    if (firstTime == true)
    {
        LoadTreeList();
        firstTime = false;
    }

    //static const char* currentTree = nullptr;


    // create imgui combo
    if (ImGui::BeginMenu("Trees"))
    {
        for (auto tree : treeList)
        {
            // user chooses what tree they want
            if (ImGui::MenuItem(tree))
            {
                // set tree name
                p_type_data.member("parentTree").setString(tree);
            }

        }

        ImGui::EndMenu();
    }

    

    return true;
}
