/*!***************************************************************************************
\file       BaseNode2.cpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief		Example node of a TemplateNode.
*****************************************************************************************/

#include "BaseNode2.hpp"

BaseNode2::BaseNode2() : m_cool(true), m_int(2), m_float(2.0f), m_color(100.0f, 0.0f, 100.0f, 20.0f)
{
  setName("Cool Node");
  setIntputs(2);
  setOutputs(4);
}

BaseNode2::~BaseNode2()
{

}

typeRT BaseNode2::onRender()
{
    typeRT l_data;

    l_data.setTypeName("Node");
    l_data.setVariableName("BaseNode2");

    std::vector<typeRT> l_render_members;
    std::vector<typeRT> l_non_render_members;

    typeRT l_render_data;
    l_render_data.setTypeName("Node Render Data");
    l_render_data.setVariableName("Node Render Data");

    typeRT l_cool("Cool", bool(true));
    l_render_members.push_back(l_cool);

    typeRT l_int("Cool Value", int(2));
    l_render_members.push_back(l_int);

    typeRT l_float("Cool Time", float(2.0f));
    l_render_members.push_back(l_float);

    //////////////////////////////////////////////////

    typeRT l_non_render_data;
    l_non_render_data.setTypeName("Node Non Render Data");
    l_non_render_data.setVariableName("Node Non Render Data");

    typeRT l_inputs("Inputs", int(2));
    l_non_render_members.push_back(l_inputs);

    typeRT l_outputs("Outputs", int(4));
    l_non_render_members.push_back(l_outputs);

    //typeRT l_color("Color");
    //color4 l_color_data(100.0f, 0.0f, 100.0f, 20.0f);
    //l_color.setRawData(&l_color_data, sizeof(color4), "Color");
    //l_non_render_members.push_back(l_color);

    //Push back render data
    for (auto i_member : l_render_members)
    {
        l_render_data.insertMember(i_member);
    }

    //Push back non render data
    for (auto i_member : l_non_render_members)
    {
        l_non_render_data.insertMember(i_member);
    }

    l_data.insertMember(l_render_data);
    l_data.insertMember(l_non_render_data);

    return l_data;
}

void BaseNode2::updateData(const typeRT& p_data)
{

}

std::pair<bool, std::string> BaseNode2::validateBase()
{
  //Add any rules here that are specific to this node
  if (getNumChildren() >= 1)
  {
    return std::make_pair<bool, std::string>(true, "Base Success");
  }

  return std::make_pair<bool, std::string>(false, "Need at least one child.");
}
