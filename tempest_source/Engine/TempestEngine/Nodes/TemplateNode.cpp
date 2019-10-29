/*!***************************************************************************************
\file       TemplateNode.cpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief		This is a template node used for understading how to build a node class.
*****************************************************************************************/

#include "TemplateNode.hpp"

TemplateNode::TemplateNode() : m_type("Template")
{

}

TemplateNode::~TemplateNode()
{

}

typeRT TemplateNode::toTypeRT() const
{
  return getTypeRT();
}

void TemplateNode::updateFromTypeRT(const typeRT& p_data)
{
  updateData(p_data);
}

std::pair<bool, std::string> TemplateNode::validate()
{
	return validateBase();
}

std::pair<bool, std::string> TemplateNode::validateBase()
{
	//Add any conditions this type of node needs to meet
	if (true)
	{
		return std::make_pair<bool, std::string>(true, "Success");
	}
}

