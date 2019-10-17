/*!***************************************************************************************
\file       AgentRenderer.hpp
\author     Charles Boudousquie
\date       10/1/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Renders the agent component.
*****************************************************************************************/
#pragma once
#include "ComponentRenderer.hpp"
#include <vector>

namespace Editor
{
    class agentRenderer : public componentRenderer
    {
    public:
        agentRenderer(editorWindow * p_parent_window);

    protected:
        bool onRender(typeRT & p_type_data) override;

    private:
        void LoadTreeList();

        std::vector<const char*> treeList;
    };
};
