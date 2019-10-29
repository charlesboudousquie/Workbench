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

#include "EditorObjectManager.hpp"
#include "EditorObject.hpp"

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

  //Ask engine for typeRT data of current id
	//typeRT l_type_data = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->getTypeRT(l_selection_id);

  //Ask EditorObjectManager for typeRT data of current id
  EditorObjectManager & l_editor_object_manager = getTopWindow()->getSceneWindow().getEditorObjectManager();
  EditorObject * l_editor_object = l_editor_object_manager.getEditorObject(l_selection_id);
  if(l_editor_object != nullptr)
  {
    typeRT & l_type_data = l_editor_object->getData();

    //ImGui::Text("Selection Type: %i", l_selection_type);
    //ImGui::Text("Selection ID: %u", l_selection_id);

    // update filter

    // display the typeRT structure and record any changes into the l_newData parameter.

    auto l_selection_keeper = getSelectionKeeper();

    if (l_selection_keeper.isSceneSelected())
    {
      if (m_renderer.renderScene(l_type_data, l_selection_id))
      {
        // ship back the changes made.
        //getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->applyTypeRT(l_selection_id, l_type_data);

        //Set dirty flag
        if(l_editor_object != nullptr)
        {
          l_editor_object->setDirty(true);
        }
      }
    }
    else if (l_selection_keeper.isSpaceSelected())
    {
      if (m_renderer.renderSpace(l_type_data, l_selection_id))
      {
        // ship back the changes made.
        //getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->applyTypeRT(l_selection_id, l_type_data);

        //Set dirty flag
        if (l_editor_object != nullptr)
        {
          l_editor_object->setDirty(true);
        }
      }
    }
    else if (l_selection_keeper.isGameObjectSelected())
    {
      if (m_renderer.renderGameObject(l_type_data, l_selection_id))
      {
        // ship back the changes made.
        //getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->applyTypeRT(l_selection_id, l_type_data);

        //Set dirty flag
        if (l_editor_object != nullptr)
        {
          l_editor_object->setDirty(true);
        }
      }
    }

    //Send dirty typeRT data for current object back to engine
    l_editor_object_manager.sendDirtyObjectDataToEngine(l_selection_id);
  }

  auto l_scene_manipulator = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock();

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
            newCommand->Init(&getEngineController(), l_editor_object);

						// menu item clicked, request engine to add the component
            l_scene_manipulator->addGameObjectComponent(l_selection_id,
							l_comp_type.name);

            //Ask for typeRT from engine to override ours
            typeRT l_new_data = l_scene_manipulator->getTypeRT(l_selection_id);
            l_editor_object->setData(l_new_data);

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

