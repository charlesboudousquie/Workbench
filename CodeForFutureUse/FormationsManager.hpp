/*!***************************************************************************************
\file       FormationsManager.hpp
\author     Charles Boudousquie
\date       11/19/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Contains all squads within the game.
*****************************************************************************************/
#pragma once

#include <SystemBase.hpp>

#include <vector>

class FormationsManager final : public systemBase
{

    public:
    
        void JoinSquad(objID);
        

    private:
        std::vector<Squad> squads;
};



