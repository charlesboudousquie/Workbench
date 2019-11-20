/*!***************************************************************************************
\file       SquadRing.cpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Each squad may have 1 or more ring formations around it. This class
            represents that formation.
*****************************************************************************************/
#include "SquadRing.hpp"

#include "Space.hpp"
  
bool HasUnit(objID id)
{
    return slots.find(id) != slots.end();
}
  
bool SquadRing::Full()
{
    return slots.size() == maxCapacity;
}

bool SquadRing::Empty()
{
    return slots.empty();
}

int SquadRing::SpaceLeft()
{
    return maxCapacity - slots.size();
}

objID SquadRing::AvailableUnit()
{
    return slots.begin()->first;
}

void SquadRing::Update(float dt)
{
    float epsilon = 0.1f;
    
    // remove any dead units
    RemoveDeadUnits();
    
    CalculatePositions();
    
    // for each unit
    for(auto slot : slots)
    {
        // get unit object
        auto actor = m_space->getGameObjectByID(slot->first);
        vector3 actorPos = actor->getComponent<transform>().getPosition();
        
        
        // find distance between unit and 
        auto slotPos = slot.second;
        vector3 vecToslot = slotPos - actorPos; // check this
        
        float dist = vecToslot.distance();
        
        // if distance from designated slot is too much
        if(dist > 0.1f)
        {
          // normalize vector
          vecToslot = vecToslot.normalize();
          
          // get rigidbody
          auto & rb = actor->getComponent<rigidBody>();
          
          // push object to destination
          rb->applyImpulse(vecToslot * maxSpeed * dt); // max speed????
          
        }
        
    }
}

void SquadRing::Insert(objID id)
{
    assert(slots.find(id) == slots.end());
    assert(slots.size() < maxCapacity);
  
    int currentsize = slots.size();
    
    auto newPos = radius * {sin(currentsize * 2 * PI / maxCapacity), 0, 
                   cos(currentsize * 2 * PI / maxCapacity)};
    
    slots[id] = newPos;
}

void SquadRing::Remove(objID id)
{
    slots.erase(id);
}

void SquadRing::TakeUnits(SquadRing& rhs)
{   
    
    // while we aint full
    while(!this->Full() && !rhs.Empty())
    {
        objID unit = rhs.AvailableUnit();
        this->Insert(unit);
        rhs.Remove(unit);
    }
}

void SquadRing::CalculatePositions()
{
    
    if(slots.size() == 0){return;}
    
    // get leader position
    auto leaderPos = m_space->getGameObjectByID(leaderID)->getComponent<transform>()->getPosition();
    
    int index = 0;
    
    // for each slot, update its position
    for(auto slot : slots)
    {
      // recalculate positions based on unit count
      auto newPos = leaderPos;
      newPos += radius * {sin(index * 2 * PI / maxCapacity), 0, cos(index * 2 * PI / maxCapacity)};
                   
      *slot = newPos;
      index++;
    }
    
}

void SquadRing::RemoveDeadUnits()
{
  
   for (auto it = slots.begin(); it != slots.end();) 
   {
       if(space->getGameObjectByID(it->first)) 
       {
           it = slots.erase(it);
       }
       else
       {
           it++;
       }
     
   }
}




