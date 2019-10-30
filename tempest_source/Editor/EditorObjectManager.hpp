/*!***************************************************************************************
\file       EditorObjectManager.hpp
\author     Ryan Booth
\date       10/10/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/
#pragma once

#include <map>

typedef unsigned long objID;

class typeRT;

namespace Editor
{
  class EditorObject;
  enum class EditorObjectType;
  class engineController;
  struct EditorLogger;

   class EditorObjectManager
   {
   public:
     EditorObjectManager(Editor::engineController & p_engine_controller, Editor::EditorLogger & p_logger);

     EditorObject * getEditorObject(objID p_object_id);

     void addEditorObject(EditorObject * p_object);

     void removeEditorObject(objID p_object_id);

     void removeEditorObject(EditorObject * p_object);

     void clearObjectList();

     std::map<objID, EditorObject *> getEditorObjectsOfType(EditorObjectType p_object_type);

     void updateEditorObjectFromEngine(objID p_object_id, const typeRT & p_data);

     void sendDirtyObjectDataToEngine(objID p_object_id);

     void sendAllDirtyObjectDataToEngine();

     bool ifEditorObjectExists(objID p_object_id);

   private:
     std::map<objID, EditorObject *> m_objects;

     engineController & m_engine_controller;

     EditorLogger & m_logger;
   };
}