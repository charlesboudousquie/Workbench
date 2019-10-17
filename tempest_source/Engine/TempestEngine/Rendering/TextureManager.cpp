/*!***************************************************************************************
\file       TextureManager.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Manages all the textures on both the CPU and GPU
*****************************************************************************************/
#include "../Precompiled.h"
//========Self Include==================================================================//
#include "TextureManager.hpp"

//========1st Party Includes============================================================//

#include "SimpleTexture.hpp"

#include "TypeData.hpp"

#include "../Utility/TextureLoader.hpp"


//========3rd Party Includes============================================================//
#include <Logger.hpp>
#include <filesystem>
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

	///////========================================================================///////
	///////   Public                                                               ///////
	///////========================================================================///////

		//==============================================================================//
		//        Constructor                                                           //
		//==============================================================================//

		//==============================================================================//
		//        Destructor                                                            //
		//==============================================================================//

		//==============================================================================//
		//        Operators                                                             //
		//==============================================================================//

		//==============================================================================//
		//        Getters & Setters                                                     //
		//==============================================================================//

        // could do this better but just want to get it working
        std::shared_ptr<texture> textureManager::getSimpleTexture(std::string const& p_fileName)
        {

            auto l_texture = m_textures.find(p_fileName); // find texture in map

            if(l_texture == m_textures.end())
            {
							// not in map, add it
							std::filesystem::path l_path("textures");
							l_path /= p_fileName;
							createSimpleTexture(assetManager::getAsset(l_path));
							l_texture = m_textures.find(p_fileName);
                //logger("TextureManager").debug() << "Error: Simple Texture Not in Map: " << p_fileName;
                //return nullptr;
            }

            return l_texture->second;
        }

        std::shared_ptr<texture> textureManager::getCubeTexture(std::string const& p_fileName)
        {
            auto l_texture = m_textures.find(p_fileName);

            if (l_texture == m_textures.end())
            {
                logger("TextureManager").debug() << "Error: Cube Texture Not in Map: " << p_fileName;
                return nullptr;
            }
           return l_texture->second;
        }

        void textureManager::UnbindAll()
        {
            for(auto & t : m_textures)
            {
                t.second->unbind();
            }


        }

        ////==========================================================================////
		////      Non-Virtual                                                         ////
		////==========================================================================////

		//////======================================================================//////
		//////    Non-Static                                                        //////
		//////======================================================================//////

		/////========================================================================/////
		/////     Functions                                                          /////
		/////========================================================================/////
        std::shared_ptr<texture> textureManager::createSimpleTexture(assetHandle p_assetHandle)
        {
						auto texturePtr = std::shared_ptr<texture>(new simpleTexture(p_assetHandle));

						texturePtr->setData(p_assetHandle);

#if 0
						auto l_assetname = getFileNameFromAssetPath(
							std::string(p_assetHandle->getName().begin(),
								p_assetHandle->getName().end()));
#else
			auto l_assetname = std::filesystem::path(p_assetHandle->getName()).filename().generic_string();
#endif

            m_textures.insert(std::make_pair(l_assetname, texturePtr));
						texturePtr->build();
            return texturePtr;
        }

        std::shared_ptr<texture> textureManager::createCubeTexture(std::string p_label, std::vector<assetHandle> p_assetHandles)
        {

						auto label = getFileNameFromAssetPath(
							std::string(p_assetHandles[0]->getName().begin(),
								p_assetHandles[0]->getName().end()));

					  auto texturePtr = std::shared_ptr<texture>(new cubeTexture(p_assetHandles, label));


            m_textures.insert(std::make_pair(p_label, texturePtr));
            return getCubeTexture(p_label);
        }

	///////========================================================================///////
	///////   Private                                                              ///////
	///////========================================================================///////

		//////======================================================================//////
		//////    Non-Static                                                        //////
		//////======================================================================//////

		/////========================================================================/////
		/////     Functions                                                          /////
		/////========================================================================/////

		////==========================================================================////
		////      Non-Virtual                                                         ////
		////==========================================================================////

		//==============================================================================//
		//        Helper                                                                //
		//==============================================================================//
