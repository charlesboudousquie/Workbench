/*!***************************************************************************************
\file       NavMeshWindow.cpp
\author     Henry Brobeck
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "NavMeshWindow.hpp"
#include "Utility/NavMeshGenerator.hpp"
#include "Engine.hpp"
#include "EngineRunner.hpp"
#include "EngineController.hpp"
#include "WaypointNode.hpp"

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
	Editor::navMeshWindow::navMeshWindow(editorWindow * p_parent_editor)
		: windowBase(p_parent_editor)
	{

	}

	void Editor::navMeshWindow::onRender()
	{
		/*
		// render stuff here
		if (ImGui::Button("Generate NavMesh"))
		{
			navMeshGenerator::buildNavMesh();
		}
		if (ImGui::Button("Show Voxel Debug"))
		{
			navMeshGenerator::showVoxelDebug();
		}
		if (ImGui::Button("Hide Voxel Debug"))
		{
			navMeshGenerator::hideVoxelDebug();
		}
		if (ImGui::Button("Show Span Debug"))
		{
			navMeshGenerator::showSpanDebug();
		}
		if (ImGui::Button("Hide Span Debug"))
		{
			navMeshGenerator::hideSpanDebug();
		}
		if (ImGui::Button("Show Top Debug"))
		{
			navMeshGenerator::showTopDebug();
		}
		if (ImGui::Button("Hide Top Debug"))
		{
			navMeshGenerator::hideTopDebug();
		}
		*/
		if (ImGui::Button("Generate Starting Path"))
		{
			getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->dynamicWaypointGraphCreatePath();
		}
		if (ImGui::Button("Generate Node Set Path"))
		{
			getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->dynamicWaypointGraphCreateSetPaths();
		}
		if (ImGui::Button("Stitch Paths"))
		{
			getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->dynamicWaypointGraphStitchPaths();
		}
		if (ImGui::Button("Clear"))
		{
			getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->dynamicWaypointGraphClear();
		}
		if (ImGui::Button("Delete Random Node Set"))
		{
			getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->dynamicWaypointGraphDeleteRandomNodeSet();
		}
		if (ImGui::Button("BIG RED BUTTON"))
		{
			getEngineController().getEngineRunner()->getEngine()->getSceneManipulator().lock()->dynamicWaypointGraphTestFunction();
		}
	}

}
