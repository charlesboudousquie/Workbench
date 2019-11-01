/*!***************************************************************************************
\file       ObjectCommand.hpp
\author     Charles Boudousquie
\date       9/10/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the object command abstract class.
*****************************************************************************************/
#pragma once

#include "Command.hpp"

namespace Editor
{

    class hierarchySelectionKeeper;

    /*!***************************************************************************************
    \par class: ObjectCommand
    \brief   This class is the abstract representation of modifying an object.
    *****************************************************************************************/
    class ObjectCommand : public Command
    {
    public:
    };

    /*!***************************************************************************************
    \par class: ObjectMoveToSpaceCommand
    \brief   This class moving an object from one space to another.
    *****************************************************************************************/
    class ObjectMoveToSpaceCommand : public ObjectCommand
    {
        objID oldSpace;
        objID newSpace;

    public:

        void SetOldSpace(objID old_space);
        void SetNewSpace(objID new_space);

        /*!***************************************************************************************
        \brief Sends back to original space
        *****************************************************************************************/
        void undo() override;

        /*!***************************************************************************************
        \brief Sends object to new space.
        *****************************************************************************************/
        void redo() override;

    };


    /*!***************************************************************************************
    \par class: ObjectAddParentCommand
    \brief   This class represents the action of adding the parent of an object.
    *****************************************************************************************/
    class ObjectAddParentCommand : public ObjectCommand
    {
        objID parentID;

    public:

        void SetParentID(objID parentObjectID);

        /*!***************************************************************************************
        \brief Reattaches parent
        *****************************************************************************************/
        void undo();
        /*!***************************************************************************************
        \brief Detaches parent again
        *****************************************************************************************/
        void redo();
    };


    /*!***************************************************************************************
    \par class: ObjectRemoveParentCommand
    \brief   This class represents the action of removing the parent of an object.
    *****************************************************************************************/
    class ObjectRemoveParentCommand : public ObjectCommand
    {
        objID parentID;

    public:

        void SetParentID(objID parentObjectID);

        /*!***************************************************************************************
        \brief Reattaches parent
        *****************************************************************************************/
        void undo();
        /*!***************************************************************************************
        \brief Detaches parent again
        *****************************************************************************************/
        void redo();
    };

    /*!***************************************************************************************
    \par class: ObjectDeleteCommand
    \brief   This class represents the action of deleting an object.
    *****************************************************************************************/
    class ObjectDeleteCommand : public ObjectCommand
    {
        hierarchySelectionKeeper* objSelector;

        objID spaceID;

        // Do not call record on this object!
    public:

        void SetSpace(objID spaceID);

        /*!***************************************************************************************
        \brief Gives Delete command the object selector so it can clear it when object is deleted
        \param objectSelector - the object selector
        *****************************************************************************************/
        void SetObjectSelector(hierarchySelectionKeeper* objectSelector);

        /*!***************************************************************************************
        \brief Undos a change done to the object
        *****************************************************************************************/
        void undo() override;
        /*!***************************************************************************************
        \brief Redos a change done to the object
        *****************************************************************************************/
        void redo() override;

        /*!***************************************************************************************
        \brief There is nothing to record if object is deleted
        *****************************************************************************************/
        void Record() override;
    };

    /*!***************************************************************************************
    \par class: ObjectDataChangeCommand
    \brief   This class represents the action of modifying an object.
    *****************************************************************************************/
    class ObjectDataChangeCommand : public ObjectCommand
    {
    public:

        /*!***************************************************************************************
        \brief Undos a change done to the object
        *****************************************************************************************/
        void undo() override;

        /*!***************************************************************************************
        \brief Redos a change done to the object
        *****************************************************************************************/
        void redo() override;
    };
};
