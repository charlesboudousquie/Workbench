/*!***************************************************************************************
\file       Engine.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the engine class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "Engine.hpp"
//========1st Party Includes============================================================//
#include "Actions/ActionSystem.hpp"
#include "Application.hpp"
#include "Components/Camera.hpp"
#include "Components/Light.hpp"
#include "Components/Renderer.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Skybox.hpp"
#include "External/SceneManipulator.hpp"
#include "GameObjectFiltering/GameObjectFilter.hpp"
#include "GameObjectFiltering/ListBuilder.hpp"
#include "GameScripts/InputTestScript.hpp"
#include "GameScripts/TestingScript.hpp"
#include "Rendering/GraphicsSystem.hpp"
#include "Input/InputSystem.hpp"
#include "Messaging/MessagingSystem.hpp"
#include "PersistenceSystem.hpp"
#include "Platform/PlatformWindowing.hpp"
#include "SceneManagement/Scene.hpp"
#include "SceneManagement/SceneSystem.hpp"
#include "SceneManagement/Transform.hpp"
#include "Scripting/ScriptingSystem.hpp"
#include "SystemBundle.hpp"
#include "SystemManager.hpp"
#include "TransformTestScript.hpp"
#include "Components/InputComponent.hpp"
#include "Rendering/TypeData.hpp"
#include "Audio/AudioSystem.h"
#include "Telemetry/TelemetrySystem.hpp"
#include "Rendering/CameraSystem.hpp"
#include "BehaviorTreeManager.hpp"
#include "DynamicWaypoints/DynamicWaypointGraph.hpp"
#include "BehaviorTreeManipulator.hpp"

#include "SystemManagement/SystemManagerSTA.hpp"
#include "GameScripts/LightTestScript.hpp"
// holy moly thats a lot of includes
//========3rd Party Includes============================================================//
#include <synchapi.h>
#include <memory>
#include "External/bundleManipulator.hpp"
#include "Audio/AudioComponent.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "PersistenceManipulator.hpp"
#include "Replay/InstantReplaySystem.hpp"
//#include "Replay/JumbotronComponent.hpp"
#include "SceneManagement/UISystem.hpp"
#include "Components/RenderTarget.hpp"
#include "GameScripts/CameraScript.hpp"
#include "GameScripts/RenderTargetScript.hpp"
#include "Events/EventSystem.hpp"
#include "GamePadSystem.hpp"
#include "Components/ParticleEmitter.hpp"
#include "Rendering/ParticleSystem.hpp"
#include "External/EngineMetadataManipulator.hpp"
#include "External/CameraManipulatorInterface.hpp"
#include "External/CameraManipulator.hpp"
#include "External/NodeManipulatorInterface.hpp"
#include "External/NodeManipulator.hpp"
#include "External/GraphicsManipulator.hpp"
#include "Utility/AssetManager.hpp"
#include "Utility/GeneralizedFileLoader.hpp"
#include "Systems/FBXLoader.hpp"
#include "Utility/NavMeshGenerator.hpp"
#include "External/AssetManipulator.hpp"
#include <RenderContextInterface.hpp>

//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/

///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

engine::engine(externalWindowInterface * p_window, renderContextInterface * p_render_context, inputSourceInterface * p_input_source_ptr, icomponentRepository * p_component_repository)
	: m_window_ptr{ p_window }, m_render_context_ptr{ p_render_context },
	m_input_source_ptr{ p_input_source_ptr }, m_system_manager{ new systemManagerSTA() },
	m_scene_manipulator_ptr{ nullptr }, m_bundle_manipulator_ptr{ nullptr },
	m_persistence_manipulator_ptr{ nullptr }, m_component_repository_ptr{ p_component_repository }
{ }

engine::~engine()
{
	delete m_system_manager;
	m_system_manager = nullptr;
}

void engine::initialize(configuration p_config)
{
	// 4. initial asset manager
// ------------------------
	assetManager::init(p_config.asset_path.string());

	// save the configuration
	m_config = p_config;

	// clear the quest requested flag
	m_is_quit_requested = false;

	// notify the application interface that there is a new engine
	application::setEnginePtr(this);

	// setup the game object filtering
	m_go_filter = new GameObjectFiltering::gameObjectFilter(this, m_component_repository_ptr);
	m_go_filter->setBuilder(new GameObjectFiltering::listBuilder());

	// configure and start the system manager
	m_system_manager->initialize([this]() { return this->getConfiguration(); });
	m_system_manager->setGameObjectGatherer(m_go_filter);

	/////================================================================================/////
	/////  Begin: TO BE MOVED TO THE PLUGIN INTERFACE                                    /////
	/////================================================================================/////

	// 1. Construct bundles with system instances
	// ------------------------------------------

	systemBundle* l_scripting_bundle = new systemBundle("Scripting Bundle",
		{
			std::make_pair("scripting", new scriptingSystem())
		});

	gamepadSystem* l_gamepadSystem = new gamepadSystem;
	inputSystem* l_inputSystem = new inputSystem(m_input_source_ptr);
	l_inputSystem->addInputSource(l_gamepadSystem->getInputAdapter());
	// m_gamepadInputAdapater = new gamepadInputAdapter;
	// l_inputSystem->addInputSource(m_gamepadInputAdapater);

	systemBundle* l_graphics_and_windowing_bundle = new systemBundle("graphics & display",
		{
			//std::make_pair("windowing", new windowing()),
			std::make_pair(platformWindowing::getName(), new platformWindowing(m_window_ptr, m_render_context_ptr)),
			std::make_pair(graphicsSystem::getName(), new graphicsSystem(m_window_ptr, m_render_context_ptr)),
			//std::make_pair("graphics", new renderSystem(m_render_context_ptr)),
			std::make_pair(gamepadSystem::getName(), l_gamepadSystem),
			std::make_pair(inputSystem::getName(), l_inputSystem)

		},
		MAIN_THREAD_AFFINITY); //NOT_SET_AFFINITY,MAIN_THREAD_AFFINITY

	systemBundle* l_audio_bundle = new systemBundle("Audio Bundle",
		{
			std::make_pair(AudioSystem::getName(), new AudioSystem())
		});

	systemBundle* l_scene_management_bundle = new systemBundle(sceneSystem::getName(),
		{
			std::make_pair(sceneSystem::getName(), new sceneSystem(m_go_filter))
		});

	systemBundle* l_automation_bundle = new systemBundle("Automation Bundle",
		{
			std::make_pair(actionSystem::getName(), new actionSystem()),
			std::make_pair(messagingSystem::getName(), new messagingSystem()),
			std::make_pair(eventSystem::getName(), new eventSystem())
		});

	systemBundle* l_physics_bundle = new systemBundle("physics Bundle",
		{
			std::make_pair("physicsSystem", new physicsSystem())
		});

	systemBundle* l_persistence_bundle = new systemBundle(persistenceSystem::getName(),
		{
			std::make_pair(persistenceSystem::getName(), new persistenceSystem())
		});

	//systemBundle* l_telemetry_bundle = new systemBundle(telemetrySystem::getName(), 
	//	{
	//		std::make_pair(telemetrySystem::getName(), new telemetrySystem())
	//	});

	/*systemBundle* l_instantreplay_bundle = new systemBundle(instantReplaySystem::getName(),
		{
			std::make_pair(instantReplaySystem::getName(), new instantReplaySystem())
		});*/

	systemBundle* l_ui_bundle = new systemBundle(uiSystem::getName(),
		{
			std::make_pair(uiSystem::getName(), new uiSystem())
		});

	systemBundle* l_particle_bundle = new systemBundle(particleSystem::getName(),
		{
			std::make_pair(particleSystem::getName(), new particleSystem())
		});

	systemBundle* l_camera_bundle = new systemBundle(cameraSystem::getName(),
		{
			std::make_pair(cameraSystem::getName(), new cameraSystem())
		});

    // charles code
    systemBundle* l_behavior_bundle = new systemBundle(BehaviorTreeManager::getName(),
		{
            std::make_pair(BehaviorTreeManager::getName(), new BehaviorTreeManager())
        }
        );


	// systemBundle* l_gamepad_bundle = new systemBundle(gamepadSystem::getName(),
	// 	{
	// 		std::make_pair(gamepadSystem::getName(), new gamepadSystem)
	// 	}
	// 	);
	// 2. add system bundles to the system manager
	// -------------------------------------------
	m_system_manager->addSystemBundles(
		{
			//l_telemetry_bundle,
			//l_instantreplay_bundle,
			l_scripting_bundle,
			l_graphics_and_windowing_bundle,
			l_audio_bundle,
			l_scene_management_bundle,
			l_automation_bundle,
			l_physics_bundle,
			l_persistence_bundle,
			l_ui_bundle,
			l_particle_bundle,
			l_camera_bundle,
		    l_behavior_bundle,
		});

	const std::vector<std::string> l_system_bundle_names
	{
		//l_telemetry_bundle->name(),
		//l_instantreplay_bundle->name(),
		l_scene_management_bundle->name(),
		l_automation_bundle->name(),
		l_scripting_bundle->name(),
		l_graphics_and_windowing_bundle->name(),
		l_audio_bundle->name(),
		l_physics_bundle->name(),
		l_persistence_bundle->name(),
		l_ui_bundle->name(),
		 l_particle_bundle->name(),
		l_camera_bundle->name(),
	    l_behavior_bundle->name(),
	};

	// 3. initialize the bundles
	// -------------------------
	m_system_manager->initializeSystemBundles(l_system_bundle_names);

	m_system_manager->activateSystemBundles(l_system_bundle_names);




	// 5. load the initial scene
	// -------------------------
  //getPersistenceManipulator().lock()->setPersistenceLocation(p_config.data_path);
  if(m_persistence_manipulator_ptr == nullptr)
  {
    getPersistenceManipulator();
  }
  m_persistence_manipulator_ptr->setPersistenceLocation(p_config.data_path.string());

	if (!p_config.no_initial_scene)
	{
		getPersistenceManipulator().lock()->load(p_config.initial_scene_name);
	}
	navMeshGenerator::init(this);
}

void engine::update()
{
	// m_gamepadInputAdapater->updateState();
	m_system_manager->update();
}

void engine::render()
{
	//// render all systems
	//for (auto system_ptr : enabled_systems)
	//	system_ptr->render();
}

void engine::shutdown()
{
	m_system_manager->shutdown();
	Sleep(1000);
	delete m_go_filter;
	// delete m_gamepadInputAdapater;
}

std::weak_ptr<behaviorTreeManipulatorInterface> engine::getBehaviorManipulator()
{
    if (m_behavior_tree_manipulator_ptr == nullptr)
    {
        if (m_system_manager != nullptr)
        {
            m_behavior_tree_manipulator_ptr = std::make_shared<BehaviorTreeManipulator>(m_system_manager);
        }
    }

    return m_behavior_tree_manipulator_ptr;
}

void engine::requestQuit()
{
	m_is_quit_requested = true;
}

std::weak_ptr<sceneManipulatorInterface> engine::getSceneManipulator()
{
	if (m_scene_manipulator_ptr == nullptr)
	{
		if (m_system_manager != nullptr)
		{
            m_scene_manipulator_ptr = std::make_shared<sceneManipulator>(this, m_system_manager, m_go_filter);
			//m_scene_manipulator_ptr = std::shared_ptr<sceneManipulatorInterface>(new sceneManipulator(this, m_system_manager, m_go_filter));
		}
	}

	return m_scene_manipulator_ptr;
}

std::weak_ptr<bundleManipulatorInterface> engine::getBundleManipulator()
{
	if (m_bundle_manipulator_ptr == nullptr)
	{
		if (m_system_manager != nullptr)
		{
            //m_bundle_manipulator_ptr = std::shared_ptr<bundleManipulatorInterface>(new bundleManipulator(m_system_manager));
            m_bundle_manipulator_ptr = std::make_shared<bundleManipulator>(m_system_manager);
		}
	}
	return m_bundle_manipulator_ptr;
}

std::weak_ptr<persistenceManipulatorInterface> engine::getPersistenceManipulator()
{
	if (m_persistence_manipulator_ptr == nullptr)
	{
		if (m_system_manager != nullptr)
		{
            //m_persistence_manipulator_ptr = std::shared_ptr<persistenceManipulator>(new persistenceManipulator(m_system_manager));
            m_persistence_manipulator_ptr = std::make_shared<persistenceManipulator>(m_system_manager);
		}
	}
	return m_persistence_manipulator_ptr;
}

std::weak_ptr<engineMetadataManipulatorInterface> engine::getEngineMetadataManipulator()
{
	if (m_engine_metadata_manipulator_ptr == nullptr)
	{
        //m_engine_metadata_manipulator_ptr = std::shared_ptr<engineMetadataManipulator>(new engineMetadataManipulator());
        m_engine_metadata_manipulator_ptr = std::make_shared<engineMetadataManipulator>();
	}
	return m_engine_metadata_manipulator_ptr;
}

std::weak_ptr<cameraManipulatorInterface> engine::getCameraManipulator()
{
	if(m_camera_manipulator_ptr == nullptr)
	{
		if (m_system_manager != nullptr)
		{
            //m_camera_manipulator_ptr = std::shared_ptr<cameraManipulatorInterface>(new cameraManipulator(m_system_manager));
            m_camera_manipulator_ptr = std::make_shared<cameraManipulator>(m_system_manager);
		}
	}
	return m_camera_manipulator_ptr;
}

std::weak_ptr<assetManipulatorInterface> engine::getAssetManipulator()
{
    if (m_asset_manipulator_ptr == nullptr)
    {
        if (m_system_manager != nullptr)
        {
            //m_asset_manipulator_ptr = std::shared_ptr<assetManipulatorInterface>(new assetManipulator());
            m_asset_manipulator_ptr = std::make_shared<assetManipulator>();
        }
    }
    return m_asset_manipulator_ptr;
}

std::weak_ptr<nodeManipulatorInterface> engine::getNodeManipulator()
{
  if (m_node_manipulator_ptr == nullptr)
  {
    if (m_system_manager != nullptr)
    {
        //m_node_manipulator_ptr = std::shared_ptr<nodeManipulatorInterface>(new nodeManipulator(this, m_system_manager));
        m_node_manipulator_ptr = std::make_shared<nodeManipulator>(this, m_system_manager);
    }
  }

  return m_node_manipulator_ptr;
}

std::weak_ptr<graphicsManipulatorInterface> engine::getGraphicsManipulator()
{
	if (m_graphics_manipulator_ptr == nullptr)
	{
		if (m_system_manager != nullptr)
		{
			m_graphics_manipulator_ptr = std::make_shared<graphicsManipulator>(m_system_manager);
		}
	}

	return m_graphics_manipulator_ptr;
}


////==================================================================================////
////      (Non-)Configurable                                                          ////
////==================================================================================////

////==================================================================================////
////      Overridden/(Non-/Pure-)Virtual                                              ////
////==================================================================================////

//======================================================================================//
//        (Conversion )Constructor / Operators / Getters & Setters / Helper Destructor  //
//======================================================================================//

bool engine::isQuitRequested() const
{
	return m_is_quit_requested;
}

// TODO(Arron): This method does not belong here.  There should be another way to access the current scene,
//        as the user needs to get the sceneSystem themselves.
std::weak_ptr<scene> engine::getCurrentScene()
{
	sceneSystem * l_scene_system = m_system_manager->getSystem<sceneSystem>();
	if (l_scene_system == nullptr)
		return std::weak_ptr<scene>();
	else
		return l_scene_system->getCurrentScene();
}

