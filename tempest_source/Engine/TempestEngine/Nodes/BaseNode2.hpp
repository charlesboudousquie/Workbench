/*!***************************************************************************************
\file       BaseNode1.hpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief		Example node of a TemplateNode.
*****************************************************************************************/

#pragma once

#include <string>

#include "TemplateNode.hpp"
#include "Color4.hpp"

class BaseNode2 : public TemplateNode
{
public:
  BaseNode2();

  ~BaseNode2();

  static typeRT onRender();

  void updateData(const typeRT& p_data) override;

  std::pair<bool, std::string> validateBase() override;

protected:

private:
  bool m_cool;
  int m_int;
  float m_float;

  color4 m_color;
};