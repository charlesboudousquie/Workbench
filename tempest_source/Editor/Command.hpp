/*!***************************************************************************************
\file       Command.hpp
\author     Charles Boudousquie
\date       9/10/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the command abstract class.
*****************************************************************************************/
#pragma once

#include "Reflection.hpp"

namespace Editor
{
  class EditorObject;
  class engineController;
    //class typeRT;
    /*!***************************************************************************************
    \par class: Command
    \brief   This is the abstract interface for which a change to a gameobject(s) is represented.
    *****************************************************************************************/
    class Command
    {
    public:

        /*!***************************************************************************************
        \brief Initializes command with enginecontroller and object id.
        \param controller - gives access to engine, useful for object creation/deletion
        \param objectID - id of object, even if object is deleted we still have a reference to it
        *****************************************************************************************/
        virtual void Init(engineController* controller, EditorObject * p_editor_object);

        /*!***************************************************************************************
        \brief Undos a change done to the object
        *****************************************************************************************/
        virtual void undo() = 0;
        
        /*!***************************************************************************************
        \brief Redos a change done to the object
        *****************************************************************************************/
        virtual void redo() = 0;

        /*!***************************************************************************************
        \brief Records after state.
        \param newState - current state of object to be recorded
        *****************************************************************************************/
        virtual void Record();

    protected:
        std::string originalName;
        std::string newName;
        engineController * eController;
        EditorObject * m_editor_object;
        typeRT beforeState;
        typeRT afterState;
        unsigned long id; // hopefully objID always stays a unsigned long
    };

    
};

