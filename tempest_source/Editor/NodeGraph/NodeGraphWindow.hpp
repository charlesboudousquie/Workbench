/*!***************************************************************************************
\file       NodeGraphWindow.hpp
\author     Ryan Booth
\date       9/10/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "../Scaffolding/WindowBase.hpp"

//======== 3rd Party Includes ==========================================================//
#include <imgui.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
class NodeManager;
class EditorNode;

namespace Editor
{
  //======== Forward Declarations=======================================================//
  class editorWindow;
  class engineController;

  /*!***************************************************************************************
  \par class: nodeGraphWindow
  \brief   This class handles rendering the nodeGraphWindow in the editor
  *****************************************************************************************/
  class nodeGraphWindow : public windowBase
  {
  public:
    /*!***************************************************************************************
    \brief  Constructs the window with the parameters for the bases
    \param p_parent_editor - reference to the editorWindow parent
    \param p_current_state - reference to the editor's current state object
    \param p_engine_controller - reference to the editor's engine controller object
    \param p_selection - reference to the hierarchy selection manager
    *****************************************************************************************/
    nodeGraphWindow(editorWindow * p_parent_editor, /*editorState * p_current_state,*/ engineController * p_engine_controller);

    ~nodeGraphWindow();

    void reset();

    ImVec2 getCanvasSize() const { return m_canvas_size; }

    void saveGraph();

  protected:
    /*!***************************************************************************************
    \brief  Handles the render event for the window, rendering the window appropriately
    *****************************************************************************************/
    void onRender() override;

  private:
    bool m_draw;
    bool m_show_grid;
    bool m_open_context_menu;
    bool m_first_run;

    NodeManager * m_node_manager;

    int m_node_hovered_in_list;
    int m_node_hovered_in_scene;

    ImVec2 m_scrolling;
    ImVec2 m_canvas_size;

    float m_node_slot_radius;
    ImVec2 m_node_window_padding;

    std::string m_graph_name;

    bool m_open_graph_popup;
    bool m_open_graph;

    editorWindow* m_editor_window;

    std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> m_node_definitions;

    void createNodeMenu(ImVec2 p_scene_pos);
    void linkNodeMenu(std::unordered_map<int, EditorNode*>::iterator p_iterator_node, EditorNode* p_node);
    void decoupleNodeMenu(std::unordered_map<int, EditorNode*>::iterator p_iterator_node, EditorNode* p_node);


    void drawNodeLinks(ImDrawList* p_draw_list);
    void drawNodes(ImDrawList * p_draw_list);
    void graphNameRender();
    void openGraphButton();
    void testGraphButton();

    void renderGraphPopup();

    void renderNodeList(); // render left hand side node list
    void renderNodePopup(); // renders popup when user right clicks node

    void decoupleNodeMenu(); // provides menu for decoupling node
  };


} // namespace Editor
