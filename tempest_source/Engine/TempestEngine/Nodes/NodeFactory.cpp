/*!***************************************************************************************
\file       NodeFactory.cpp
\author     Ryan Booth
\date       9/20/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/

#include "rapidjson.h"
#include "document.h"

#include "NodeFactory.hpp"

#include <unordered_map>
#include <fstream>

//Nodes
#include "Nodes/BaseNode1.hpp"
#include "Nodes/BaseNode2.hpp"

// leaves
#include "Nodes/Behaviors/DefaultLeaf.hpp"
#include "Nodes/Behaviors/JumpUp.hpp"

// decorators
#include "Nodes/Behaviors/Inverter.hpp"
#include "Nodes/Behaviors/Repeater.hpp"
#include "Nodes/Behaviors/RepeatUntilN.hpp"
#include "Nodes/Behaviors/RepeatUntilFailure.hpp"
#include "Nodes/Behaviors/RepeatUntilSuccess.hpp"
#include "Nodes/Behaviors/ReturnTrue.hpp"
#include "Nodes/Behaviors/Timer.hpp"

//composites
#include "Nodes/Behaviors/Selector.hpp"
#include "Nodes/Behaviors/Sequencer.hpp"

const std::string l_fileName("../../Samples/Assets/Nodes.def");

std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> NodeFactory::m_node_factory_list;

static std::map<std::string, std::function<typeRT()>> staticTypes
{
//// leaves
//{"DefaultLeaf", DefaultLeaf::onRender},
//{"JumpUp", JumpUp::onRender},
//
//// composites
//{"Selector", Selector::onRender},
//{"Sequencer", Sequencer::onRender},
//
//// decorators
//{"Inverter",Inverter::onRender},
//{"Repeater", Repeater::onRender},
//{"RepeatUntilN", RepeatUntilN::onRender},
//{"RepeatUntilFailure",RepeatUntilFailure::onRender},
//{"RepeatUntilSuccess", RepeatUntilSuccess::onRender},
//{"ReturnTrue", ReturnTrue::onRender},
////{"Timer", Timer::getStaticData},
};

//static std::map<std::string, std::function<typeRT(const rapidjson::Value &)>> jsonTypes
//{
//    {"Timer", Timer::getJSONData}
//};

// function obsolete
typeRT NodeFactory::getNodeTypeRT(const std::pair<std::string, std::string> & p_node_type)
{
    // if type is known in map
    if (staticTypes.find(p_node_type.first) != staticTypes.end())
    {
        return staticTypes[p_node_type.first]();
    }

    // return nothing
    throw std::exception("Node Factory creating node that has no getStaticData function");
}

//typeRT NodeFactory::getNodeTypeRTFromJSON(const std::pair<std::string, 
//    std::string>& p_node_type, const rapidjson::Value & json_data)
//{
//    // if type is known in map
//    if (jsonTypes.find(p_node_type.first) != jsonTypes.end())
//    {
//        return jsonTypes[p_node_type.first](json_data);
//    }
//
//    // return nothing
//    throw std::exception("Node Factory creating node that has no getJSONData function");
//}

void NodeFactory::parseNodesDefinitions()
{
  std::ifstream l_file(l_fileName);

  if (l_file.good())
  {
    m_node_factory_list.clear();

    std::string l_register;

    while (std::getline(l_file, l_register))
    {
      std::string l_type;

      if (l_register == "\n")
      {
        continue;
      }

      size_t l_parenthesis = l_register.find_first_of('(');

      if (l_parenthesis != std::string::npos)
      {
        l_type = l_register.substr(0, l_parenthesis);

        std::string l_className;

        size_t l_comma = l_register.find_first_of(',') - 1;

        l_className = l_register.substr(l_parenthesis + 1, l_comma - l_parenthesis);

        size_t l_first_quotes = l_register.find_first_of(34) + 1;
        size_t l_last_quotes = l_register.find_last_of(34);

        if (l_first_quotes != std::string::npos && l_last_quotes != std::string::npos)
        {
          std::string l_goodName = l_register.substr(l_first_quotes, l_last_quotes - l_first_quotes);

          std::string l_type_name;
          size_t l_underscore = l_type.find_first_of('_') + 1;
          l_type_name = l_type.substr(l_underscore, l_parenthesis - l_underscore);

          m_node_factory_list[l_type_name].push_back(std::pair<std::string, std::string>(l_className, l_goodName));
        }
        else
        {
          //return std::string("[ERROR] Nodes.def could not be parsed.\n");
        }
      }

    }

    l_file.close();
  }
  else
  {
    //return std::string("[WARNING] Nodes.def not found. Please make sure the Editor executable is in the same folder.\n");
  }

  //return std::string("[NOTICE] Nodes.def successfully parsed.\n");
}

std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> NodeFactory::getNodeDefinitions()
{
  if(m_node_factory_list.empty())
  {
    parseNodesDefinitions();
  }

  return m_node_factory_list;
}

