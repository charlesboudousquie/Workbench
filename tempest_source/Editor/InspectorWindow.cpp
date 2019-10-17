/*!***************************************************************************************
\file       InspectorWindow.cpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the inspector window class
*****************************************************************************************/
#include "InspectorWindow.hpp"
#include "HierarchyWindow.hpp"
#include "EditorWindow.hpp"
#include "Reflection.hpp"
#include "../Editor/EngineController.hpp"
#include <EngineRunner.hpp>
#include <Engine.hpp>

#include "ObjectCommand.hpp"
#include "UndoRedo.hpp"

Editor::inspectorWindow::inspectorWindow(editorWindow * p_parent_editor)
	: windowBase(p_parent_editor),
	m_renderer(p_parent_editor)
{
	m_renderer.setRemoveHandler([&](std::string p_component_name)
	{
		//get game object id
		objID l_selection_id = getSelectionKeeper().getSelectionId();
		getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->removeGameObjectComponent(l_selection_id, p_component_name);
	});
}

void Editor::inspectorWindow::onRender()
{
	if (getSelectionKeeper().isNoneSelected())
		return;
	objID l_selection_id = getSelectionKeeper().getSelectionId();

	typeRT l_type_data = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->getTypeRT(l_selection_id);

	//ImGui::Text("Selection Type: %i", l_selection_type);
	//ImGui::Text("Selection ID: %u", l_selection_id);

	// update filter

	// display the typeRT structure and record any changes into the l_newData parameter.

  auto l_selection_keeper = getSelectionKeeper();

  if (l_selection_keeper.isSceneSelected())
  {
    if (m_renderer.renderScene(l_type_data))
    {
      // ship back the changes made.
      getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->applyTypeRT(l_selection_id, l_type_data);
    }
  }
  else if (l_selection_keeper.isSpaceSelected())
  {
    if (m_renderer.renderSpace(l_type_data))
    {
      // ship back the changes made.
      getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->applyTypeRT(l_selection_id, l_type_data);
    }
  }
  else if(l_selection_keeper.isGameObjectSelected())
  {
    if (m_renderer.renderGameObject(l_type_data))
    {
      // ship back the changes made.
      getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->applyTypeRT(l_selection_id, l_type_data);
    }
  }

	// display hierarchy menu
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Create"))
		{
			auto l_component_types = getEngineController().getEngineRunner()->getEngine()->getEngineMetadataManipulator().lock()->getComponentTypes();

			// loop over all component types from the engine
			for(auto l_comp_type : l_component_types)
			{
				// create a menu item if the editor should be able to add a type of the component
				if(l_comp_type.allowEditorAdd)
				{
					if(ImGui::MenuItem(l_comp_type.friendlyName.c_str()))
					{
                        //record object state before change
                        std::shared_ptr<Command> newCommand = std::make_shared<ObjectDataChangeCommand>();
                        newCommand->Init(&getEngineController(), getSelectionKeeper().getSelectionId());

						// menu item clicked, request engine to add the component
						getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->addGameObjectComponent(l_selection_id,
							l_comp_type.name);

                        //record object state after change for undo redo purposes
                        newCommand->Record();
                        UndoRedoManager::GetInstance().RecordState(newCommand, &getEngineController());
					}
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

