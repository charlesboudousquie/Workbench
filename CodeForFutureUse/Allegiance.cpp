/*!***************************************************************************************
\file       Allegiance.cpp
\author     Charles Boudousquie
\date       11/4/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Component to set who this actor/npc will side with during battle.
*****************************************************************************************/
#include "Allegiance.hpp"

Faction Allegiance::getFaction()
{
  return faction;
}

componentType const Allegiance::type() const
{
    return getType();
}

typeRT Allegiance::toTypeRT() const
{
    // set type and variable name
    typeRT result;
    result.setTypeName(getType().name());
    result.setVariableName(getType().name());

    // create faction
    typeRT factionType("faction", this->faction);

    // this should set faction
    result.insertMember(factionType);

    return result;
}

void Allegiance::updateFromTypeRT(typeRT & p_type)
{
    if (p_type.members().find("faction") != p_type.members().end())
    {
        this->faction = p_type.member("faction").getInt();
    }
    else
    {
        throw std::runtime_error("ERROR updatefrom type rt in allegiance component was missing faction");
    }
}

std::string Allegiance::getQueryableType()
{
    return std::string(type().name());
}

std::string Allegiance::getQueryableID()
{
    if (!getGameObject().expired())
    {
        return std::to_string(getGameObject().lock()->getID());
    }
    else { return "orphan Allegiance component"; }
}

std::string Allegiance::getQueryableProperty(std::string p_name)
{
    return toTypeRT().getString();
}

std::vector<std::string> Allegiance::getQueryablePropertyNames()
{
    return { "faction" };
}










