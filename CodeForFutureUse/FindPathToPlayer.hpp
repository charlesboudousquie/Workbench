/*!***************************************************************************************
\file       FindPathToPlayer.hpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Finds path to player from actor's current position.
*****************************************************************************************/
#pragma once
#include "Leaf.hpp"

class FindPathToPlayer : public Leaf
{
    public:
        void Init() override;
};

