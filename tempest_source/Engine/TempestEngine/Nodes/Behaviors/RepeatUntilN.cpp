/*!***************************************************************************************
\file       RepeatUntilN.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node fully processes its child exactly N times aka N SUCCESSes.
*****************************************************************************************/
#include "RepeatUntilN.hpp"
#ifndef TESTING_NODES

typeRT RepeatUntilN::getRenderData()
{
    // get default data
    auto defaultRenderData = Decorator::decoratorOnRender();

    // add specialized data
    typeRT l_max_time("maximumSuccesses", int(1));
    defaultRenderData.member("Node Render Data").insertMember(l_max_time);

    // return combined typeRT data
    return defaultRenderData;
}

void RepeatUntilN::fillSpecialRenderData(const rapidjson::Value & JSON, typeRT &data)
{
    assert(data.getVariableName() == "Node Render Data");

    // if json does not have max success count then something is wrong
    if (JSON.HasMember("maximumSuccesses"))
    {
        data.member("maximumSuccesses").setInt(JSON["maximumSuccesses"].GetInt());
    }
    else
    {
        throw std::exception("Missing maximumSuccesses in repeatuntiln.cpp");
    }

}

void RepeatUntilN::serializeSpecialData(typeRT & data, rapidjson::Document & doc)
{
    assert(data.getVariableName() == "Node Render Data");

    int max_Successes = data.member("maximumSuccesses").getInt();

    if (doc.HasMember("maximumSuccesses"))
    {
        doc["maximumSuccesses"].SetInt(max_Successes);
    }
    else
    {
        doc.AddMember("maximumSuccesses", max_Successes, doc.GetAllocator());
    }
}

void RepeatUntilN::updateFromFile(const rapidjson::Value & value)
{
    maximumSuccesses = value["maximumSuccesses"].GetInt();
}

void RepeatUntilN::Init()
{
    // progressing and running, child is ready
    Decorator::Init();

    GetBlackboard().setValue<int>("RUN_counter", 0);
}

void RepeatUntilN::Update(float dt)
{
    auto task = GetTask();
    int counter = GetBlackboard().getValue<int>("RUN_counter");


    // activate child behavior
    BehaviorResult childResult = task->GetResult();

    // if child succeeded then we are closer to success
    if (childResult == BehaviorResult::SUCCESS)
    {
        counter++;
        GetBlackboard().setValue<int>("RUN_counter", counter);
        if (counter == this->maximumSuccesses)
        {
            task->SetResult(BehaviorResult::SUCCESS);
            task->SetPhase(BehaviorPhase::DONE);
        }
        else
        {
            GiveToChild(task); // give task back to child again
        }
    }
    // if child failed then it means we could not complete a behavior N times
    else if (childResult == BehaviorResult::FAILURE)
    {
        task->SetResult(BehaviorResult::FAILURE);
        task->SetPhase(BehaviorPhase::DONE);
    }
}
#endif

