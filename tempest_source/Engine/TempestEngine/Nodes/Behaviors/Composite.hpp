/*!***************************************************************************************
\file       Composite.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents a node in a behavior tree that has multiple child nodes.
*****************************************************************************************/
#pragma once
#include <vector>

#include "BehaviorAgent.hpp"
#include "Behavior.hpp"

class Composite : public Behavior
{
  protected:

      static typeRT compositeOnRender();

    // unique function to composites.
    void SetPhaseOfChildren(BehaviorPhase);

    void Init() override;

  // TODO figure out how iterator will not be violated when adding or removing nodes from vector!!!
  std::vector<BehaviorPtr>::iterator currentChild;
  std::vector<BehaviorPtr> childNodes;

  // NOTE: this class itself should never be constructed.
  // subclasses like selector or sequencer should be constructed instead.
  Composite();
  public:

  std::vector<BehaviorPtr> GetChildren() override;

  BehaviorPtr getCurrentChild() override;


  void addChild(BehaviorPtr);
  void removeChild(BehaviorPtr);
  void clearChildren();
};


