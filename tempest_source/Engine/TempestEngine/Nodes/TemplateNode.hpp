/*!***************************************************************************************
\file       TemplateNode.hpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief		This is a template node used for understanding how to build a node class.
*****************************************************************************************/

#pragma once

#include <string>

#include "Node.hpp"

class TemplateNode : public Node
{
public:
	TemplateNode();

	~TemplateNode();

	typeRT toTypeRT() const override;

  void updateFromTypeRT(const typeRT& p_data) override;

	std::pair<bool, std::string> validate() override;

	const std::string & getType() const { return m_type; }

protected:
    virtual typeRT getTypeRT() const = 0;
  virtual void updateData(const typeRT& p_data) = 0;

	virtual std::pair<bool, std::string> validateBase();
private:
	std::string m_type;
};
