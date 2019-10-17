/*!***************************************************************************************
\file       Agent.hpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This component is used for our AI system which includes behavior trees.
*****************************************************************************************/
#include "Component.hpp"
#include "QueryableInterface.hpp"

class Agent : public componentCRTP<Agent>, public queryableInterface
{
    // name of the tree that the agent uses to conduct its behavior.
    std::string treeName;

public:
    /*!*******************************************************************************
    \brief  Retrieves the type of component this instance is. SceneSystem requirement.
    \return componentType - The type of this component.
    *********************************************************************************/
    virtual componentType const type() const override;

    /*!*******************************************************************************
    \brief  Converts this into a type
    \return Type equivalent.
    *********************************************************************************/
    virtual typeRT toTypeRT() const override;

    virtual void updateFromTypeRT(typeRT & p_type) override;

    static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Agent", true, true, true); }


    /*!***************************************************************************************
        \brief  Gets component type
        \return The component type
        *****************************************************************************************/
    static componentType const getType() { static componentType const type = componentType::forConcrete<Agent>(); return type; }


    /*!***************************************************************************************
    \brief  Retrieves the type this class, for use with the queryable interface.
    \return string - The type of class this represents.
    *****************************************************************************************/
    std::string getQueryableType() override;

    /*!***************************************************************************************
    \brief  Retrieves the ID of this object's parent, given the parent is valid
    \return string - The ID of the object.
    *****************************************************************************************/
    std::string getQueryableID() override;

    /*!***************************************************************************************
    \brief  Retrieves a property of the object by name
    \param p_name the name of the property(data)
    \return string - Returns the desired data
    *****************************************************************************************/
    std::string getQueryableProperty(std::string p_name) override;

    /*!***************************************************************************************
    \brief  Retrieves a property of the object by name
    \param p_name the name of the property(data)
    \return string - Returns the desired data
    *****************************************************************************************/
    std::vector<std::string> getQueryablePropertyNames() override;

    void setTreeName(const std::string &);
    std::string getTreeName();

};

