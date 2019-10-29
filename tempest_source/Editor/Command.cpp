#include "Command.hpp"
#include "UndoRedo.hpp"
#include "EngineController.hpp"
#include "SceneManipulator.hpp"
#include "EditorObject.hpp"

#include "EngineRunner.hpp"
#include "Engine.hpp"

//#define GET_MANIPULATOR eController->getEngineRunner()->getEngine()->getSceneManipulator().lock()

void Editor::Command::Init(engineController * controller, EditorObject * p_editor_object)
{
  if(p_editor_object != nullptr)
  {
    eController = controller;
    m_editor_object = p_editor_object;
    id = p_editor_object->getObjectID();
    // set old state
    //beforeState = GET_MANIPULATOR->getTypeRT(id);
    //originalName = GET_MANIPULATOR->getObjectName(id);
    
    beforeState = m_editor_object->getConstData();
    originalName = m_editor_object->getObjectName();
  }

}

void Editor::Command::Record()
{
    //afterState = GET_MANIPULATOR->getTypeRT(id);
    //newName = GET_MANIPULATOR->getObjectName(id);
    
  afterState = m_editor_object->getConstData();
  newName = m_editor_object->getObjectName();
}

