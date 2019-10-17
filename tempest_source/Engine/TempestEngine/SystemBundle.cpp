/*!***************************************************************************************
\file       SystemBundle.cpp
\author     Michael-Paul Moore
\date       6/29/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  System bundle functionality.
*****************************************************************************************/

//========Self Include==================================================================//
#include "SystemBundle.hpp"

//========1st Party Includes============================================================//
#include "SystemBase.hpp"
#include "SystemTask.hpp"

////////==========================================================================////////
////////  Class (systemBundle)                                                    ////////
////////==========================================================================////////

	///////============================================================================///////
	///////   Public                                                                   ///////
	///////============================================================================///////

		//////==============================================================================//////
		//////    Non-Static                                                                //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		//======================================================================================//
		//        Constructor                                                                   //
		//======================================================================================//

		systemBundle::systemBundle(const std::string & p_bundle_name,
		                           const std::vector<std::pair<std::string, systemBase*>>&& p_systems,
		                           const thread_number && p_thread_afinity)
		                          : m_task(*this, p_thread_afinity), m_bundle_name(p_bundle_name),
		                            m_systems_in_vector(createVector(p_systems)),
		                            m_systems_in_map(createMap(p_systems)) { }

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		const std::string& systemBundle::name() const
		{
			return m_bundle_name;
		}

		systemTask & systemBundle::task()
		{
			return m_task;
		}

		const systemTask & systemBundle::task() const
		{
			return const_cast<systemBundle*>(this)->task();
		}

		const std::map<std::string, systemBase*>& systemBundle::systemMap() const
		{
			return m_systems_in_map;
		}

		const std::vector<systemBase*>& systemBundle::sysVec() const
		{
			return m_systems_in_vector;
		}

		//======================================================================================//
		//        Destructor                                                                   ///
		//======================================================================================//

		systemBundle::~systemBundle()
		{
			for (auto i_iter = m_systems_in_vector.rbegin(); i_iter != m_systems_in_vector.rend(); )
			{
				delete *i_iter;
				++i_iter;
			}
		}

	///////============================================================================///////
	///////   Private                                                                  ///////
	///////============================================================================///////

		//////==============================================================================//////
		//////    Non-Static                                                                //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		std::map<std::string, systemBase*> systemBundle::createMap(const std::vector<std::pair<std::string, systemBase*>>& p_systems) const
		{
			std::map<std::string, systemBase*> result;
			for (const auto & system_pair : p_systems)
				result[system_pair.first] = system_pair.second;
			return result;
		}

		std::vector<systemBase*> systemBundle::createVector(const std::vector<std::pair<std::string, systemBase*>>& p_systems) const
		{
			// preserve ordering
			std::vector<systemBase*> l_result;
			for(size_t i_index = 0; i_index < p_systems.size(); ++i_index)
			{
				l_result.push_back(p_systems[i_index].second);
			}
			return l_result;
		}
