/*!***************************************************************************************
\file       ObjectCommand.cpp
\author     Charles Boudousquie
\date       9/10/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation for the object command classes.
*****************************************************************************************/

#include "ObjectCommand.hpp"
#include "EngineController.hpp"
#include "UndoRedo.hpp"
#include "GameObject.hpp"
#include "HierarchySelectionKeeper.hpp"

#include "EngineRunner.hpp"
#include "Engine.hpp"

#define GET_MANIPULATOR eController->getEngineRunner()->getEngine()->getSceneManipulator().lock()

void Editor::ObjectDataChangeCommand::undo()
{
    GET_MANIPULATOR->applyTypeRT(id, beforeState);
    GET_MANIPULATOR->setGameObjectName(id, originalName);
}

void Editor::ObjectDataChangeCommand::redo()
{
    GET_MANIPULATOR->applyTypeRT(id, afterState);
    GET_MANIPULATOR->setGameObjectName(id, newName);
}




void Editor::ObjectDeleteCommand::SetSpace(objID id)
{
    spaceID = id;
}

void Editor::ObjectDeleteCommand::SetObjectSelector(hierarchySelectionKeeper * objectSelector)
{
    this->objSelector = objectSelector;
}



void Editor::ObjectDeleteCommand::undo()
{
    // recreate object
    auto newObject = GET_MANIPULATOR->addEmptyGameObject();

    // get the id of the "new" object
    id = newObject->getID();
    GET_MANIPULATOR->applyTypeRT(id, beforeState);
    GET_MANIPULATOR->setGameObjectName(id, this->originalName);

    auto spaceName = GET_MANIPULATOR->getSpaceName(spaceID);
    GET_MANIPULATOR->moveObjectToSpace(id, spaceName);
}

void Editor::ObjectDeleteCommand::redo()
{
    // delete object again
    GET_MANIPULATOR->removeGameObject(id);
    objSelector->clearSelection();
}

void Editor::ObjectDeleteCommand::Record()
{
}



void Editor::ObjectRemoveParentCommand::SetParentID(objID parentObjectID)
{
    this->parentID = parentObjectID;
}

void Editor::ObjectRemoveParentCommand::undo()
{
    // attach parent
    GET_MANIPULATOR->addParentToGameObject(parentID, id);
}

void Editor::ObjectRemoveParentCommand::redo()
{
    // remove parent
    GET_MANIPULATOR->removeParent(id);
}

void Editor::ObjectAddParentCommand::SetParentID(objID parentObjectID)
{
    this->parentID = parentObjectID;
}

void Editor::ObjectAddParentCommand::undo()
{
    GET_MANIPULATOR->removeParent(id);
}

void Editor::ObjectAddParentCommand::redo()
{
    GET_MANIPULATOR->addParentToGameObject(parentID, id);
}


void Editor::ObjectMoveToSpaceCommand::SetOldSpace(objID old_space)
{
    oldSpace = old_space;
}

void Editor::ObjectMoveToSpaceCommand::SetNewSpace(objID new_space)
{
    newSpace = new_space;
}

/*!***************************************************************************************
\brief Sends back to original space
*****************************************************************************************/
void Editor::ObjectMoveToSpaceCommand::undo()
{
    //std::string oldSpaceName = GET_MANIPULATOR->getSpaceName(oldSpace);
    GET_MANIPULATOR->moveObjectToSpace(id, oldSpace);
}

/*!***************************************************************************************
\brief Sends object to new space.
*****************************************************************************************/
void Editor::ObjectMoveToSpaceCommand::redo()
{
    //std::string newSpaceName = GET_MANIPULATOR->getSpaceName(newSpace);
    GET_MANIPULATOR->moveObjectToSpace(id, newSpace);
}



