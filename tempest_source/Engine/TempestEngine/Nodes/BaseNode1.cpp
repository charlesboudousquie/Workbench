/*!***************************************************************************************
\file       BaseNode1.cpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief		Example node of a TemplateNode.
*****************************************************************************************/

#include "BaseNode1.hpp"

BaseNode1::BaseNode1()
{
  setName("BaseNode1");

  setIntputs(1);
  setOutputs(1);
}

BaseNode1::~BaseNode1()
{
  
}

typeRT BaseNode1::onRender()
{
  typeRT l_data;

  return l_data;
}

void BaseNode1::updateData(const typeRT& p_data)
{

}
