/*!***************************************************************************************
\file       EditorNode.cpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/

#include "EditorNode.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include "Color4.hpp"

#include <iostream>
const float c_field_width = 75.0f; //units = pixels


EditorNode::EditorNode() : m_id(0), m_depth(0), m_name(""), m_class_name(""), m_type_name(""), m_inputs(0), m_outputs(0), m_num_children(0), m_num_parents(0),
m_position(0.0f, 0.0f), m_scale(190.0f, 90.0f), m_color(0.0f, 0.0f, 0.0f, 17.0f), m_active(false)
{

}

void EditorNode::disableModifications()
{
  ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
  ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
}

void EditorNode::enableModifications()
{
  ImGui::PopItemFlag();
  ImGui::PopStyleVar();
}

bool EditorNode::render(bool p_engine_active)
{
  bool l_changed = false;

  //Get a copy in case we change it
  //typeRT l_type_data = m_render_data;
  
  std::map<std::string, typeRT>& l_members = m_render_data.members();

  ImGui::BeginGroup(); // Lock horizontal position

  if(l_members.find("Node Render Data") != l_members.end())
  {
    typeRT & l_node_render_data = m_render_data.member("Node Render Data");

    for (auto & i_member : l_node_render_data.members())
    {
      typeRT & l_member = i_member.second;

      if (p_engine_active)
        disableModifications();

      ImGui::PushItemWidth(c_field_width);

      if (l_member.getTypeName() == "bool")
      {
        bool l_value = l_member.getBool();

        ImGui::Checkbox(l_member.getVariableName().c_str(), &l_value);
        if (l_value != l_member.getBool())
        {
          l_member.setBool(l_value);
          l_changed = true;
        }
      }
      else if (i_member.second.getTypeName() == "char")
      {
        char l_value = l_member.getChar();
        ImGui::InputText(l_member.getVariableName().c_str(), &l_value, 1);
        if (l_value != l_member.getChar())
        {
          l_member.setChar(l_value);
          l_changed = true;
        }
      }
      else if (i_member.second.getTypeName() == "int")
      {
        int l_value = l_member.getInt();
        ImGui::InputInt(l_member.getVariableName().c_str(), &l_value);
        if (l_value != l_member.getInt())
        {
          l_member.setInt(l_value);
          l_changed = true;
        }
      }
      else if (i_member.second.getTypeName() == "unsigned int")
      {
        int l_value = l_member.getUInt();
        ImGui::InputInt(l_member.getVariableName().c_str(), &l_value);
        int l_unsigned_value = static_cast<unsigned int>(l_value);
        if (l_unsigned_value != l_member.getUInt())
        {
          l_member.setUInt(l_unsigned_value);
          l_changed = true;
        }
      }
      else if (i_member.second.getTypeName() == "float")
      {
        float l_value = l_member.getFloat();
        ImGui::InputFloat(l_member.getVariableName().c_str(), &l_value, 0, 0, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
        if (l_value != l_member.getFloat())
        {
          l_member.setFloat(l_value);
          l_changed = true;
        }
      }
      else if (i_member.second.getTypeName() == "double")
      {
        double l_value = l_member.getDouble();
        ImGui::InputDouble(l_member.getVariableName().c_str(), &l_value);
        if (l_value != l_member.getDouble())
        {
          l_member.setDouble(l_value);
          l_changed = true;
        }
      }
      else if (i_member.second.getTypeName() == "string")
      {
        constexpr int buff_size = 2048;
        char buffer[buff_size] = { 0 };
        l_member.getString().copy(buffer, sizeof(buffer));
        buffer[buff_size - 1] = '\0';
        if (ImGui::InputText(l_member.getVariableName().c_str(), &buffer[0], buff_size, ImGuiInputTextFlags_EnterReturnsTrue))
        {
          std::string l_test(buffer);
          l_member.setString(l_test);
          l_changed = true;
        }
      }
      else if (i_member.second.getTypeName() == "unsigned long")
      {
        unsigned long l_value = l_member.getULong();
        ImGui::InputScalar(l_member.getVariableName().c_str(), ImGuiDataType_U64, &l_value);
        if (l_value != l_member.getULong())
        {
          l_member.setULong(l_value);
          l_changed = true;
        }
      }

      if (p_engine_active)
        enableModifications();

      ImGui::PopItemWidth();
    }
  }

  std::string l_name = m_name;
  l_name.append(": ");
  l_name.append(std::to_string(m_id));

  ImGui::Text("%s", l_name.c_str());

  ImGui::EndGroup();

  return l_changed;
}

ImVec2 EditorNode::GetInputSlotPos(int slot_no) const
{
  return ImVec2(m_position.x, m_position.y + m_scale.y * ((float)slot_no + 1) / ((float)m_inputs + 1));
}

ImVec2 EditorNode::GetOutputSlotPos(int slot_no) const
{ 
  return ImVec2(m_position.x + m_scale.x, m_position.y + m_scale.y * ((float)slot_no + 1) / ((float)m_outputs + 1));
}

void EditorNode::setRenderData(const typeRT & p_render_data)
{
  m_render_data = p_render_data;

  auto l_members = m_render_data.members();

  if (l_members.find("Node Non Render Data") != l_members.end())
  {
    typeRT l_non_render_data = m_render_data.member("Node Non Render Data");

    auto l_non_render_members = l_non_render_data.members();

    if (l_non_render_members.find("Inputs") != l_non_render_members.end())
    {
      setIntputs(l_non_render_data.member("Inputs").getInt());
    }
    if (l_non_render_members.find("Outputs") != l_non_render_members.end())
    {
      setOutputs(l_non_render_data.member("Outputs").getInt());
    }
  }

  if(l_members.find("Color") != l_members.end())
  {
    color4 l_color = *reinterpret_cast<color4*>(m_render_data.member("Color").getRawData());

    setColor(ImVec4(l_color.x, l_color.y, l_color.z, l_color.w));
  }
}

void EditorNode::addNodeLink(NodeLink p_link)
{
  m_node_links.push_back(p_link);
}

void EditorNode::removeNodeLink(NodeLink p_link)
{
  for(auto i_node_link = m_node_links.begin(); i_node_link != m_node_links.end(); ++i_node_link)
  {
    if(*i_node_link == p_link)
    {
      m_node_links.erase(i_node_link);

      return;
    }
  }
}

