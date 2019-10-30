/*!***************************************************************************************
\file       NodeManipulatorInterface.hpp
\author     Ryan Booth
\date       9/20/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/
#pragma once

#include <Reflection.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

//#include <rapidjson.h>
//#include "document.h"

class nodeManipulatorInterface
{
public:
  virtual std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> getNodeDefinitions() = 0;

  //virtual typeRT getStaticData(const std::pair<std::string, std::string> & p_node_type) = 0;

  /*virtual typeRT getJSONData(const std::pair<std::string, std::string> & p_node_type,
      const rapidjson::Value &) = 0;*/

};
