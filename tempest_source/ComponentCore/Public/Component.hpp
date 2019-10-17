/*!***************************************************************************************
\file       Component.hpp
\author     Aaron Damyen
\date       5/14/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  The definition of the component base class.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "ComponentType.hpp"
#include <Reflection.hpp>

//========3rd Party Includes============================================================//
#include <memory>
#include <ComponentTypeDetails.hpp> // convenience for subclasses

//========Forward Declarations==========================================================//
class gameObject;

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: component
\brief   This is the base class for all component types.
*****************************************************************************************/
class component
{
	////////======================================================================////////
	////////   Public                                                             ////////
	////////======================================================================////////
	public:
		//////======================================================================//////
		//////    (Non-)Static                                                      //////
		//////======================================================================//////

		/*!*******************************************************************************
		\brief  Retrieves the type of component this instance is. SceneSystem requirement.
		\return componentType - The type of this component.
		*********************************************************************************/
		virtual componentType const type() const = 0;

		/*!*******************************************************************************
		\brief  Allows inherited types to be properly destroyed.
		*********************************************************************************/
		virtual ~component() = default;

		/*!*******************************************************************************
		\brief  Converts this into a type
		\return Type equivalent.
		*********************************************************************************/
		virtual typeRT toTypeRT() const = 0;

		virtual void updateFromTypeRT(typeRT & p_type) = 0;

		virtual bool isTransient() { return false; }

		/*!*******************************************************************************
		\brief  Retrieves the reference to the game object this component is attached to.
		\return std::weak_ptr<gameObject> - Reference to the parent game object.
		*********************************************************************************/
		std::weak_ptr<gameObject> getGameObject();

		/*!*******************************************************************************
		\brief  Tells the total number of types of components in the entire engine. 
		\return int - number of component types in the game engine. 
		*********************************************************************************/
		static int getComponentTypes() { return componentTypeCount;  }
		virtual int getTypeID() = 0;
		static int incrementComponentTypeCount() { return componentTypeCount++; }

	////////======================================================================////////
	////////   Private                                                            ////////
	////////======================================================================////////
	private:
		/*!*******************************************************************************
		\brief  Retrieves the reference to the game object this component is attached to.
		\return std::weak_ptr<gameObject> - Reference to the parent game object.
		*********************************************************************************/
		static int componentTypeCount;
		/*!*******************************************************************************
		\brief  Reference to the parent game object.
		*********************************************************************************/
		std::weak_ptr<gameObject> parentGameObject;
	
		//////======================================================================//////
		//////    (Non-)Static                                                      //////
		//////======================================================================//////

		/*!*******************************************************************************
		\brief  Allows game object to reach into this to update parent properly.
		\relates gameObject
		*********************************************************************************/
		friend gameObject;
};

template <typename T>
class componentCRTP : public component
{
public:

	/*!***************************************************************************************
	\brief  Retrieves the type of component this instance is. SceneSystem requirement.
	\return componentType - The type of this component.
	*****************************************************************************************/
	virtual componentType const type() const override { 
		static componentType const s_type = componentType::forConcrete<T>();
		return s_type; 
	}

	virtual int getTypeID() override {
		return componentID;
	}
private:
	static int componentID;
};

template <typename T>
int componentCRTP<T>::componentID = incrementComponentTypeCount();


