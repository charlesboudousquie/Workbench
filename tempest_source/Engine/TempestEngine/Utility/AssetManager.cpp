/*!**********************************************************************************************************
\file       AssetManager.cpp
\author     Henry Brobeck
\date       1/30/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/

#include "AssetManager.hpp"
#include <filesystem>
#include "GeneralizedFileLoader.hpp"
#include "../Systems/FBXLoader.hpp"
#include "TextureLoader.hpp"
#include "MaterialLoader.hpp"

// IMPORTANT NOTE: std::codecvt_utf8 (and I think everythign in <locale>) has been
//                 deprecated. It would be good to find another, cross-platform way to
//                 handle the conversions which doesn't make use of these deprecated
//                 features.
#include <locale> // std::wstring_convert, std::codecvt_utf8

std::map<std::wstring, asset*> assetManager::m_assets;
std::vector<std::wstring> assetManager::m_assetNames;
std::map<std::wstring, fileLoader*> assetManager::m_typeToLoader;
fileLoader* assetManager::m_fallbackLoader;
logger assetManager::log("AssetManager");
//
// asset::asset(const std::wstring& p_fullName) : m_assetData(nullptr), m_references(0), m_fullPath(p_fullName)
// {
//	m_name = m_fullPath.filename();
//	m_extention = m_fullPath.extension();
//	m_path = m_fullPath.parent_path();
// }

asset::asset(const std::wstring& p_fullName, const std::wstring& p_name, fileLoader* p_loader) : m_assetData(nullptr), m_references(0), m_fullPath(p_fullName), m_loader(p_loader)
{
	m_name = p_name;
	m_extention = m_fullPath.extension().generic_wstring();
	m_path = m_fullPath.parent_path().generic_wstring();
	m_type = m_extention;
}

const std::wstring& asset::getName()
{
	return m_name;
}

const std::wstring& asset::getExtention()
{
	return m_extention;
}

const std::wstring& asset::getPath()
{
	return m_path;
}

const std::filesystem::path& asset::getFullPath()
{
	return m_fullPath;
}

bool asset::hasData()
{
	return m_assetData != nullptr;
}

fileLoader* asset::getLoader()
{
	return m_loader;
}

const std::wstring& asset::getType()
{
	return m_type;
}

void asset::setType(const std::wstring& p_type)
{
	m_type = p_type;
}

int asset::getRefs()
{
	return m_references;
}

assetHandle::assetHandle(asset* p_parent) : m_parent(p_parent)
{
	incRef();
}

assetHandle::assetHandle(assetHandle&& rhs) noexcept
{
	m_parent = rhs.m_parent;
	rhs.m_parent = nullptr;
}

assetHandle::assetHandle(const assetHandle& rhs)
{
	m_parent = rhs.m_parent;
	incRef();
}

assetHandle::~assetHandle()
{
	decRef();
}

asset* assetHandle::operator->() const
{
	assetManager::loadAsset(*m_parent);
	return m_parent;
}

assetHandle& assetHandle::operator=(const assetHandle& rhs)
{
	if(&rhs == this)
	{
		return *this;
	}
	decRef();
	m_parent = rhs.m_parent;
	incRef();
	return *this;
}

assetHandle& assetHandle::operator=(assetHandle&& rhs) noexcept
{
	decRef();
	m_parent = rhs.m_parent;
	rhs.m_parent = nullptr;
	return *this;
}

bool assetHandle::operator==(void* rhs) const
{
	return m_parent == rhs;
}

bool assetHandle::operator==(std::nullptr_t rhs) const
{
	return m_parent == rhs;
}

void assetHandle::incRef()
{
	if(m_parent)
	{
		m_parent->m_references++;
	}
}

void assetHandle::decRef()
{
	if(m_parent)
	{
		m_parent->m_references--;
	}
}

void assetManager::init(const std::string & p_asset_path)
{
	//To add a file type to a specific file loader, add it here
	auto general = new generalizedFileLoader;
	m_fallbackLoader = general;
	auto fbx = new FBXLoader;
	m_typeToLoader.emplace(L".fbx", fbx);
	auto l_textureLoader = new textureLoader;
	m_typeToLoader.emplace(L".png", l_textureLoader);
	auto l_materialLoader = new materialLoader;
	m_typeToLoader.emplace(L".mtl", l_materialLoader);

    //auto l_behavior_treeLoader = new TreeLoader;

	loadAssetList(p_asset_path);

}

void assetManager::shutdown()
{
	//unload all assets, then delete them
	for(auto assetIter : m_assets)
	{

		unloadAsset(*assetIter.second);
		delete assetIter.second;
	}
	m_assets.clear();
	m_assetNames.clear();
	for(auto loaderIter : m_typeToLoader)
	{
		delete loaderIter.second;
	}
	delete m_fallbackLoader;
}

void assetManager::loadAsset(asset& p_asset)
{
	if(p_asset.hasData())
	{
		return;
	}


	p_asset.getLoader()->loadAsset(p_asset);

}

void assetManager::loadAsset(assetHandle& p_assetHandle)
{
	if(p_assetHandle.m_parent)
	{
		loadAsset(*p_assetHandle.m_parent);
	}

}

void assetManager::unloadAsset(asset& p_asset)
{
	if(p_asset.hasData())
	{
		p_asset.getLoader()->unloadAsset(p_asset);
	}
}

const std::vector<std::wstring>& assetManager::assetList()
{
	return m_assetNames;
}

std::vector<std::string> assetManager::assetList(std::string fileExtensionFilter)
{
    std::vector<std::string> list;

    // for all asset names
    for (auto file : m_assetNames)
    {
        // convert it to regular string
        std::string fileName = stringFromWide(file);

        // check if file extension is actually in string
        if (fileName.find(fileExtensionFilter) != std::wstring::npos)
        {
            list.push_back(fileName);
        }
    }

    return list;
}

assetHandle assetManager::getAsset(const std::filesystem::path& p_name)
{
	auto iter = m_assets.find(p_name.generic_wstring());
	if(iter == m_assets.end())
	{
		log.error(std::string("Failed to load asset, name doesn't exist:") + p_name.generic_string());
		return assetHandle(nullptr);
	}
	return assetHandle(iter->second);
}

std::string assetManager::stringFromWide(const std::wstring& p_convert)
{
	//snippet from https://stackoverflow.com/questions/215963/how-do-you-properly-use-widechartomultibyte

#if 0
	if (p_convert.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &p_convert[0], (int)p_convert.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &p_convert[0], (int)p_convert.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> l_converter;
	return l_converter.to_bytes(p_convert);
#endif
}

std::wstring assetManager::wideFromString(const std::string& p_string)
{
#if 0
	if (p_string.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &p_string[0], (int)p_string.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &p_string[0], (int)p_string.size(), &wstrTo[0], size_needed);
	return wstrTo;
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> l_converter;
	return l_converter.from_bytes(p_string);
#endif
}

int assetManager::unloadUnreferenced()
{
	int numUnloaded = 0;
	for(auto iter : m_assets)
	{
		if(iter.second->getRefs() <= 0)
		{
			unloadAsset(*iter.second);
		}
	}
	return numUnloaded;
}

bool assetManager::unloadIfUnreferenced(const std::wstring& p_assetName)
{
	auto assetIter = m_assets.find(p_assetName);
	if(assetIter != m_assets.end())
	{
		if(assetIter->second->getRefs() <= 0)
		{
			unloadAsset(*assetIter->second);
			return true;
		}
	}
	return false;
}

void assetManager::loadAssetList(const std::filesystem::path & p_asset_path)
{
	//setup vector for loading assets from fileLoaders
	std::vector<asset*> assetList;
	//get current working directory then go to assets subfolder
	for(auto file : std::filesystem::recursive_directory_iterator(p_asset_path))
	{
		auto filePath = file.path();
		if(!is_directory(filePath))
		{

			//this is a file, not a directory, so add it to the asset list
			//construct the relative path
			// auto relative = std::filesystem::relative(filePath, assets);
			assetList.clear();
			auto loaderIter = m_typeToLoader.find(filePath.extension().generic_wstring());
			fileLoader* loader = m_fallbackLoader;
			if(loaderIter != m_typeToLoader.end())
			{
				//specialized file loader for file type was in map
				loader = loaderIter->second;
			}

			loader->assetsFrom(p_asset_path.string(), filePath.generic_wstring(),
							   assetList);
			for(auto assetIter : assetList)
			{
				m_assetNames.push_back(assetIter->getName());
				m_assets.insert(std::pair(assetIter->getName(), assetIter));
			}

		}
	}

}
