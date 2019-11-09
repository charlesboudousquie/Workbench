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

const char maxSuccessStr[] = "maximumSuccesses";
const char runCounterStr[] = "RUN_counter";

const SpecialData RepeatUntilN::specialData =
{
    {{DATA_TYPE::INT, "maximumSuccesses", 0},
    {DATA_TYPE::UNSIGNED_INT, "RUN_counter", 0}}
};

//typeRT RepeatUntilN::getRenderData()
//{
//    // get default data
//    auto defaultRenderData = Decorator::decoratorOnRender();
//
//    // add specialized data
//    typeRT l_max_successes(maxSuccessStr, int(1));
//    defaultRenderData.member("Node Render Data").insertMember(l_max_successes);
//
//    // return combined typeRT data
//    return defaultRenderData;
//}
//
//void RepeatUntilN::fillSpecialRenderData(const rapidjson::Value & JSON, typeRT &data)
//{
//    assert(data.getVariableName() == "Node Render Data");
//
//    // if json does not have max success count then something is wrong
//    if (JSON.HasMember(maxSuccessStr))
//    {
//        data.member(maxSuccessStr).setInt(JSON[maxSuccessStr].GetInt());
//    }
//    else
//    {
//        throw std::runtime_error("Missing maximumSuccesses in repeatuntiln.cpp");
//    }
//
//}
//
//void RepeatUntilN::serializeSpecialData(typeRT & data, rapidjson::Document & doc)
//{
//    assert(data.getVariableName() == "Node Render Data");
//
//    int max_Successes = data.member(maxSuccessStr).getInt();
//
//    if (doc.HasMember(maxSuccessStr))
//    {
//        doc[maxSuccessStr].SetInt(max_Successes);
//    }
//    else
//    {
//        doc.AddMember(maxSuccessStr, max_Successes, doc.GetAllocator());
//    }
//}

void RepeatUntilN::updateFromFile(const rapidjson::Value & value)
{
    maximumSuccesses = value[maxSuccessStr].GetInt();
}

void RepeatUntilN::Init()
{
    // progressing and running, child is ready
    Decorator::Init();

    GetBlackboard().setValue<int>(runCounterStr, 0);
}

void RepeatUntilN::Update(float dt)
{
    auto task = GetTask();
    int counter = GetBlackboard().getValue<int>(runCounterStr);


    // activate child behavior
    BehaviorResult childResult = task->GetResult();

    // if child succeeded then we are closer to success
    if (childResult == BehaviorResult::SUCCESS)
    {
        counter++;
        GetBlackboard().setValue<int>(runCounterStr, counter);
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

