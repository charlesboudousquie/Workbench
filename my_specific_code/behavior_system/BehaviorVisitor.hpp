/*!***************************************************************************************
\file       BehaviorVisitor.hpp
\author     Charles Boudousquie
\date       10/25/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      creates typeRT data for the renderer to use
*****************************************************************************************/
#pragma once
#include <string>
#include "Reflection.hpp"
#include "rapidjson.h"
#include "document.h"

class BehaviorVisitor
{
    BehaviorVisitor() {}
public:

    bool BehaviorIsSpecialized(const std::string& name);

    static BehaviorVisitor& GetInstance()
    {
        static BehaviorVisitor instance;
        return instance;
    }

    // default renderization
    typeRT GetRenderData(const std::string& name);
    // specialized renderization
    typeRT GetRenderData(const rapidjson::Value &, const std::string& name);

    // if there is any special data inside the behavior that needs to be saved
    // to disk, then call this function.
    // If there is no special data then DO NOT call this function
    void SaveToFile(const std::string& name, typeRT& data, rapidjson::Document& doc);

};


