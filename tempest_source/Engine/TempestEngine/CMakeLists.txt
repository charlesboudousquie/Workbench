cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../Programs/cmake-precompiled-header")
include(PrecompiledHeader)

# dependencies

find_package(GLEWInternal REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLM REQUIRED)
#find_package(BulletInternal REQUIRED)
find_package(FBX REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(FreetypeInternal REQUIRED)
find_package(STB REQUIRED)

if(WIN32)
    find_package(XINPUT REQUIRED)
else()
    find_package(XInput REQUIRED)
    find_package(LinuxWinDefs REQUIRED)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions -fdeclspec")
endif()

add_library(TempestEngine STATIC
    Precompiled.cpp # this MUST be the first entry in the files list (for cotire)
    Precompiled.h
    Application.hpp Application.cpp
    Blackboard.hpp Blackboard.cpp
    DLLControl.hpp
    Engine.hpp Engine.cpp
    FilterBase.h
    GameObjectCollector.hpp
    GameObjectGatherer.hpp
#    GamepadControls.hpp GamepadControls.cpp # TODO: Fix this!
    GamePadSystem.hpp GamePadSystem.cpp
    LeakDetection.hpp
    MeshDebug.hpp MeshDebug.cpp
    PersistenceManipulator.hpp PersistenceManipulator.cpp
    PersistenceManipulatorInterface.hpp
    PersistenceRequest.hpp PersistenceRequest.cpp
    PersistenceSystem.hpp PersistenceSystem.cpp
    ScriptMaps.hpp ScriptMaps.cpp
    Serialization.h Serialization.cpp
    SystemBase.hpp SystemBase.cpp
    SystemBundle.hpp SystemBundle.cpp
    SystemManager.hpp SystemManager.cpp
    SystemTask.hpp SystemTask.cpp
    TransformTestScript.hpp TransformTestScript.cpp

    ../../EngineInterfaces/Public/IComponentRepository.hpp ../../ComponentRepository/Private/ComponentRepository.cpp
    Actions/Action.hpp Actions/Action.cpp
    Actions/ActionSystem.hpp Actions/ActionSystem.cpp
    Actions/DelayAction.hpp Actions/DelayAction.cpp
    Actions/LerpAction.hpp Actions/LerpAction.cpp

    Audio/AudioComponent.hpp Audio/AudioComponent.cpp
    Audio/AudioSystem.h Audio/AudioSystem.cpp

    Components/Agent.hpp Components/Agent.cpp
    Components/AnimatorComponent.hpp Components/AnimatorComponent.cpp
    Components/Button.hpp Components/Button.cpp
    Components/Camera.hpp Components/Camera.cpp
    Components/FBXFileComponent.h Components/FBXFileComponent.cpp
    Components/InputComponent.hpp Components/InputComponent.cpp
    Components/Light.hpp Components/Light.cpp
    Components/ParticleEmitter.hpp Components/ParticleEmitter.cpp
    Components/Renderer.hpp Components/Renderer.cpp
    Components/RenderTarget.hpp Components/RenderTarget.cpp
    Components/RigidBody.hpp Components/RigidBody.cpp
    Components/ScriptCPP.hpp Components/ScriptCPP.cpp
    Components/Skybox.hpp Components/Skybox.cpp
    Components/WaypointNode.hpp Components/WaypointNode.cpp
    Components/WaypointNodeSet.hpp Components/WaypointNodeSet.cpp

    DynamicWaypoints/WaypointGraphPathfinder.hpp DynamicWaypoints/WaypointGraphPathfinder.cpp
    DynamicWaypoints/DynamicWaypointGraph.hpp DynamicWaypoints/DynamicWaypointGraph.cpp

    Events/CollisionEvent.hpp
    Events/TestEvent.hpp

    External/AssetManipulator.hpp External/AssetManipulator.cpp
    External/AssetManipulatorInterface.hpp
    External/bundleManipulator.hpp External/bundleManipulator.cpp
    External/bundleManipulatorInterface.hpp
    External/CameraManipulator.hpp External/CameraManipulator.cpp
    External/CameraManipulatorInterface.hpp
    External/EngineMetadataManipulator.hpp External/EngineMetadataManipulator.cpp
    External/EngineMetadataManipulatorInterface.hpp
	External/GraphicsManipulatorInterface.hpp
	External/GraphicsManipulator.hpp External/GraphicsManipulator.cpp
    External/NodeManipulator.hpp External/NodeManipulator.cpp
    External/NodeManipulatorInterface.hpp
    External/SceneEntityReference.hpp External/SceneEntityReference.cpp
    External/SceneManipulator.hpp External/SceneManipulator.cpp
    External/SceneManipulatorInterface.hpp
    External/BehaviorTreeManipulator.hpp External/BehaviorTreeManipulator.cpp

    GameObjectFiltering/ComponentTypeSet.hpp GameObjectFiltering/ComponentTypeSet.cpp
    GameObjectFiltering/GameObjectFilter.hpp GameObjectFiltering/GameObjectFilter.cpp
    GameObjectFiltering/ListBuilder.hpp GameObjectFiltering/ListBuilder.cpp

    GameScripts/AudioScript.h GameScripts/AudioScript.cpp
    GameScripts/BombScript.h GameScripts/BombScript.cpp
    GameScripts/CameraScript.hpp GameScripts/CameraScript.cpp
    GameScripts/ChangeSceneAfterDelayScript.hpp GameScripts/ChangeSceneAfterDelayScript.cpp
    GameScripts/CharacterStateScript.h GameScripts/CharacterStateScript.cpp
    GameScripts/DamagePrismScript.hpp GameScripts/DamagePrismScript.cpp
    GameScripts/DynamiteScript.h GameScripts/DynamiteScript.cpp
    GameScripts/EnemyAIScript.h GameScripts/EnemyAIScript.cpp
    GameScripts/GameManagerScript.h GameScripts/GameManagerScript.cpp
    GameScripts/GrabbableScript.hpp GameScripts/GrabbableScript.cpp
    GameScripts/GroundPoundScript.h GameScripts/GroundPoundScript.cpp
    GameScripts/InputTestScript.hpp GameScripts/InputTestScript.cpp
    GameScripts/JumpPadScript.h GameScripts/JumpPadScript.cpp
    GameScripts/LariatScript.h GameScripts/LariatScript.cpp
    GameScripts/LifetimeScript.hpp GameScripts/LifetimeScript.cpp
    GameScripts/LightTestScript.hpp GameScripts/LightTestScript.cpp
    GameScripts/menuControllerScript.hpp GameScripts/menuControllerScript.cpp
    GameScripts/PlayButtonScript.hpp GameScripts/PlayButtonScript.cpp
    GameScripts/PlayerControllerScript.hpp GameScripts/PlayerControllerScript.cpp
    GameScripts/PrebuildTestScript.h GameScripts/PrebuildTestScript.cpp
    GameScripts/RenderTargetScript.hpp GameScripts/RenderTargetScript.cpp
    GameScripts/ShooterScript.h GameScripts/ShooterScript.cpp
    GameScripts/SpawnerScript.h GameScripts/SpawnerScript.cpp
    GameScripts/StressTestScript.hpp GameScripts/StressTestScript.cpp
    GameScripts/TestingScript.hpp GameScripts/TestingScript.cpp
    GameScripts/TextSampleScript.hpp GameScripts/TextSampleScript.cpp

    Input/GamepadControlDefines.hpp
    Input/GamepadInputAdapter.hpp Input/GamepadInputAdapter.cpp
    Input/InputSystem.hpp Input/InputSystem.cpp

    Messaging/Message.hpp

    # Behavior Tree Section

    BehaviorEnums.hpp
    BehaviorTree.hpp BehaviorTree.cpp
    BehaviorTreeManager.hpp BehaviorTreeManager.cpp
    BehaviorTreeBuilder.hpp BehaviorTreeBuilder.cpp
    BehaviorTreeDataBase.hpp BehaviorTreeDataBase.cpp
    BehaviorTask.hpp BehaviorTask.cpp
    #BehaviorTaskProxy.hpp BehaviorTaskProxy.cpp

    BehaviorVisitor.hpp BehaviorVisitor.cpp
    Nodes/Behaviors/Behavior.hpp Nodes/Behaviors/Behavior.cpp
    Nodes/Node.hpp Nodes/Node.cpp
    Nodes/NodeFactory.hpp Nodes/NodeFactory.cpp
    Nodes/TemplateNode.hpp Nodes/TemplateNode.cpp
    Nodes/BaseNode1.hpp Nodes/BaseNode1.cpp
    Nodes/BaseNode2.hpp Nodes/BaseNode2.cpp
    # leaves
    Nodes/Behaviors/Leaf.hpp Nodes/Behaviors/Leaf.cpp 
    Nodes/Behaviors/DefaultLeaf.hpp Nodes/Behaviors/DefaultLeaf.cpp
    Nodes/Behaviors/Repeater.hpp Nodes/Behaviors/Repeater.cpp
    Nodes/Behaviors/JumpUp.hpp Nodes/Behaviors/JumpUp.cpp

    #Decorators
    Nodes/Behaviors/Timer.hpp Nodes/Behaviors/Timer.cpp
    Nodes/Behaviors/Inverter.hpp Nodes/Behaviors/Inverter.cpp
    Nodes/Behaviors/Decorator.hpp Nodes/Behaviors/Decorator.cpp
    Nodes/Behaviors/ReturnTrue.hpp Nodes/Behaviors/ReturnTrue.cpp
    Nodes/Behaviors/RepeatUntilN.hpp Nodes/Behaviors/RepeatUntilN.cpp
    Nodes/Behaviors/RepeatUntilFailure.hpp Nodes/Behaviors/RepeatUntilFailure.cpp
    Nodes/Behaviors/RepeatUntilSuccess.hpp Nodes/Behaviors/RepeatUntilSuccess.cpp

    #Composites
    Nodes/Behaviors/Composite.hpp Nodes/Behaviors/Composite.cpp
    Nodes/Behaviors/Selector.hpp Nodes/Behaviors/Selector.cpp
    Nodes/Behaviors/Sequencer.hpp Nodes/Behaviors/Sequencer.cpp


    # End of Behavior Tree Section

    Physics/PhysicsSystem.hpp Physics/PhysicsSystem.cpp

    Platform/PlatformWindowing.hpp Platform/PlatformWindowing.cpp

    Rendering/Buffer.hpp
    Rendering/CameraBase.hpp Rendering/CameraBase.cpp
    Rendering/CameraSystem.hpp Rendering/CameraSystem.cpp
    Rendering/CubeTexture.hpp Rendering/CubeTexture.cpp
    Rendering/FrameBuffer.hpp Rendering/FrameBuffer.cpp
    Rendering/FrameBufferManager.hpp Rendering/FrameBufferManager.cpp
    Rendering/GLDebug.hpp Rendering/GLDebug.cpp
#    Rendering/GLoader.hpp Rendering/GLoader.cpp
    Rendering/GraphicsSystem.hpp Rendering/GraphicsSystem.cpp
    Rendering/IndexBufferObject.hpp Rendering/IndexBufferObject.cpp
    Rendering/LineMesh.cpp
    Rendering/DepthTexture.hpp Rendering/DepthTexture.cpp
    Rendering/DebugLines.hpp Rendering/DebugLines.cpp
    Rendering/LineBuffer.hpp Rendering/LineBuffer.cpp
    Rendering/LineSegment.hpp Rendering/LineSegment.cpp
    Rendering/Material.hpp Rendering/Material.cpp
#    Rendering/MaterialManager.hpp Rendering/MaterialManager.cpp
    Rendering/MaterialLoader.hpp Rendering/MaterialLoader.cpp
    Rendering/Mesh.hpp Rendering/Mesh.cpp
    Rendering/MeshBuilder.hpp Rendering/MeshBuilder.cpp
    Rendering/ModularFrameBuffer.hpp Rendering/ModularFrameBuffer.cpp
    Rendering/ParticleBuffer.hpp Rendering/ParticleBuffer.cpp
    Rendering/ParticleMesh.hpp Rendering/ParticleMesh.cpp
    Rendering/ParticleSystem.hpp Rendering/ParticleSystem.cpp
    Rendering/ProgramPipeline.hpp Rendering/ProgramPipeline.cpp
    Rendering/Shader.hpp Rendering/Shader.cpp
    Rendering/ShaderManager.hpp Rendering/ShaderManager.cpp
    Rendering/ShaderProgram.hpp Rendering/ShaderProgram.cpp
    Rendering/SimpleTexture.hpp Rendering/SimpleTexture.cpp
    Rendering/TextRenderer.hpp Rendering/TextRenderer.cpp
    Rendering/Texture.hpp Rendering/Texture.cpp
    Rendering/TextureManager.hpp Rendering/TextureManager.cpp
    Rendering/TriangleMesh.hpp Rendering/TriangleMesh.cpp
    Rendering/TypeData.hpp
    Rendering/VertexArrayManager.hpp Rendering/VertexArrayManager.cpp
    Rendering/VertexArrayObject.hpp Rendering/VertexArrayObject.cpp
    Rendering/VertexBufferObject.hpp Rendering/VertexBufferObject.cpp
    #Rendering/Windowing.hpp Rendering/Windowing.cpp # TODO: Fix this!

    Replay/InstantReplaySystem.hpp Replay/InstantReplaySystem.cpp
    Replay/JumbotronComponent.hpp Replay/JumbotronComponent.cpp

    SceneManagement/ComponentList.hpp SceneManagement/ComponentList.cpp
    SceneManagement/GameObject.hpp SceneManagement/GameObject.cpp
    SceneManagement/GameObjectSerialization.hpp SceneManagement/GameObjectSerialization.cpp
    SceneManagement/Scene.hpp SceneManagement/Scene.cpp
    SceneManagement/SceneSystem.hpp SceneManagement/SceneSystem.cpp
    SceneManagement/Space.hpp SceneManagement/Space.cpp
    SceneManagement/Transform.hpp SceneManagement/Transform.cpp
    SceneManagement/UISystem.hpp SceneManagement/UISystem.cpp

    Scripting/ScriptingSystem.hpp Scripting/ScriptingSystem.cpp

    SystemManagement/SystemManagerInterface.hpp
    SystemManagement/SystemManagerSTA.hpp SystemManagement/SystemManagerSTA.cpp
    SystemManagement/SystemMonitor.hpp SystemManagement/SystemMonitor.cpp

    Systems/AnimationSystem.hpp Systems/AnimationSystem.cpp
    Systems/FBXLoader.hpp Systems/FBXLoader.cpp

    Telemetry/InjectableInterface.hpp Telemetry/InjectableInterface.cpp
    Telemetry/QueryableInterface.hpp
    Telemetry/SQLConnection.hpp Telemetry/SQLConnection.cpp
    Telemetry/TelemetryComponent.hpp Telemetry/TelemetryComponent.cpp
    Telemetry/TelemetrySystem.hpp Telemetry/TelemetrySystem.cpp

    Threading/JobManager.hpp Threading/JobManager.cpp
    Threading/Task.hpp Threading/Task.cpp

    Utility/AssetManager.hpp Utility/AssetManager.cpp
    Utility/Debug.hpp Utility/Debug.cpp
    Utility/GeneralizedFileLoader.hpp Utility/GeneralizedFileLoader.cpp
    Utility/NavMeshGenerator.hpp Utility/NavMeshGenerator.cpp
    Utility/TextureLoader.hpp Utility/TextureLoader.cpp
    ComponentFactory.cpp
    ComponentFactory.hpp
    ComponentLists.h
    )

target_include_directories(TempestEngine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC Actions
    PUBLIC Audio
    PUBLIC Components
    PUBLIC Events
    PUBLIC External
    PUBLIC GameObjectFiltering
    PUBLIC GameScripts
    PUBLIC Input
    PUBLIC Logging
    PUBLIC Math
    PUBLIC Messaging
    PUBLIC Metadata
    PUBLIC Physics
    PUBLIC Platform
    PUBLIC Prebuild
    PUBLIC Rendering
    PUBLIC Replay
    PUBLIC SceneManagement
    PUBLIC Scripting
    PUBLIC SystemManagement
    PUBLIC Systems
    PUBLIC Telemetry
    PUBLIC Threading
    PUBLIC Utility
    PRIVATE ${GLM_INCLUDE_DIRS}
    PRIVATE ${RapidJSON_INCLUDE_DIRS}
    PRIVATE ${STB_INCLUDE_DIRS}
    PRIVATE ${LinuxWinDefs_INCLUDES}
)

if(UNIX)
    target_link_libraries(TempestEngine
        PRIVATE stdc++fs
        PRIVATE LinuxWinDefs
        PRIVATE ${XInput_LIBRARIES}
    )
endif()

target_link_libraries(TempestEngine
    PRIVATE Logger
    PRIVATE Math
    PRIVATE MathSerialization
    PUBLIC Configuration
    PUBLIC EngineInterfaces
    PUBLIC SerializationCore
    PUBLIC ComponentCore
    PRIVATE SystemCore
    PRIVATE Graphics.OpenGL4
    PRIVATE Physics.Bullet
    PRIVATE AudioEngine
    PRIVATE GLEWInternal
    PRIVATE FBX
    PRIVATE FreetypeInternal
    #PRIVATE BulletInternal
    PRIVATE BulletCollision
    PRIVATE BulletDynamics
    PRIVATE LinearMath
    PRIVATE GLFW
    PRIVATE XInput
    PRIVATE ComponentRepository
    PRIVATE EventSystem
)


# build up resources list
list(APPEND resources ${Graphics.OpenGL4_RESOURCES})
list(APPEND resources ${AudioEngine_RESOURCES})
list(APPEND resources ${GLEWInternal_RESOURCES})
list(APPEND resources ${FBX_RESOURCES})
list(APPEND resources ${FreetypeInternal_RESOURCES})
list(APPEND resources ${GLFW_RESOURCES})
set(TempestEngine_RESOURCES ${resources} CACHE STRING "TempestEngine_RESOURCES" FORCE)


set_target_properties(TempestEngine PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)

## setup the precompiled header
add_precompiled_header(TempestEngine Precompiled.h
    SOURCE_CXX Precompiled.cpp FORCEINCLUDE)
