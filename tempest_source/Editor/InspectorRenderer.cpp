/*!***************************************************************************************
\file       InspectorRenderer.cpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the renderer for the inspector window
*****************************************************************************************/
#include "InspectorRenderer.hpp"
#include "StyleKeeper.hpp"
#include <Reflection.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "EngineController.hpp"
#include "Scaffolding/WindowBase.hpp"
#include "EditorWindow.hpp"
#include "EditorLogger.hpp"
//Component Renderers
#include "Components/ButtonRenderer.hpp"
#include "Components/RigidBodyRenderer.h"
#include "Components/RendererRenderer.hpp"
#include "Components/InputRenderer.hpp"
#include "Components/AgentRenderer.hpp"
#include <Color4.hpp>
#include <Color4Serialization.hpp>
#include <EngineRunner.hpp>
#include <Engine.hpp>
#include "EditorObject.hpp"

#include "UndoRedo.hpp"
#include "ObjectCommand.hpp"


Editor::inspectorRenderer::inspectorRenderer(editorWindow * p_parent_window)
	: renderableBase(p_parent_window), m_inside_special_renderer(false)
{
	m_remove_handler = [](std::string p_component_name) { /* do nothing */ };

	getStyleKeeper().setStyle("*", "id", "readonly", 1);
  getStyleKeeper().setStyle("*", "subType", "readonly", 1);

  //Special Renderers
  m_component_renderers["class button"] = new buttonRenderer(p_parent_window);
  m_component_renderers["class rigidBody"] = new rigidBodyRenderer(p_parent_window);
  m_component_renderers["class renderer"] = new rendererRenderer(p_parent_window);
  m_component_renderers["class inputComponent"] = new inputRenderer(p_parent_window);
  m_component_renderers["class Agent"] = new agentRenderer(p_parent_window);
}

Editor::inspectorRenderer::~inspectorRenderer()
{
  //Free data inside ma of component renderers
  std::unordered_map<std::string, componentRenderer *>::iterator i_componentRenderer = m_component_renderers.begin();

  while(i_componentRenderer != m_component_renderers.end())
  {
    delete i_componentRenderer->second;
    i_componentRenderer->second = nullptr;

    ++i_componentRenderer;
  }

  m_component_renderers.clear();
}

void Editor::inspectorRenderer::setFilterOption(std::string p_name, bool p_enable)
{
	m_filter_options[p_name] = p_enable;
}

void Editor::inspectorRenderer::setRemoveHandler(std::function<void(std::string)> p_function)
{
	m_remove_handler = p_function;
}

bool Editor::inspectorRenderer::renderGameObject(typeRT & p_type_data, objID p_editor_object_id)
{
  EditorLogger & l_log = getLogger();
  EditorObjectManager & l_editor_object_manager = getTopWindow()->getSceneWindow().getEditorObjectManager();
  EditorObject * l_editor_object = l_editor_object_manager.getEditorObject(p_editor_object_id);
  if (getSelectionKeeper().isGameObjectSelected())
  {
    std::string l_name(getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->getObjectName(getSelectionKeeper().getSelectionId()));
    //Max length for gameObjectName
    l_name.reserve(40);

    // create command for undo redo operations
    CommandPtr command = std::make_shared<ObjectDataChangeCommand>();
    command->Init(&getEngineController(), getSelectionKeeper().getSelectionId());

    if (ImGui::InputText("<- Name", &l_name, ImGuiInputTextFlags_EnterReturnsTrue))
    {
      auto l_selection_keeper = getSelectionKeeper();
      l_log.AddLog("[EDITOR] Game Object: %s name changed to: %s.\n", getSelectionKeeper().getSelectionName().c_str(), l_name.c_str());
      getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->setGameObjectName(l_selection_keeper.getSelectionId(), std::string(l_name.c_str()));

      //Ask for typeRT from engine to override ours
      typeRT l_new_data = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->getTypeRT(l_selection_keeper.getSelectionId());
      l_editor_object->setData(l_new_data);
    
      // record change in game object's name
      command->Record();
      UndoRedoManager::GetInstance().RecordState(command, &getEngineController());
    }

    // create command to record change done to object inside inspector
    CommandPtr command2 = std::make_shared<ObjectDataChangeCommand>();
    command2->Init(&getEngineController(), getSelectionKeeper().getSelectionId());

    // reset watch dog to watch for future changes
    UndoRedoWatchdog::Get().Reset();

    bool RenderResult = render("*", p_type_data, p_editor_object_id);
    // if object changed and it is actually 
    if (UndoRedoWatchdog::Get().sawChange() && getSelectionKeeper().isGameObjectSelected())
    {
        command2->Record();
        UndoRedoManager::GetInstance().RecordState(command2, &getEngineController());
    }

    return RenderResult;

  }

  return false;
}

bool Editor::inspectorRenderer::renderScene(typeRT& p_type_data, objID p_editor_object)
{
  EditorLogger & l_log = getLogger();

  bool l_object_changed = render("*", p_type_data, p_editor_object);

  return l_object_changed;
}

bool Editor::inspectorRenderer::renderSpace(typeRT& p_type_data, objID p_editor_object)
{
  EditorLogger & l_log = getLogger();

  bool l_object_changed = render("*", p_type_data, p_editor_object);

  return l_object_changed;
}

void Editor::inspectorRenderer::beginStyle(const std::string& p_current_component_type, const std::string& p_variable_name)
{
	unsigned int l_disabled = getStyleKeeper().getStyle(p_current_component_type, p_variable_name, "readonly");
	if(l_disabled == 1)
		startDisabled();
}

void Editor::inspectorRenderer::endStyle(const std::string& p_current_component_type, const std::string& p_variable_name)
{
	unsigned int l_disabled = getStyleKeeper().getStyle(p_current_component_type, p_variable_name, "readonly");
	if (l_disabled == 1)
		endDisabled();
}

bool Editor::inspectorRenderer::render(const std::string & p_current_component_type, typeRT & p_type_data, objID p_editor_object)
{
	bool l_changed = false;
    // notify watchdog and obviously l_changed will be true as well
    auto MarkChange = [ &l_changed]() 
    {
        UndoRedoWatchdog::Get().Notify(); l_changed = true; 
    };
  Editor::EditorLogger & l_log = getLogger();

  auto l_scene_manipulator = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock();

  EditorObjectManager & l_editor_object_manager = getTopWindow()->getSceneWindow().getEditorObjectManager();
  EditorObject * l_editor_object = l_editor_object_manager.getEditorObject(p_editor_object);

	if (p_type_data.getTypeName() == "bool")
	{
		beginStyle(p_current_component_type, p_type_data.getVariableName());
		bool l_value = p_type_data.getBool();
		ImGui::Checkbox(p_type_data.getVariableName().c_str(), &l_value);
		if (l_value != p_type_data.getBool())
		{
			p_type_data.setBool(l_value);
            MarkChange();
			//l_changed = true;
		}
		endStyle(p_current_component_type, p_type_data.getVariableName());
	}
	else if (p_type_data.getTypeName() == "char")
	{
		beginStyle(p_current_component_type, p_type_data.getVariableName());
		char l_value = p_type_data.getChar();
		ImGui::InputText(p_type_data.getVariableName().c_str(), &l_value, 1);
		if (l_value != p_type_data.getChar())
		{
			p_type_data.setChar(l_value);
			//l_changed = true;
            MarkChange();
		}
		endStyle(p_current_component_type, p_type_data.getVariableName());
	}
	else if (p_type_data.getTypeName() == "int")
	{
		beginStyle(p_current_component_type, p_type_data.getVariableName());
		int l_value = p_type_data.getInt();
		ImGui::InputInt(p_type_data.getVariableName().c_str(), &l_value);
		if (l_value != p_type_data.getInt())
		{
			p_type_data.setInt(l_value);
            MarkChange();
            //l_changed = true;
		}
		endStyle(p_current_component_type, p_type_data.getVariableName());
	}
  else if (p_type_data.getTypeName() == "unsigned int")
  {
    beginStyle(p_current_component_type, p_type_data.getVariableName());
    int l_value = p_type_data.getUInt();
    ImGui::InputInt(p_type_data.getVariableName().c_str(), &l_value);
    int l_unsigned_value = static_cast<unsigned int>(l_value);
    if (l_unsigned_value != p_type_data.getUInt())
    {
      p_type_data.setUInt(l_unsigned_value);
      //l_changed = true;
      MarkChange();
    }
    endStyle(p_current_component_type, p_type_data.getVariableName());
  }
	else if (p_type_data.getTypeName() == "float")
	{
		beginStyle(p_current_component_type, p_type_data.getVariableName());
		float l_value = p_type_data.getFloat();
		ImGui::InputFloat(p_type_data.getVariableName().c_str(), &l_value);
		if (l_value != p_type_data.getFloat())
		{
			p_type_data.setFloat(l_value);
			//l_changed = true;
            MarkChange();
		}
		endStyle(p_current_component_type, p_type_data.getVariableName());
	}
	else if (p_type_data.getTypeName() == "double")
	{
		beginStyle(p_current_component_type, p_type_data.getVariableName());
		double l_value = p_type_data.getDouble();
		ImGui::InputDouble(p_type_data.getVariableName().c_str(), &l_value);
		if (l_value != p_type_data.getDouble())
		{
			p_type_data.setDouble(l_value);
			//l_changed = true;
            MarkChange();
		}
		endStyle(p_current_component_type, p_type_data.getVariableName());
	}
	else if (p_type_data.getTypeName() == "string")
	{
		beginStyle(p_current_component_type, p_type_data.getVariableName());
		constexpr int buff_size = 2048;
		char buffer[buff_size] = { 0 };
		p_type_data.getString().copy(buffer, sizeof(buffer));
		buffer[buff_size - 1] = '\0';
		if (ImGui::InputText(p_type_data.getVariableName().c_str(), &buffer[0], buff_size, ImGuiInputTextFlags_EnterReturnsTrue))
		{
		  std::string l_test(buffer);
			p_type_data.setString(l_test);
			//l_changed = true;
            MarkChange();
		}
		endStyle(p_current_component_type, p_type_data.getVariableName());
	}
	else if (p_type_data.getTypeName() == "unsigned long")
	{
		beginStyle(p_current_component_type, p_type_data.getVariableName());
		unsigned long l_value = p_type_data.getULong();
		ImGui::InputScalar(p_type_data.getVariableName().c_str(), ImGuiDataType_U64, &l_value);
		if (l_value != p_type_data.getULong())
		{
			p_type_data.setULong(l_value);
			//l_changed = true;
            MarkChange();
		}
		endStyle(p_current_component_type, p_type_data.getVariableName());
	}
	else if (p_type_data.getTypeName() == "gameObject")
	{
		for (auto & member : p_type_data.members())
		{
			if (render(p_current_component_type, member.second, p_editor_object))
			{
				l_changed = true;
                break;
			}
		}
	}
	else if (p_type_data.getTypeName() == "vector3")
	{
		beginStyle(p_current_component_type, p_type_data.getVariableName());
		float l_items[3];
		l_items[0] = p_type_data.member("x").getFloat();
		l_items[1] = p_type_data.member("y").getFloat();
		l_items[2] = p_type_data.member("z").getFloat();
		ImGui::DragFloat3(p_type_data.getVariableName().c_str(), &l_items[0]);
		if (l_items[0] != p_type_data.member("x").getFloat())
		{
			p_type_data.member("x").setFloat(l_items[0]);
            MarkChange();
			//l_changed = true;
		}
		if (l_items[1] != p_type_data.member("y").getFloat())
		{
			p_type_data.member("y").setFloat(l_items[1]);
            MarkChange();
			//l_changed = true;
		}
		if (l_items[2] != p_type_data.member("z").getFloat())
		{
			p_type_data.member("z").setFloat(l_items[2]);
            MarkChange();
			//l_changed = true;
		}
		endStyle(p_current_component_type, p_type_data.getVariableName());
	}
	else if (p_type_data.getTypeName() == "vector4")
	{
		beginStyle(p_current_component_type, p_type_data.getVariableName());
		float l_items[4];
		//float color_min[4] = { 0,0,0,0 };
		//float color_max[4] = { 1,1,1,1 };
		l_items[0] = p_type_data.member("x").getFloat();
		l_items[1] = p_type_data.member("y").getFloat();
		l_items[2] = p_type_data.member("z").getFloat();
		l_items[3] = p_type_data.member("w").getFloat();
		

		/*ImGui::ColorDrawHorizontalHSV("HSV");
		ImGui::RangeSliderFloat(p_type_data.getVariableName().c_str(), &l_items[0], &l_items[1], 0, 1, NULL, 1);*/
		ImGui::DragFloat4(p_type_data.getVariableName().c_str(), &l_items[0]);

		if (l_items[0] != p_type_data.member("x").getFloat())
		{
			p_type_data.member("x").setFloat(l_items[0]);
            MarkChange();
			//l_changed = true;
		}
		if (l_items[1] != p_type_data.member("y").getFloat())
		{
			p_type_data.member("y").setFloat(l_items[1]);
            MarkChange();
            //l_changed = true;
		}
		if (l_items[2] != p_type_data.member("z").getFloat())
		{
			p_type_data.member("z").setFloat(l_items[2]);
            MarkChange();
            //l_changed = true;
		}
		if (l_items[3] != p_type_data.member("w").getFloat())
		{
			p_type_data.member("w").setFloat(l_items[3]);
            MarkChange();
            //l_changed = true;
		}
		endStyle(p_current_component_type, p_type_data.getVariableName());
	}
	else if (p_type_data.getTypeName() == "colorRange")
	{
		beginStyle(p_current_component_type, p_type_data.getVariableName());

		std::vector<typeRT>& members = p_type_data.array();
		color4 min = ::toColor4(members[0]);
		color4 max = ::toColor4(members[1]);

		float color_min[4] = {min.x, min.y, min.z};
		float color_max[4] = {max.x, max.y, max.z};


		//ImGui::ColorPicker4("MyColor##4", &l_items[0], ImGuiColorEditFlags_PickerHueBar, NULL);
		if (ImGui::ColorDrawHorizontalHSV("HSV", &color_min[0], &color_max[0]))
		{

			members[0] = ::toTypeRT(color4(color_min[0], color_min[1], color_min[2]));
			members[1] = ::toTypeRT(color4(color_max[0], color_max[1], color_max[2]));
            MarkChange();
            //l_changed = true;

		}
		
		endStyle(p_current_component_type, p_type_data.getVariableName());
	}
	else if (p_type_data.getTypeName() == "angleRange")
	{
		beginStyle(p_current_component_type, p_type_data.getVariableName());

		std::vector<typeRT>& members = p_type_data.array();
		float min = (members[0].getFloat());
		float max = (members[1].getFloat());


		//ImGui::ColorPicker4("MyColor##4", &l_items[0], ImGuiColorEditFlags_PickerHueBar, NULL);
	
		if (ImGui::RangeSliderFloat(p_type_data.getVariableName().c_str(), &min, &max, 0, 90, NULL, 1))
		{
			members[0].setFloat(min);
			members[1].setFloat(max);
            MarkChange();
            //l_changed = true;

		}

	endStyle(p_current_component_type, p_type_data.getVariableName());
	}
	else if (p_type_data.getTypeName() == "quaternion")
	{
		beginStyle(p_current_component_type, p_type_data.getVariableName());
		float l_items[4];
		l_items[0] = p_type_data.member("x").getFloat();
		l_items[1] = p_type_data.member("y").getFloat();
		l_items[2] = p_type_data.member("z").getFloat();
		l_items[3] = p_type_data.member("w").getFloat();

		ImGui::DragFloat4(p_type_data.getVariableName().c_str(), &l_items[0]);
		if (l_items[0] != p_type_data.member("x").getFloat())
		{
			p_type_data.member("x").setFloat(l_items[0]);
            MarkChange();
            //l_changed = true;
		}
		if (l_items[1] != p_type_data.member("y").getFloat())
		{
			p_type_data.member("y").setFloat(l_items[1]);
            MarkChange();
            //l_changed = true;
		}
		if (l_items[2] != p_type_data.member("z").getFloat())
		{
			p_type_data.member("z").setFloat(l_items[2]);
            MarkChange();
            //l_changed = true;
		}
		if (l_items[3] != p_type_data.member("w").getFloat())
		{
			p_type_data.member("w").setFloat(l_items[3]);
            MarkChange();
            //l_changed = true;
		}
		endStyle(p_current_component_type, p_type_data.getVariableName());
}
	else if (p_type_data.getTypeName() == "childList")
	{
		// ignored

	}
	else if (p_type_data.getTypeName() == "components")
	{
		for (auto & l_component : p_type_data.array())
		{
			if (render(p_current_component_type, l_component, p_editor_object))
			{
               l_changed = true;
               break;
			}
		}

    //Add Parent Code
    //Have to press enter while in the text field to run this code
    std::string l_buffer;
    if (ImGui::InputText("Add Parent", &l_buffer, ImGuiInputTextFlags_EnterReturnsTrue))
    {
      objID l_myID = getSelectionKeeper().getSelectionId();

      //Find the button with the given name
      objID l_parent_id = getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->getObjectID(std::string(l_buffer.c_str()));

      if (l_parent_id != 0)
      {
          auto newCommand = std::make_shared<ObjectAddParentCommand>();
          newCommand->Init(&getEngineController(), getSelectionKeeper().getSelectionId());
        newCommand->SetParentID(l_parent_id);

        l_scene_manipulator->addParentToGameObject(l_parent_id, l_myID);

        //Ask for typeRT from engine to override parents new typeRT containing the child
        typeRT l_new_data = l_scene_manipulator->getTypeRT(l_parent_id);
        EditorObject * l_parent_object = l_editor_object_manager.getEditorObject(l_parent_id);

        l_parent_object->setData(l_new_data);

          UndoRedoManager::GetInstance().RecordState(newCommand, &getEngineController());
      }
      else
      {
        l_log.AddLog("[WARNING] No object by the name of %s.\n", l_buffer.c_str());
      }
    }

    //Remove Parent code
    if(ImGui::Button("Remove Parent"))
    {
      objID l_parent_id = l_scene_manipulator->getParentID(getSelectionKeeper().getSelectionId());

      if (l_parent_id != 0)
        {
            auto newCommand = std::make_shared<ObjectRemoveParentCommand>();
            newCommand->Init(&getEngineController(), getSelectionKeeper().getSelectionId());
            // make sure parent ID is recorded
        newCommand->SetParentID(l_scene_manipulator->getParentID(getSelectionKeeper().getSelectionId()));

            l_log.AddLog("[EDITOR] Removed parent from object: %s.\n", getSelectionKeeper().getSelectionName().c_str());
        l_scene_manipulator->removeParent(getSelectionKeeper().getSelectionId());

        //Ask for typeRT from engine to override parents new typeRT (now missing the child)
        typeRT l_new_data = l_scene_manipulator->getTypeRT(l_parent_id);
        EditorObject * l_parent_object = l_editor_object_manager.getEditorObject(l_parent_id);

        l_parent_object->setData(l_new_data);

            UndoRedoManager::GetInstance().RecordState(newCommand, &getEngineController());
        }
    }

    //Move Space code
    std::string l_space;
    //Max length for space Name
    l_space.reserve(40);

    if (ImGui::InputText("Move to Space", &l_space, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        auto moveCommand = std::make_shared<ObjectMoveToSpaceCommand>();
        moveCommand->Init(&getEngineController(), getSelectionKeeper().getSelectionId());

        //get old space id
      objID l_old_space_id = l_scene_manipulator->getSpaceIDForObject(getSelectionKeeper().getSelectionId());
      moveCommand->SetOldSpace(l_old_space_id);

        // get new space id
      objID l_new_space_id = l_scene_manipulator->getSpaceIDFromName(l_space);

      if (l_new_space_id != 0)
        {
        moveCommand->SetNewSpace(l_new_space_id);

            //sceneManip->moveObjectToSpace(m_selection->getSelectionId(), std::string(l_space.c_str()));
        l_scene_manipulator->moveObjectToSpace(getSelectionKeeper().getSelectionId(), l_new_space_id);

        //Need to ask engine for two new typeRTs, one for the old and one for the new space
        typeRT l_old_space_data = l_scene_manipulator->getTypeRT(l_old_space_id);
        typeRT l_new_space_data = l_scene_manipulator->getTypeRT(l_new_space_id);

        EditorObject * l_old_space_object = l_editor_object_manager.getEditorObject(l_old_space_id);
        EditorObject * l_new_space_object = l_editor_object_manager.getEditorObject(l_new_space_id);

        l_old_space_object->setData(l_old_space_data);
        l_new_space_object->setData(l_new_space_data);

            UndoRedoManager::GetInstance().RecordState(moveCommand, &getEngineController());
        }
    }

    //Delete GameObject code
    if(ImGui::Button("Delete GameObject"))
    {
      auto l_selection_keeper = getSelectionKeeper();
      objID l_object_id = l_selection_keeper.getSelectionId();
        auto deleteCommand = std::make_shared<ObjectDeleteCommand>();
      deleteCommand->Init(&getEngineController(), l_object_id);
      deleteCommand->SetObjectSelector(&l_selection_keeper);
      objID l_space_id = l_scene_manipulator->getSpaceIDForObject(l_object_id);
      deleteCommand->SetSpace(l_space_id);

      l_log.AddLog("[EDITOR] Deleted Game Object: %s.\n", l_selection_keeper.getSelectionName().c_str());
      l_scene_manipulator->removeGameObject(l_object_id);
      getSelectionKeeper().clearSelection();
      //Need to delete EditorObject of the same id
      l_editor_object_manager.removeEditorObject(l_object_id);

      //Need to get the new space data from engine (since its now missing an object)
      typeRT l_space_data = l_scene_manipulator->getTypeRT(l_space_id);
      EditorObject * l_space_object = l_editor_object_manager.getEditorObject(l_space_id);
      l_space_object->setData(l_space_data);

      UndoRedoManager::GetInstance().RecordState(deleteCommand, &getEngineController());

      return true;
    }
	}
	else if (p_type_data.getTypeName() == p_type_data.getVariableName() && p_type_data.getTypeName() != "") // assume it is a component
	{
	bool l_removed = false;
		if (ImGui::TreeNode(p_type_data.getTypeName().c_str()))
		{
			if (p_type_data.getVariableName() == "class transform")
			{
				getTopWindow()->getGizmoRenderer().renderProperties();
			}
			else
			{
        //Find the associated component renderer
        auto l_componentRenderer = m_component_renderers.find(p_type_data.getVariableName());
        if (l_componentRenderer != m_component_renderers.end())
        {
          l_changed = l_componentRenderer->second->render(p_type_data, p_editor_object);

          m_inside_special_renderer = true;
          m_current_special_renderer = p_type_data.getVariableName();
        }
        else
        {
          m_inside_special_renderer = false;
        }

        // display each component and send a blank typeRT for each to fill out with changes
        for (auto & member : p_type_data.members())
        {
          //If we are inside a special component renderer
          if (m_inside_special_renderer)
          {
            //If this specific special renderer overrides the current variable we are looking at.
            if (m_component_renderers[m_current_special_renderer]->overridesMember(member.second.getVariableName()))
            {
              //do nothing
              continue;
            }
          }
          //Try to render it as normal
          if (render(p_type_data.getVariableName(), member.second, p_editor_object))
          {
            l_changed = true;
          }
        }

        auto l_engine_runner = getEngineController().getEngineRunner()->getEngine()->getEngineMetadataManipulator();
				// engine will tell us if the component type is removable (if engine doesn't know, then we don't allow removal)
				bool l_removable = l_engine_runner.lock()->hasComponentType(p_type_data.getTypeName())
					&& l_engine_runner.lock()->getComponentType(p_type_data.getTypeName()).allowEditorRemove;
				if (l_removable)
				{
					if (ImGui::Button("Remove"))
					{
            std::string l_variable_name = p_type_data.getVariableName();
						m_remove_handler(l_variable_name);

            //Need to get new typeRT from engine
            typeRT l_new_data = l_scene_manipulator->getTypeRT(l_editor_object->getObjectID());
            l_editor_object->setData(l_new_data);

						l_removed = true;

            l_log.AddLog("[EDITOR] Removed component: %s\n from object: %s.\n", l_variable_name.c_str(), getSelectionKeeper().getSelectionName().c_str());
					}
				}
			}
			ImGui::TreePop();

			if (l_removed)
				return true;
		}
	}
	else
	{
    auto l_selection_keeper = getSelectionKeeper();

    if (l_selection_keeper.isSpaceSelected())
    {
      std::string l_name(l_scene_manipulator->getSpaceName(l_selection_keeper.getSelectionId()));
      //Max length for space Name
      l_name.reserve(40);

      if (ImGui::InputText("<- Name", &l_name, ImGuiInputTextFlags_EnterReturnsTrue))
      {
        l_log.AddLog("[EDITOR] Space: %s name changed to: %s.\n", l_selection_keeper.getSelectionName().c_str(), l_name.c_str());
        l_scene_manipulator->setSpaceName(l_selection_keeper.getSelectionId(), std::string(l_name.c_str()));

        //Need to get new typeRT from engine
        typeRT l_new_data = l_scene_manipulator->getTypeRT(l_editor_object->getObjectID());
        l_editor_object->setData(l_new_data);
      }

      if(ImGui::Button("Delete Space"))
      {
        l_log.AddLog("[EDITOR] Deleted Space: %s.\n", l_selection_keeper.getSelectionName().c_str());
        l_scene_manipulator->deleteSpace(l_selection_keeper.getSelectionId());
        l_selection_keeper.clearSelection();

        //Need to delete associated Editor Object (us)
        l_editor_object_manager.removeEditorObject(l_editor_object->getObjectID());
        return false;
      }

      if (p_type_data.members().find("shouldDeleteOnLoad") != p_type_data.members().end())
      {
        bool l_value = p_type_data.member("shouldDeleteOnLoad").getBool();
        ImGui::Checkbox("Should Delete On Load", &l_value);
        if (l_value != p_type_data.member("shouldDeleteOnLoad").getBool())
        {
          p_type_data.member("shouldDeleteOnLoad").setBool(l_value);
          l_changed = true;
        }
      }
      
    }
    else if(l_selection_keeper.isSceneSelected())
    {
      std::string l_name(l_scene_manipulator->getSceneName(l_selection_keeper.getSelectionId()));
      //Max length for scene Name
      l_name.reserve(40);

      if (ImGui::InputText("<- Name", &l_name, ImGuiInputTextFlags_EnterReturnsTrue))
      {
        l_log.AddLog("[EDITOR] Scene: %s name changed to: %s.\n", l_selection_keeper.getSelectionName().c_str(), l_name.c_str());
        l_scene_manipulator->setSceneName(l_selection_keeper.getSelectionId(), std::string(l_name.c_str()));

        //Need to get new typeRT from engine
        typeRT l_new_data = l_scene_manipulator->getTypeRT(l_editor_object->getObjectID());
        l_editor_object->setData(l_new_data);
      }
    }
    else
    {
      ImGui::Text("UNHANDLED TYPE: '%s' NAME: '%s'", p_type_data.getTypeName().c_str(), p_type_data.getVariableName().c_str());
    }
	}

	return l_changed;
}

bool Editor::inspectorRenderer::isFilterOptionSet(std::string p_name) const
{
	auto l_iter = m_filter_options.find(p_name);
	if (l_iter == m_filter_options.end())
		return false;
	return (*l_iter).second;
}

void Editor::inspectorRenderer::startDisabled()
{
	if (m_disable_depth == 0)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}
	m_disable_depth++;
}

void Editor::inspectorRenderer::endDisabled()
{
	if (m_disable_depth <= 0)
		return;
	m_disable_depth--;
	if (m_disable_depth == 0)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}
}
