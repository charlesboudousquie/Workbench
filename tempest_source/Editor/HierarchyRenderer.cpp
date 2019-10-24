/*!***************************************************************************************
\file       HierarchyRenderer.cpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the renderer of the hierarchy window content.
*****************************************************************************************/
#include "HierarchyRenderer.hpp"
#include "HierarchySelectionKeeper.hpp"
#include "EngineController.hpp"
#include "EngineRunner.hpp"
#include <Engine.hpp>
#include <imgui.h>
#include <SceneManagement/Scene.hpp>
#include <SceneManagement/Space.hpp>
#include <SceneManagement/GameObject.hpp>
#include "EditorWindow.hpp"
#include "EditorObjectManager.hpp"
#include "EditorObject.hpp"

Editor::hierarchyRenderer::hierarchyRenderer(editorWindow * p_parent_window)
	: renderableBase{p_parent_window}
{ }

void Editor::hierarchyRenderer::onRender()
{
	// display the current scene
	if(!getEngineController().getEngineRunner()->getEngine()->getCurrentScene().expired())
	{
		std::shared_ptr<scene> l_scene = getEngineController().getEngineRunner()->getEngine()->getCurrentScene().lock();
		onRender({l_scene});
	}
}

void Editor::hierarchyRenderer::onRender(std::vector<std::shared_ptr<scene>> p_scenes)
{
	for(auto l_scene : p_scenes)
		renderScene(l_scene);
}

void Editor::hierarchyRenderer::renderScene(std::shared_ptr<scene> p_scene)
{
  auto l_scene_manipulator = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock();
  EditorObjectManager & l_editor_object_manager = getTopWindow()->getSceneWindow().getEditorObjectManager();
  objID l_object_id = p_scene->getID();

  //First time rendering this object, need to create a Editor Object for this scene
  if(!l_editor_object_manager.ifEditorObjectExists(l_object_id))
  {
    //Need to create new Editor Object
    typeRT l_data = l_scene_manipulator->getTypeRT(l_object_id);
    EditorObject * l_new_object = new EditorObject(EditorObjectType::enm_scene, l_data, l_object_id);

    if(l_new_object != nullptr)
    {
      l_editor_object_manager.addEditorObject(l_new_object);
    }
  }

	ImGuiTreeNodeFlags l_node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	// check for selection
	if (getSelectionKeeper().isSelected(p_scene))
		l_node_flags |= ImGuiTreeNodeFlags_Selected;
	if (ImGui::TreeNodeEx(p_scene->getName().c_str(), l_node_flags))
	{
		// check for selected
		if (ImGui::IsItemClicked())
		{
			getSelectionKeeper().setSelected(p_scene);
		}
		// display spaces
		for (std::shared_ptr<space> l_space : p_scene->getSpaces())
		{
			renderSpace(l_space);
		}
		ImGui::TreePop();
	}

}

void Editor::hierarchyRenderer::renderSpace(std::shared_ptr<space> p_space)
{
  auto l_scene_manipulator = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock();
  EditorObjectManager & l_editor_object_manager = getTopWindow()->getSceneWindow().getEditorObjectManager();
  objID l_object_id = p_space->getID();

  //First time rendering this object, need to create a Editor Object for this scene
  if (!l_editor_object_manager.ifEditorObjectExists(l_object_id))
  {
    //Need to create new Editor Object
    typeRT l_data = l_scene_manipulator->getTypeRT(l_object_id);
    EditorObject * l_new_object = new EditorObject(EditorObjectType::enm_space, l_data, l_object_id);

    if (l_new_object != nullptr)
    {
      l_editor_object_manager.addEditorObject(l_new_object);
    }
  }

	ImGuiTreeNodeFlags l_node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	// check for selection
	if (getSelectionKeeper().isSelected(p_space))
		l_node_flags |= ImGuiTreeNodeFlags_Selected;
	if (ImGui::TreeNodeEx(p_space->getName().c_str(), l_node_flags))
	{
		// check for selected
		if (ImGui::IsItemClicked())
		{
			getSelectionKeeper().setSelected(p_space);
		}
		// display game objects
		for (std::shared_ptr<gameObject> l_object : p_space->getTopLevelGameObjectList())
		{
			renderGameObject(l_object);
		}
		ImGui::TreePop();
	}

}

void Editor::hierarchyRenderer::renderGameObject(std::shared_ptr<gameObject> p_object)
{
  auto l_scene_manipulator = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock();
  EditorObjectManager & l_editor_object_manager = getTopWindow()->getSceneWindow().getEditorObjectManager();
  objID l_object_id = p_object->getID();

  //First time rendering this object, need to create a Editor Object for this scene
  if (!l_editor_object_manager.ifEditorObjectExists(l_object_id))
  {
    //Need to create new Editor Object
    typeRT l_data = l_scene_manipulator->getTypeRT(l_object_id);
    EditorObject * l_new_object = new EditorObject(EditorObjectType::enm_game_object, l_data, l_object_id);

    if (l_new_object != nullptr)
    {
      l_editor_object_manager.addEditorObject(l_new_object);
    }
  }

	ImGuiTreeNodeFlags l_node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	// check for selection
	if (getSelectionKeeper().isSelected(p_object))
		l_node_flags |= ImGuiTreeNodeFlags_Selected;
	// if there are children, show as a leaf
	bool l_isLeaf = p_object->getChildren().size() == 0;
	if (l_isLeaf)
	{
		// set additional flags for leaf
		l_node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	bool l_node_open = ImGui::TreeNodeEx(p_object->getName().c_str(), l_node_flags);
	// check for selected
	if (ImGui::IsItemClicked())
	{
		getSelectionKeeper().setSelected(p_object);
	}

	if (l_node_open)
	{
		// display children
		for (std::shared_ptr<gameObject> l_child : p_object->getChildren())
		{
			renderGameObject(l_child);
		}
		if (!l_isLeaf)
			ImGui::TreePop();
	}

}
