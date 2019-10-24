/*!***************************************************************************************
\file       EditorObject.hpp
\author     Ryan Booth
\date       10/10/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/
#pragma once

#include "Reflection.hpp"

namespace Editor
{
  enum class EditorObjectType
  {
    enm_scene,
    enm_space,
    enm_game_object,
    enm_INVALID
  };

  class EditorObject
  {
  public:
    EditorObject();

    EditorObject(EditorObjectType p_object_type, const typeRT & p_data, objID p_object_id);

    const typeRT & getConstData() const { return m_data; }

    typeRT & getData() { return m_data; }

    void setData(const typeRT & p_data);

    objID getObjectID() const { return m_object_id; }

    void setObjectID(objID p_object_id);

    EditorObjectType getObjectType() const { return m_object_type; }

    void setObjectType(EditorObjectType p_object_type);

    bool getDirty() const { return m_dirty; }

    void setDirty(bool p_dirty);

  private:
    EditorObjectType m_object_type;
    typeRT m_data;
    objID m_object_id;

    bool m_dirty;
  };
}

