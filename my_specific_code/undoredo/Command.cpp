#include "Command.hpp"
#include "UndoRedo.hpp"
#include "EngineController.hpp"
#include "SceneManipulator.hpp"

#include "EngineRunner.hpp"
#include "Engine.hpp"

#define GET_MANIPULATOR eController->getEngineRunner()->getEngine()->getSceneManipulator().lock()

void Editor::Command::Init(engineController * controller, unsigned int objectID)
{
    eController = controller;
    id = objectID;
    // set old state
    beforeState = GET_MANIPULATOR->getTypeRT(id);
    originalName = GET_MANIPULATOR->getObjectName(id);
}

void Editor::Command::Record()
{
    afterState = GET_MANIPULATOR->getTypeRT(id);
    newName = GET_MANIPULATOR->getObjectName(id);
}

