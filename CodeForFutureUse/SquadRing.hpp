/*!***************************************************************************************
\file       SquadRing.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Each squad may have 1 or more ring formations around it. This class
            represents that formation.
*****************************************************************************************/
#pragma once
#include <unordered_map>

#include "Vector3.hpp"

typedef unsigned long objID;

class space;

// a structure representing a circle formation
// around the player.
class SquadRing
{
  
      int maxCapacity;
      int radius;
      
      space* m_space;
      
      // slots for each npc in ring
      std::unordered_map<objID, vector3> slots;
      
      void RemoveDeadUnits();
  
  public:
  
      objID AvailableUnit();
      
      SquadRing(int maxCapacity_, int radius_, space* space_) 
        : maxCapacity(maxCapacity_), radius(radius_), m_space(space_){}
      
      bool Full();
      bool Empty();
      int  SpaceLeft();
      
      bool HasUnit(objID);
      
      void TakeUnits(SquadRing& rhs);
      
      void Insert(objID);
      void Remove(objID);
      
      void Update(float dt);
      
      void CalculatePositions();
  
};


