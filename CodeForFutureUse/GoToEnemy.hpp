/*!***************************************************************************************
\file       GoToEnemy.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Pushes npc to the nearest enemy.
*****************************************************************************************/
#pragma once
#include "Leaf.hpp"

#ifndef TESTING_NODES

class RepeatUntilN : public Leaf
{
    
public:

    void Init()override;
    void Update(float) override;
};
#endif
