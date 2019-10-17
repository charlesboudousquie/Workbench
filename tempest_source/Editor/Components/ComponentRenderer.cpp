/*!***************************************************************************************
\file       ComponentRenderer.cpp
\author     Ryan Booth
\date       3/8/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief	
*****************************************************************************************/
#include "ComponentRenderer.hpp"

Editor::componentRenderer::componentRenderer(Editor::editorWindow * p_parent_window)
        : renderableBase(p_parent_window)
{

}

bool Editor::componentRenderer::render(typeRT & p_type_data)
{
  //Aaron does this and one time he said you should do it like this. 
  return onRender(p_type_data);
}

bool Editor::componentRenderer::overridesMember(const std::string & p_member)
{
  if(m_override_members.find(p_member) != m_override_members.end())
  {
    return true;
  }

  return false;
}
