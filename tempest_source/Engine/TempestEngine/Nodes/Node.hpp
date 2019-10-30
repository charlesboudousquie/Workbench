/*!***************************************************************************************
\file       Node.hpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/
#pragma once

#include <string>
#include <Reflection.hpp>

class Node
{
public:
    Node() : m_id(0), m_depth(0), m_name(""), m_inputs(0), m_outputs(0), m_num_children(0), m_num_parents(0)
    {
    }

  virtual ~Node() = default;

  virtual void updateFromTypeRT(typeRT & p_data) = 0;

  virtual std::pair<bool, std::string> validate() = 0;

  int getId() const { return m_id; }
  void setId(int p_id) { m_id = p_id; }

  const std::string & getName() const { return m_name; }
  void SetName(const std::string& newName) { m_name = newName; }

  int getInputs() const { return m_inputs; }
  int getOutputs() const { return m_outputs; }

  int getNumChildren() const { return m_num_children; }
  int getNumParents() const { return m_num_parents; }

protected:
  void setName(const std::string & p_name) 
  { 
      m_name = p_name; 
  }

  void setIntputs(int p_inputs) { m_inputs = p_inputs; }
  void setOutputs(int p_outputs) { m_outputs = p_outputs; }


private:
  int m_id;
  int m_depth;
  std::string m_name;
 
  int m_inputs;
  int m_outputs;

  int m_num_children;
  int m_num_parents;
};
