/*!***************************************************************************************
\file       PausePathing.hpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Tells squad that the npc wishes to break from formation.
*****************************************************************************************/
#pragma once
#include "Leaf.hpp"

// Pause pathing script
class PausePathing : public Leaf
{
    public:
        void Init() override;
};


