/*!***************************************************************************************
\file       NodeGraphWindow.cpp
\author     Ryan Booth
\date       9/10/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/

//======== 1st Party Includes ==========================================================//
#include "NodeGraphWindow.hpp"
#include "NodeManager.hpp"
#include "EditorNode.hpp"
#include "../EditorWindow.hpp"
#include "BehaviorTreeManipulatorInterface.hpp"

#include "../EngineController.hpp"
#include <EngineRunner.hpp>
#include <Engine.hpp>

#include <iostream>

//======== 3rd Party Includes ==========================================================//
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "../imguifilesystem.h"
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

static const float NODE_SLOT_RADIUS = 4.0f;
static const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

Editor::nodeGraphWindow::nodeGraphWindow(editorWindow * p_parent_editor, engineController * p_engine_controller)
    : windowBase(p_parent_editor), m_editor_window(p_parent_editor), m_draw(true), m_show_grid(true), m_open_context_menu(false), m_first_run(false), m_node_manager(new NodeManager(p_engine_controller, getLogger())),
    m_node_hovered_in_list(-1), m_node_hovered_in_scene(-1), m_node_slot_radius(4.0f), m_node_window_padding(8.0f, 8.0f), m_graph_name("Unnamed_Graph"), m_open_graph(false)
{

}

Editor::nodeGraphWindow::~nodeGraphWindow()
{

}

void Editor::nodeGraphWindow::reset()
{
    m_scrolling = ImVec2(0.0f, 0.0f);
}

void Editor::nodeGraphWindow::saveGraph()
{
    std::string p_path = m_editor_window->getProjectFilePath();
    std::string p_file_name = m_graph_name + ".graph";
    m_node_manager->serializeNodes(p_path, p_file_name);
}

void Editor::nodeGraphWindow::onRender()
{
    if (!ImGui::Begin("Node Graph", &m_draw))
    {
        ImGui::End();
        return;
    }

    if (!m_first_run)
    {
        m_node_manager->retrieveNodeFactoryList();
        m_first_run = true;
    }

    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiSetCond_FirstUseEver);

    // Draw a list of nodes on the left side
    m_open_context_menu = false;
    m_node_hovered_in_list = -1;
    m_node_hovered_in_scene = -1;

    ImGui::BeginChild("node_list", ImVec2(200, 0));
    ImGui::Text("Nodes");
    ImGui::Separator();

    auto l_node_list = m_node_manager->getNodeList();

    for (auto i_node = l_node_list->begin(); i_node != l_node_list->end(); ++i_node)
    {
        const EditorNode * l_node = i_node->second;
        int l_node_id = l_node->getId();

        ImGui::PushID(l_node_id);

        std::string l_display_name = l_node->getName() + ": " + std::to_string(l_node_id);

        if (ImGui::Selectable(l_display_name.c_str(), l_node_id == m_node_manager->getSelectedNodeId()))
        {
            m_node_manager->setSelectedNodeId(l_node_id);
        }

        if (ImGui::IsItemHovered())
        {
            m_node_hovered_in_list = l_node_id;
            m_open_context_menu |= ImGui::IsMouseClicked(1);
        }

        ImGui::PopID();
    }

    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginGroup();

    // Create our child canvas
    ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", m_scrolling.x, m_scrolling.y);
    ImGui::SameLine();
    ImGui::Checkbox("Show grid", &m_show_grid);

    ImGui::SameLine();

    //Graph name code
    graphNameRender();

    //Open Graph Button code
    openGraphButton();

    // test run the current graph
    //testGraphButton();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, IM_COL32(60, 60, 70, 200));
    ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
    ImGui::PushItemWidth(120.0f);

    ImVec2 offset = ImGui::GetCursorScreenPos() + m_scrolling;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Display grid
    if (m_show_grid)
    {
        ImU32 GRID_COLOR = IM_COL32(200, 200, 200, 40);
        float GRID_SZ = 64.0f;
        ImVec2 win_pos = ImGui::GetCursorScreenPos();
        m_canvas_size = ImGui::GetWindowSize();

        for (float x = fmodf(m_scrolling.x, GRID_SZ); x < m_canvas_size.x; x += GRID_SZ)
        {
            draw_list->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, m_canvas_size.y) + win_pos, GRID_COLOR);
        }

        for (float y = fmodf(m_scrolling.y, GRID_SZ); y < m_canvas_size.y; y += GRID_SZ)
        {
            draw_list->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(m_canvas_size.x, y) + win_pos, GRID_COLOR);
        }

    }

    //Display node links
    drawNodeLinks(draw_list);

    //Display nodes
    drawNodes(draw_list);

    draw_list->ChannelsMerge();

    // Open context menu
    if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
    {
        m_node_manager->setSelectedNodeId(-1);
        m_node_hovered_in_list = m_node_hovered_in_scene = -1;
        m_open_context_menu = true;
    }

    if (m_open_context_menu)
    {
        ImGui::OpenPopup("context_menu");

        if (m_node_hovered_in_list != -1)
        {
            m_node_manager->setSelectedNodeId(m_node_hovered_in_list);
        }

        if (m_node_hovered_in_scene != -1)
        {
            m_node_manager->setSelectedNodeId(m_node_hovered_in_scene);
        }
    }

    // Draw context menu (Right click)
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

    m_node_definitions = m_node_manager->getNodeDefinitions();

    if (ImGui::BeginPopup("context_menu"))
    {
        auto i_node = m_node_manager->getSelectedNodeId() != -1 ? l_node_list->find(m_node_manager->getSelectedNodeId())
            : l_node_list->end();
        ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;

        //A node was right clicked
        if (i_node != l_node_list->end())
        {
            EditorNode * l_node = i_node->second;

            ImGui::Text("Node '%s'", l_node->getName().c_str());
            ImGui::Separator();

            if (ImGui::MenuItem("Delete"))
            {
                m_node_manager->removeNode(l_node->getId());
            }

            linkNodeMenu(i_node, l_node);

            decoupleNodeMenu(i_node, l_node);
        }
        else
        {
            createNodeMenu(scene_pos);

            if (ImGui::MenuItem("Validate Graph"))
            {
                //Loop through and have each node validate itself
            }

            if (ImGui::MenuItem("Save Graph"))
            {
                saveGraph();
            }
        }

        ImGui::EndPopup();
    }

    ImGui::PopStyleVar();

    // Scrolling
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
    {
        m_scrolling = m_scrolling + ImGui::GetIO().MouseDelta;
    }

    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::EndGroup();

    ImGui::End();

    renderGraphPopup();
}

void Editor::nodeGraphWindow::createNodeMenu(ImVec2 p_scene_pos)
{
    if (ImGui::BeginMenu("Create Node"))
    {
        // for every node type
        for (auto i_node_type = m_node_definitions.begin(); i_node_type != m_node_definitions.end(); ++i_node_type)
        {
            std::string l_type_name = i_node_type->first;

            if (ImGui::BeginMenu(l_type_name.c_str()))
            {
                for (int i = 0; i < i_node_type->second.size(); ++i)
                {
                    std::string l_good_name = i_node_type->second[i].second;

                    if (ImGui::MenuItem(l_good_name.c_str()))
                    {
                        //Create a new node of this type (need a dedicated factory for this)
                        EditorNode * l_new_node = m_node_manager->createNode(l_type_name, i_node_type->second[i]);

                        if (l_new_node != nullptr)
                        {
                            l_new_node->setPosition(p_scene_pos);

                            // ask node manager for unique id, then add node
                            m_node_manager->addNode(l_new_node, m_node_manager->createUniqueID());
                        }
                    }
                }

                ImGui::EndMenu();
            }
        }

        ImGui::EndMenu();
    }
}

void Editor::nodeGraphWindow::linkNodeMenu(std::unordered_map<int, EditorNode*>::iterator p_iterator_node, EditorNode* p_node)
{
    auto l_node_list = m_node_manager->getNodeList();

    if (ImGui::BeginMenu("Link"))
    {
        for (auto i_node_type = m_node_definitions.begin(); i_node_type != m_node_definitions.end(); ++i_node_type)
        {
            std::string l_type_name = i_node_type->first;

            //Can only link with other nodes of the same graph type
            if (p_iterator_node->second->getTypeName() == l_type_name)
            {
                if (ImGui::BeginMenu(l_type_name.c_str()))
                {
                    for (auto i_link_node = l_node_list->begin(); i_link_node != l_node_list->end(); ++i_link_node)
                    {
                        //Can't link with yourself
                        if (i_link_node->second->getId() != p_node->getId())
                        {
                            std::string l_name = i_link_node->second->getName();
                            l_name.append(": ");
                            l_name.append(std::to_string(i_link_node->second->getId()));

                            if (ImGui::MenuItem(l_name.c_str()))
                            {
                                //Link this node
                                m_node_manager->linkNodes(p_node->getId(), i_link_node->second->getId());
                            }
                        }
                    }

                    ImGui::EndMenu();
                }
            }
        }

        ImGui::EndMenu();
    }
}

void Editor::nodeGraphWindow::decoupleNodeMenu(std::unordered_map<int, EditorNode*>::iterator p_iterator_node, EditorNode* p_node)
{
    if (ImGui::BeginMenu("Decouple"))
    {
        std::vector<NodeLink> l_node_links = p_iterator_node->second->getNodeLinks();

        if (ImGui::BeginMenu("Input"))
        {
            //Loop over NodeLinks list
            for (auto i = 0; i < l_node_links.size(); ++i)
            {
                //Only check input links
                if (l_node_links[i].InputIdx == p_iterator_node->second->getId())
                {
                    EditorNode * l_linked_node = m_node_manager->getNode(l_node_links[i].OutputIdx);

                    if (l_linked_node != nullptr)
                    {
                        std::string l_output_name = l_linked_node->getName();
                        l_output_name.append(": ");
                        l_output_name.append(std::to_string(l_node_links[i].OutputIdx));

                        if (ImGui::MenuItem(l_output_name.c_str()))
                        {
                            //Decouple this node
                            m_node_manager->removeLink(l_linked_node->getId(), p_node->getId());
                        }
                    }

                }

            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Output"))
        {
            //Loop over NodeLinks list
            for (auto i = 0; i < l_node_links.size(); ++i)
            {
                //Only check output links
                if (l_node_links[i].OutputIdx == p_iterator_node->second->getId())
                {
                    EditorNode * l_linked_node = m_node_manager->getNode(l_node_links[i].InputIdx);

                    if (l_linked_node != nullptr)
                    {
                        std::string l_output_name = l_linked_node->getName();
                        l_output_name.append(": ");
                        l_output_name.append(std::to_string(l_node_links[i].InputIdx));

                        if (ImGui::MenuItem(l_output_name.c_str()))
                        {
                            //Decouple this node
                            m_node_manager->removeLink(p_node->getId(), l_linked_node->getId());
                        }
                    }

                }

            }

            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }
}

void Editor::nodeGraphWindow::drawNodeLinks(ImDrawList* p_draw_list)
{
    p_draw_list->ChannelsSplit(2);
    p_draw_list->ChannelsSetCurrent(0); // Background

    ImVec2 l_offset = ImGui::GetCursorScreenPos() + m_scrolling;

    auto l_node_list = m_node_manager->getNodeList();

    //Loop over each node
    for (auto i_node = l_node_list->begin(); i_node != l_node_list->end(); ++i_node)
    {
        std::vector<NodeLink> l_node_links = i_node->second->getNodeLinks();

        //Display each output link
        for (int i = 0; i < l_node_links.size(); ++i)
        {
            if (l_node_links[i].OutputIdx == i_node->second->getId())
            {
                NodeLink l_link = l_node_links[i];

                EditorNode * l_node_input = l_node_list->at(l_link.InputIdx);
                EditorNode * l_node_output = i_node->second;

                ImVec2 l_point1 = l_offset + l_node_input->GetInputSlotPos(l_link.InputSlot);

                ImVec2 l_point2 = l_offset + l_node_output->GetOutputSlotPos(l_link.OutputSlot);

                p_draw_list->AddBezierCurve(l_point1, l_point1 + ImVec2(-50, 0), l_point2 + ImVec2(+50, 0), l_point2, IM_COL32(200, 200, 100, 255), 3.0f);
            }
        }
    }

}

void Editor::nodeGraphWindow::drawNodes(ImDrawList * p_draw_list)
{
    // get mouse offset
    ImVec2 l_offset = ImGui::GetCursorScreenPos() + m_scrolling;

    //  get list of nodes
    auto l_node_list = m_node_manager->getNodeList();

    // check if user is in play mode
    bool l_playing = getEngineController().isPlaying();

    if (l_playing)
    {
        //TODO: Implement this function
        m_node_manager->setActiveNodeData();
    }

    // get node that is currently being processed by the BehaviorTreeManager
    auto idOfActiveNode = getEngineController().getEngineRunner()->getEngine()->getBehaviorManipulator().lock()->getActiveNodeID(nullptr);


    // for all nodes
    for (auto i_node = l_node_list->begin(); i_node != l_node_list->end(); ++i_node)
    {
        // get node and id
        EditorNode * l_node = i_node->second;
        int l_node_id = l_node->getId();

        // check for active status, meaning it is the current node that the Behavior Tree is processing
        bool nodeIsActive = idOfActiveNode == i_node->first ? true : false;
        // end of checking active status
        ImGui::PushID(l_node_id);

        ImVec2 l_node_rect_min = l_offset + l_node->getPosition();

        // Display node contents first
        p_draw_list->ChannelsSetCurrent(1); // Foreground

        bool l_old_any_active = ImGui::IsAnyItemActive();

        ImVec2 l_render_position = l_node_rect_min + NODE_WINDOW_PADDING;

        ImGui::SetCursorScreenPos(l_render_position);

        //EditorNode rendering
        //If Engine is not running, render the data that is saved inside object
        //If Engine is running, render the data that is currently active in engine
        l_node->render(l_playing);

        // Save the size of what we have emitted and whether any of the widgets are being used
        bool node_widgets_active = (!l_old_any_active && ImGui::IsAnyItemActive());
        l_node->getScale() = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
        ImVec2 node_rect_max = l_node_rect_min + l_node->getScale();

        // Display node box
        p_draw_list->ChannelsSetCurrent(0); // Background

        ImGui::SetCursorScreenPos(l_node_rect_min);
        ImGui::InvisibleButton("node", l_node->getScale());

        if (ImGui::IsItemHovered())
        {
            m_node_hovered_in_scene = l_node_id;
            m_open_context_menu |= ImGui::IsMouseClicked(1);
        }

        bool node_moving_active = ImGui::IsItemActive();

        if (node_widgets_active || node_moving_active)
        {
            m_node_manager->setSelectedNodeId(l_node_id);
        }

        if (node_moving_active && ImGui::IsMouseDragging(0))
        {
            l_node->setPosition(l_node->getPosition() + ImGui::GetIO().MouseDelta);
        }

        //Background rendering
        ImVec4 l_colorVec = l_node->getColor();
        ImU32 l_color = IM_COL32(l_colorVec.x, l_colorVec.y, l_colorVec.z, l_colorVec.w);

        float l_highlighted = 0.33f;
        float l_notHighligted = 0.23f;

        //Example for different colors
        //if (l_node->getType() == NodeType::Control_Flow)
        //{
        //	l_highlighted = 0.42f;
        //}

        ImU32 node_bg_color =
            (m_node_hovered_in_list == l_node_id || m_node_hovered_in_scene == l_node_id || (m_node_hovered_in_list == -1
                && m_node_manager->getSelectedNodeId() == l_node_id))
            ? IM_COL32(l_highlighted * l_colorVec.x, l_highlighted * l_colorVec.y, l_highlighted * l_colorVec.z, 200)
            : IM_COL32(l_notHighligted * l_colorVec.x, l_notHighligted * l_colorVec.y, l_notHighligted * l_colorVec.z, 200);

        //For while the game is running
        if (nodeIsActive)
        {
            node_bg_color = IM_COL32(0, 255, 0, 200);
        }
        else
        {
            node_bg_color = IM_COL32(l_highlighted * l_colorVec.x, l_highlighted * l_colorVec.y, l_highlighted * l_colorVec.z, 200);
        }

        //Outline rendering
        p_draw_list->AddRectFilled(l_node_rect_min, node_rect_max, node_bg_color, 4.0f);
        p_draw_list->AddRect(l_node_rect_min, node_rect_max, l_color, 4.0f);

        //Input circle rendering
        for (int slot_idx = 0; slot_idx < l_node->getInputs(); slot_idx++)
        {
            p_draw_list->AddCircleFilled(l_offset + l_node->GetInputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(255, 255, 0, 150));
        }

        //Output circle rendering
        for (int slot_idx = 0; slot_idx < l_node->getOutputs(); slot_idx++)
        {
            p_draw_list->AddCircleFilled(l_offset + l_node->GetOutputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(255, 165, 0, 150));
        }

        ImGui::PopID();
    }
}

void Editor::nodeGraphWindow::openGraphButton()
{
    ImGui::SameLine();
    if (ImGui::Button("Open Graph"))
    {
        m_open_graph = true;
    }
}

//void Editor::nodeGraphWindow::testGraphButton()
//{
//    ImGui::SameLine();
//    if (ImGui::Button("Test Graph"))
//    {
//        std::string fileName = m_graph_name + ".graph";
//        getEngineController().getEngineRunner()->getEngine()->getNodeManipulator().lock()->testTree(fileName);
//    }
//}

void Editor::nodeGraphWindow::renderGraphPopup()
{
    if (m_open_graph)
    {
        static ImGuiFs::Dialog dlg;    // one per dialog (and must be static)
        static std::string chosenPath = m_editor_window->getProjectFilePath() + "/Assets/graphs";
        std::string l_newPath;

        l_newPath = dlg.chooseFileDialog(m_open_graph, chosenPath.c_str());

        l_newPath = dlg.getLastDirectory();

        if (l_newPath != chosenPath)
        {
            chosenPath = l_newPath;
        }

        // If you want to copy the (valid) returned path somewhere, you can use something like:
        static char myPath[4096];

        if (dlg.hasUserJustCancelledDialog())
        {
            m_open_graph = false;
        }

        if (strlen(dlg.getChosenPath()) > 0)
        {
            strcpy(myPath, dlg.getChosenPath());

            //Make sure we are only opening up graph files
            char l_ext[40];
            ImGuiFs::PathGetExtension(myPath, l_ext);

            if (strcmp(l_ext, ".graph") == 0)
            {
                char l_return[4096];
                ImGuiFs::PathGetFileNameWithoutExtension(myPath, l_return);

                //m_logger.AddLog("[EDITOR] Loading graph: %s.\n", l_return);

                m_node_manager->reset();

                m_node_manager->readFromFile(myPath);

                m_graph_name = l_return;

                m_open_graph = false;
            }
            else
            {
                //m_logger.AddLog("[WARNING] %s is not a valid extension.\n Please select a .cee file.\n", l_ext);
            }
        }
    }
}

void Editor::nodeGraphWindow::graphNameRender()
{
    ImGui::SameLine();
    ImGui::PushItemWidth(250.0f);

    constexpr int buff_size = 2048;
    char buffer[buff_size] = { 0 };
    m_graph_name.copy(buffer, sizeof(buffer));
    buffer[buff_size - 1] = '\0';

    if (ImGui::InputText("Graph Name", &buffer[0], buff_size, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        std::string l_new_name(buffer);
        m_graph_name = l_new_name;
    }
}

