/*!***************************************************************************************
\file       HierarchyWindow.cpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the hierarchy window class
*****************************************************************************************/

#include "HierarchyWindow.hpp"
#include <imgui.h>
#include <memory>
#include <SceneManagement/Scene.hpp>
#include <SceneManagement/Space.hpp>
#include <SceneManagement/GameObject.hpp>
#include "EngineController.hpp"
#include "EditorLogger.hpp"
#include <EngineRunner.hpp>
#include <Engine.hpp>
#include "EditorWindow.hpp"

Editor::hierarchyWindow::hierarchyWindow(editorWindow * p_parent_editor)
	: windowBase( p_parent_editor), m_renderer(p_parent_editor)
{
}

void Editor::hierarchyWindow::onRender()
{
  Editor::EditorLogger & l_log = getLogger();
	// display hierarchy menu
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Create"))
		{
      if (ImGui::MenuItem("Empty Space"))
      {
        if(getSelectionKeeper().isSceneSelected())
        {
          auto l_return = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->addEmptySpace(getSelectionKeeper().getSelectionId());

          if(l_return != nullptr)
          {
            l_log.AddLog("[NOTICE] New Empty Space created in scene: %s \n", getSelectionKeeper().getSelectionName().c_str());

            getSelectionKeeper().setSelected(l_return);
          }
          else
          {
            l_log.AddLog("[ERROR] New Empty Space creation failed!\n");
          }

        }
        else
        {
          auto l_return = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->addEmptySpace();

          if (l_return != nullptr)
          {
            l_log.AddLog("[NOTICE] New Empty Space created.\n");

            getSelectionKeeper().setSelected(l_return);
          }
          else
          {
            l_log.AddLog("[ERROR] New Empty Space creation failed!\n");
          }
        }
      }
			if (ImGui::MenuItem("Empty Game Object"))
			{
        if(getSelectionKeeper().isNoneSelected() || getSelectionKeeper().isSceneSelected())
        {
          auto l_return = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->addEmptyGameObject();

          if (l_return != nullptr)
          {
            getSelectionKeeper().setSelected(l_return);

            l_log.AddLog("[NOTICE] New Empty Game Object created.\n");
          }
          else
          {
            l_log.AddLog("[ERROR] Game Object creation failed!\n");
          }
        }
        else
        {
          auto l_return = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->addEmptyGameObject(getSelectionKeeper().getSelectionId());

          if (l_return != nullptr)
          {
            l_log.AddLog("[NOTICE] New Empty Game Object created to parent: %s.\n", getSelectionKeeper().getSelectionName().c_str());

            getSelectionKeeper().setSelected(l_return);
          }
          else
          {
            l_log.AddLog("[ERROR] Game Object creation failed!\n");
          }
        }
			}

			ImGui::EndMenu();
		}
		// ...
		ImGui::EndMenuBar();
	}

  m_renderer.render();
}




