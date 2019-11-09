/*!***************************************************************************************
\file       MoveTowardsEnemy.cpp
\author     Charles Boudousquie
\date       11/4/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This behavior steers object towards some marked enemy. NOTE:
            Enemy should be "marked" before calling this function.
*****************************************************************************************/


void Init()
{
  Blackboard& blackboard = GetBlackboard();
  
  // set enemy pos
  this->enemyPos = blackboard.getValue("NearestEnemyPos");
  
  Leaf::Init();
  
}

void Update(float)
{
  
}



