#pragma once
#include "Audio/AudioComponent.hpp"
#include "Components/FBXFileComponent.h"
#include "Components/AnimatorComponent.hpp"
#include "GameScripts/AudioScript.h"
#include "GameScripts/BombScript.h"
#include "Components/Button.hpp"
#include "Components/Camera.hpp"
#include "GameScripts/CameraScript.hpp"
#include "GameScripts/ChangeSceneAfterDelayScript.hpp"
#include "GameScripts/CharacterStateScript.h"
#include "GameScripts/DamagePrismScript.hpp"
#include "GameScripts/DynamiteScript.h"
#include "GameScripts/EnemyAIScript.h"
#include "GameScripts/GameManagerScript.h"
#include "GameScripts/GrabbableScript.hpp"
#include "GameScripts/GroundPoundScript.h"
#include "Components/InputComponent.hpp"
#include "GameScripts/InputTestScript.hpp"
#include "Replay/JumbotronComponent.hpp"
#include "GameScripts/JumpPadScript.h"
#include "GameScripts/LariatScript.h"
#include "GameScripts/LifetimeScript.hpp"
#include "Components/Light.hpp"
#include "GameScripts/LightTestScript.hpp"
#include "GameScripts/menuControllerScript.hpp"
#include "Components/ParticleEmitter.hpp"
#include "GameScripts/PlayButtonScript.hpp"
#include "GameScripts/PlayerControllerScript.hpp"
#include "Components/RenderTarget.hpp"
#include "GameScripts/RenderTargetScript.hpp"
#include "Components/Renderer.hpp"
#include "Components/RigidBody.hpp"
#include "GameScripts/ShooterScript.h"
#include "Components/Skybox.hpp"
#include "GameScripts/SpawnerScript.h"
#include "GameScripts/StressTestScript.hpp"
#include "GameScripts/TestingScript.hpp"
#include "GameScripts/TextSampleScript.hpp"
#include "SceneManagement/Transform.hpp"
#include "TransformTestScript.hpp"
#include "DebugLines.hpp"
#include "Components/WaypointNode.hpp"
#include "Components/WaypointNodeSet.hpp"

#define COMPONENT_LIST \
X(Audio::AudioComponent)\
X(FBXFileComponent)\
X(animatorComponent)\
X(audioScript)\
X(bombScript)\
X(button)\
X(camera)\
X(cameraScript)\
X(changeSceneAfterDelayScript)\
X(characterStateScript)\
X(damagePrismScript)\
X(dynamiteScript)\
X(enemyAIScript)\
X(gameManagerScript)\
X(grabbableScript)\
X(groundPoundScript)\
X(inputComponent)\
X(inputTestScript)\
X(jumbotronComponent)\
X(jumpPadScript)\
X(lariatScript)\
X(lifetimeScript)\
X(light)\
X(lightTestScript)\
X(menuControllerScript)\
X(particleEmitter)\
X(playButtonScript)\
X(playerControllerScript)\
X(renderTarget)\
X(renderTargetScript)\
X(renderer)\
X(rigidBody)\
X(shooterScript)\
X(skybox)\
X(spawnerScript)\
X(stressTestScript)\
X(testingScript)\
X(textSampleScript)\
X(transform)\
X(transformTestScript)\
X(debugLines)\
X(waypointNode)\
X(waypointNodeSet)\

#define SCRIPT_LIST \
X(audioScript)\
X(bombScript)\
X(cameraScript)\
X(changeSceneAfterDelayScript)\
X(characterStateScript)\
X(damagePrismScript)\
X(dynamiteScript)\
X(enemyAIScript)\
X(gameManagerScript)\
X(grabbableScript)\
X(groundPoundScript)\
X(inputTestScript)\
X(jumpPadScript)\
X(lariatScript)\
X(lifetimeScript)\
X(lightTestScript)\
X(menuControllerScript)\
X(playButtonScript)\
X(playerControllerScript)\
X(renderTargetScript)\
X(shooterScript)\
X(spawnerScript)\
X(stressTestScript)\
X(testingScript)\
X(textSampleScript)\
X(transformTestScript)\

