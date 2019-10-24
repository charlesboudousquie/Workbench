/*!***************************************************************************************
\file       EditorObject.cpp
\author     Ryan Booth
\date       10/10/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/

#include "EditorObject.hpp"

Editor::EditorObject::EditorObject() : m_object_type(EditorObjectType::enm_INVALID), m_object_id(0), m_dirty(false)
{
  
}

Editor::EditorObject::EditorObject(EditorObjectType p_object_type, const typeRT& p_data, objID p_object_id)
                                  : m_object_type(p_object_type), m_data(p_data), m_object_id(p_object_id), m_dirty(false)
{

}

void Editor::EditorObject::setData(const typeRT & p_data)
{
  //Is this the valid behavior we want?
  m_data = p_data;
}

void Editor::EditorObject::setObjectID(objID p_object_id)
{
  m_object_id = p_object_id;
}

void Editor::EditorObject::setObjectType(EditorObjectType p_object_type)
{
  m_object_type = p_object_type;
}

void Editor::EditorObject::setDirty(bool p_dirty)
{
  m_dirty = p_dirty;
}
