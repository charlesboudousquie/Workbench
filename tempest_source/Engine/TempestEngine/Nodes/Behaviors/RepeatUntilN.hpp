/*!***************************************************************************************
\file       RepeatUntilN.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node fully processes its child exactly N times aka N SUCCESSes.
*****************************************************************************************/
#include "Decorator.hpp"

#ifndef TESTING_NODES

#include "rapidjson.h"
#include "document.h"

class RepeatUntilN : public Decorator
{
private:
    //int counter;
    int maximumSuccesses;
public:

    // returns default render data
    static typeRT getRenderData();
    static void fillSpecialRenderData(const rapidjson::Value &JSON, typeRT&);

    // serialization
    static void serializeSpecialData(typeRT& data, rapidjson::Document& doc);

    void updateFromFile(const rapidjson::Value &) override;

    void Init()override;
    void Update(float) override;
};
#endif
