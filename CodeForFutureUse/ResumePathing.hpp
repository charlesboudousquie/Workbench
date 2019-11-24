/*!***************************************************************************************
\file       ResumePathing.hpp
\author     Charles Boudousquie
\date       11/22/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Tells squad that the npc is ready to go back to formation.
*****************************************************************************************/
#pragma once
#include "Leaf.hpp"

// Resume pathing script
class ResumePathing : public Leaf
{
    public:
        void Init() override;
};
