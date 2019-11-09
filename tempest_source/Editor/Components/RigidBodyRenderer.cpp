/*!***************************************************************************************
\file       RigidBodyRenderer.cpp
\author     Ryan Booth
\date       3/8/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief	
*****************************************************************************************/
#include "RigidBodyRenderer.h"
#include "../EngineController.hpp"
#include "../HierarchySelectionKeeper.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "../StyleKeeper.hpp"
#include <Reflection.hpp>

#define BIT(x) (1u<<(x))

Editor::rigidBodyRenderer::rigidBodyRenderer(editorWindow* p_parent_window)
  : componentRenderer(p_parent_window)
{
  auto & l_styles = getStyleKeeper();

  l_styles.setStyle("*", "collisionLayerSelf", "readonly", 1);
  l_styles.setStyle("*", "collisionLayerWith", "readonly", 1);
}

bool Editor::rigidBodyRenderer::onRender(typeRT & p_type_data, objID /* p_editor_object_id */)
{
  auto l_engine_controller = getEngineController();
  auto l_selection = getSelectionKeeper();

  bool l_modified = false;


  if (ImGui::Checkbox("Self: World", &m_collisionLayerSelfWorld))
  {
    setCollisionLayers(m_collisionLayerSelfWorld);
  }
  ImGui::SameLine();
  if (ImGui::Checkbox("Self: Player", &m_collisionLayerSelfPlayer))
  {
    setCollisionLayers(m_collisionLayerSelfPlayer);
  }
  if (ImGui::Checkbox("Self: Enemy", &m_collisionLayerSelfEnemy))
  {
    setCollisionLayers(m_collisionLayerSelfEnemy);
  }
  ImGui::SameLine();
  if (ImGui::Checkbox("Self: Trigger", &m_collisionLayerSelfTrigger))
  {
    setCollisionLayers(m_collisionLayerSelfTrigger);
  }

  if (ImGui::Button("Update Collision Layer: Self"))
  {
	  p_type_data.member("collisionLayerSelf").setUInt(getCollisionLayerSelf());
    //l_engine_controller->getEngineRunner()->getSceneManipulator().lock()->setCollisionLayer(l_selection->getSelectionId(), getCollisionLayerSelf(), 0);
    bool l_reset = false;
    setCollisionLayers(l_reset);

    l_modified = true;
  }

  ImGui::Checkbox("Collides With: World", &m_collisionLayerWithWorld);
  ImGui::SameLine();
  ImGui::Checkbox("Collides With: Player", &m_collisionLayerWithPlayer);
  ImGui::Checkbox("Collides With: Enemy", &m_collisionLayerWithEnemy);
  ImGui::SameLine();
  ImGui::Checkbox("Collides With: Trigger", &m_collisionLayerWithTrigger);

  if (ImGui::Button("Update Collision Layer: Collides With"))
  {
	  p_type_data.member("collisionLayerWith").setUInt(getCollisionLayerWith());
    //l_engine_controller->getEngineRunner()->getSceneManipulator().lock()->setCollisionLayer(l_selection->getSelectionId(), getCollisionLayerWith(), 1);
    bool l_reset = false;
    setCollisionLayersWith(l_reset);

    l_modified = true;
  }

  return l_modified;
}

void Editor::rigidBodyRenderer::setCollisionLayers(bool & p_collisionLayerToBeTrue)
{
  m_collisionLayerSelfWorld = false;
  m_collisionLayerSelfPlayer = false;
  m_collisionLayerSelfEnemy = false;
  m_collisionLayerSelfTrigger = false;
  p_collisionLayerToBeTrue = true;
}

void Editor::rigidBodyRenderer::setCollisionLayersWith(bool& p_collisionLayerToBeTrue)
{
  m_collisionLayerWithWorld = false;
  m_collisionLayerWithPlayer = false;
  m_collisionLayerWithEnemy = false;
  m_collisionLayerWithTrigger = false;
  p_collisionLayerToBeTrue = true;
}

unsigned int Editor::rigidBodyRenderer::getCollisionLayerSelf()
{
  if (m_collisionLayerSelfWorld)
  {
    return BIT(1u);
  }
  else if (m_collisionLayerSelfPlayer)
  {
    return BIT(2u);
  }
  else if (m_collisionLayerSelfEnemy)
  {
    return BIT(3u);
  }
  else if (m_collisionLayerSelfTrigger)
  {
    return BIT(4u);
  }

  return 0u;
}

unsigned Editor::rigidBodyRenderer::getCollisionLayerWith()
{
  unsigned int l_result = 0u;

  if (m_collisionLayerWithWorld)
  {
    l_result = l_result | BIT(1u);
  }
  if (m_collisionLayerWithPlayer)
  {
    l_result = l_result | BIT(2u);
  }
  if (m_collisionLayerWithEnemy)
  {
    l_result = l_result | BIT(3u);
  }
  if (m_collisionLayerWithTrigger)
  {
    l_result = l_result | BIT(4u);
  }

  return l_result;
}