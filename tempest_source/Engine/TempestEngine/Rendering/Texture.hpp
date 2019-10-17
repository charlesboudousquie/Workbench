/*!***************************************************************************************
\file       Texture.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Holds all information about a texture
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../Utility/AssetManager.hpp"

//#include "TypeData.hpp"
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

////////==========================================================================////////
////////  Class                                                                   ////////
////////==========================================================================////////
/*!***************************************************************************************
\par class: texture
\brief   Holds all information about a texture.
*****************************************************************************************/
class texture
{
    ///////============================================================================///////
    ///////   Public                                                                   ///////
    ///////============================================================================///////
public:

    //======================================================================================//
    //        Constructor                                                                   //
    //======================================================================================//
    virtual void build() = 0;
    virtual void bind(unsigned int slot) = 0;
    virtual void unbind() = 0;
    virtual int getBoundSlot() = 0;
		virtual void setData(assetHandle p_assetHandle) = 0;

};



///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

////==================================================================================////
////      (Non-)Configurable                                                          ////
////==================================================================================////

////==================================================================================////
////      Overridden/(Non-/Pure-)Virtual                                              ////
////==================================================================================////

//======================================================================================//
//        (Conversion )Constructor / Operators / Getters & Setters / Helper Destructor  //
//======================================================================================//

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/

// namespace <namespace name>
