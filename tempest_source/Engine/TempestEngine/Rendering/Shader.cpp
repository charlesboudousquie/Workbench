/*!***************************************************************************************
\file       shader.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This hold the CPU repersentation of the GPU shaders.
*****************************************************************************************/

//========Self Include==================================================================//
#include "Shader.hpp"
//========1st Party Includes============================================================//
#include "../Utility/AssetManager.hpp"
#include "../Utility/GeneralizedFileLoader.hpp"


shader::shader(assetHandle p_assetHandle) : m_shaderName(assetManager::stringFromWide(p_assetHandle->getName()))
{
	//get source data as string
	auto data = p_assetHandle->getData<genericAssetData>();
	auto rawData = data.getRaw();
	std::stringstream ss;
	for(char c: rawData)
	{
		ss << c;
	}
	setSourceData(ss.str());
}


