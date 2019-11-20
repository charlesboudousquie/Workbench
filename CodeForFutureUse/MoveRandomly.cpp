/*!***************************************************************************************
\file       MoveRandomly.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This behavior just tells the npc to move in some random direction.
*****************************************************************************************/
#include "MoveRandomly.hpp"

const char* maxDistStr = "Max Distance";
const char* minDistStr = "Min Distance";

const static SpecialData specialData
{
  {DATA_TYPE::FLOAT, maxDistStr, -1.0f},
    {DATA_TYPE::FLOAT, minDistStr, -1.0f}
};

void MoveRandomly::updateFromFile(const rapidjson::Value & val)
{
  this->maxDistance = value[maxDistStr].GetFloat();
  this->minDistance = value[minDistStr].GetFloat();
  
}

void MoveRandomly::Init()
{
  // find random position
  auto newDestination = GetRandomWaypoint();
  
  GetBlackboard().setValue<vector3>("randomDestination", newDestination);
  
  Leaf::Init();
}

void MoveRandomly::Update(float)
{
  // epsilon is the acceptable distance we can be from a target.
  float epsilon = 2.0f;
  
  auto actor = getActor(); 
  
  auto destination = GetBlackboard().getValue<vector3>("randomDestination");
  
  float distance = GetDistance(actor, destination);
  
  if(distance <= epsilon)
  {
    GetTask()->SetPhase(BehaviorPhase::DONE);
    GetTask()->SetResult(BehaviorResult::SUCCESS);
  }
}
    
    
    
    
    
    
    
    
    
    