/*!***************************************************************************************
\file       Squad.cpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents the formation of a squad within the game.
*****************************************************************************************/
#include "Squad.hpp"
#include "Space.hpp"

FormationState Squad::GetState()
{
    return state;
}

Faction Squad::GetFaction() const
{
    return squadFaction;
}

bool HasUnit(objID id)
{
    for(const auto& ring : rings)
    {
        if(ring.HasUnit(id))
        {
            return true;
        }
    }
    
    return false;
}

void Squad::JoinSquad(objID id)
{
    // insert into last ring
    rings.back().Insert(id);
}

void Squad::LeaveSquad(objID)
{
    
}

void Squad::Update(float dt)
{
  
  // if leader died then break up squad
  if(m_space->getGameObjectByID(leaderID) == nullptr)
  {
      // ?????
  }
  
  // for each ring check if it is not filled, if so take squad members from 
  // outside ring.
  for(auto ring = rings.begin(); ring != rings.end();)
  {
      // if we have room
      if(!ring->Full())
      {
          auto nextRing = std::next(ring);
          // if not at end
          if(nextRing != rings.end())
          {
              // if next ring actually has units we can take
              if(!nextRing->Empty())
              {
                  ring->TakeUnits(nextRing);
              }
          }
      }
      
      ring++;
  }
  
  // for each ring push units such that they are in their correct positions.
  for(auto& ring : rings)
  {
      ring.Update(dt);
  }
  
}

