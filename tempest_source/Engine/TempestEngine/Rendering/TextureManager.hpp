/*!***************************************************************************************
\file       TextureManager.hpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Manages all the textures on both the CPU and GPU
*****************************************************************************************/
#pragma once
//========Self Include==================================================================//
//========1st Party Includes============================================================//
#include "Texture.hpp"
#include "SimpleTexture.hpp"
#include "CubeTexture.hpp"
#include "Material.hpp"

#include <unordered_map>

//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

////////==========================================================================////////
////////      Class                                                               ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: textureManager
\brief   Manages all the textures on both the CPU and GPU.
*****************************************************************************************/
class textureManager
{
    ///////============================================================================///////
    ///////   Public                                                                   ///////
    ///////============================================================================///////

public:

	textureManager() = default;

    //======================================================================================//
    //        Getters & Setters                                                             //
    //======================================================================================//

    /*!***************************************************************************************
    \brief Get the texture
    \param p_textureLabel -  Name of the texture
    \return shared ptr of the texture in the map
    *****************************************************************************************/
    std::shared_ptr<texture> getSimpleTexture(std::string const& p_filename);
    std::shared_ptr<texture> getCubeTexture(std::string const& p_textureLabel);

	template<typename Texture_T, typename... Args>
	std::shared_ptr<Texture_T> createTexture(std::string&& tag, Args&&...args)
	{
		return std::static_pointer_cast<Texture_T>(
			m_textures.emplace(
				std::move(tag),
				std::make_shared<Texture_T>(std::forward<Args>(args)...)
			).first->second
		);
	}

	template<typename Texture_T, typename... Args>
	std::shared_ptr<Texture_T> createTexture(std::string const& tag, Args&&...args)
	{
		return std::static_pointer_cast<Texture_T>(
			m_textures.emplace(
				tag,
				std::make_shared<Texture_T>(std::forward<Args>(args)...)
			).first->second
		);
	}

    /////================================================================================/////
    /////     Functions                                                                  /////
    /////================================================================================/////

    void UnbindAll();


    std::shared_ptr<texture> createCubeTexture(std::string p_label, std::vector<assetHandle> p_assetHandles);
    ///////============================================================================///////
    ///////    Private                                                                 ///////
    ///////============================================================================///////

private:

    /*!***************************************************************************************
    \brief Loads a material from file
    \param p_assetPath The file path
    *****************************************************************************************/
    std::shared_ptr<texture> createSimpleTexture(assetHandle p_assetHandle);

    /////================================================================================/////
    /////     Functions                                                                  /////
    /////================================================================================/////
    std::unordered_map<std::string, std::shared_ptr<texture> > m_textures;

    //configurationFunctionPtr m_configurationFunction;

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
