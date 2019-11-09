/*!***************************************************************************************
\file       Health.cpp
\author     Charles Boudousquie
\date       10/30/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Health component for npc or player.
*****************************************************************************************/
#include "Health.hpp"
#include "GameObject.hpp"

Health::Health() : health(-1)
{
}

void Health::DecreaseHealth(int amount)
{
  health-= amount;
}

void Health::IncreaseHealth(int amount)
{
  health += amount;
}   
    
int Health::GetHealth()
{
  return health;
}
    
void Health::SetHealth(int amount)
{
  health = amount;
}

bool Health::IsDead()
{
  return health <= 0;
}

void Health::Kill()
{
    health = -1000;
}

componentType const Health::type() const
{
    return getType();
}

typeRT Health::toTypeRT() const
{
    // set type and variable name
    typeRT result;
    result.setTypeName(getType().name());
    result.setVariableName(getType().name());

    // create health
    typeRT healthValue("health", this->health);

    // this should set health
    result.insertMember(healthValue);

    return result;
}

void Health::updateFromTypeRT(typeRT & p_type)
{
    if (p_type.members().find("health") != p_type.members().end())
    {
        this->health = p_type.member("health").getFloat();
    }
    else
    {
        throw std::runtime_error("ERROR updatefrom type rt in health component was missing health value");
    }
}

std::string Health::getQueryableType()
{
    return std::string(type().name());
}

std::string Health::getQueryableID()
{
    if (!getGameObject().expired())
    {
        return std::to_string(getGameObject().lock()->getID());
    }
    else { return "orphan health component"; }
}

std::string Health::getQueryableProperty(std::string p_name)
{
    return toTypeRT().getString();
}

std::vector<std::string> Health::getQueryablePropertyNames()
{
    return { "health" };
}
    