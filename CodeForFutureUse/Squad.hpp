/*!***************************************************************************************
\file       Squad.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents the formation of a squad within the game.
*****************************************************************************************/
#pragma once
#include <vector>

#include "Vector3.hpp"

#include "Factions.hpp"

typedef unsigned long objID;
class space;

// a group of cooperative npcs that stick close together
class Squad
{
    
      space* m_space;
  
      int m_maxRings; // maximum number of rings this squad can have
  
      Faction squadFaction;
      
  public:
    
      Squad(space* space_, int maxRings, Faction faction) 
      : m_space(space_), m_maxRings(maxRings), squadFaction(faction) {}
      
      enum class FormationState{BROKEN, FORMING, COMPLETE};
      
      Faction GetFaction() const;
      
      vector3 GetCenter() const;
      objID getLeaderID() const;
    
      FormationState GetState();
      
      void Update(float dt);
      
      // checks if unit is actually in this squad
      bool HasUnit(objID);
      
      // returns false if squad is full
      bool JoinSquad(objID);
      void LeaveSquad(objID);
  
  private:
  
      FormationState state;
      
      objID leaderID;
      
      std::vector<SquadRing> rings;
};

