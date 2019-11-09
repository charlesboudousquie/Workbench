/*!***************************************************************************************
\file       Timer.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node is a simple Decorator timer that activates its child node when
            timer hits 0.
*****************************************************************************************/
#include "Timer.hpp"
#ifndef TESTING_NODES

const char maxTimeStr[] = "Max Time";

const SpecialData Timer::specialData=
{
    {SpecialData::Entry{DATA_TYPE::FLOAT, "Max Time", -1.0f}}
};

//typeRT Timer::getRenderData()
//{
//    // get default data
//    auto defaultRenderData = Decorator::decoratorOnRender();
//
//    // add specialized data
//    typeRT l_max_time(maxTimeStr, float(-1));
//    defaultRenderData.member("Node Render Data").insertMember(l_max_time);
//
//    // return combined typeRT data
//    return defaultRenderData;
//}
//
//void Timer::fillSpecialRenderData(const rapidjson::Value & JSON, typeRT &data)
//{
//    assert(data.getVariableName() == "Node Render Data");
//
//    // if json does not have max time variable then we are screwed
//    if (JSON.HasMember(maxTimeStr))
//    {
//        // set typert to have max time value
//        data.member(maxTimeStr).setFloat(JSON[maxTimeStr].GetFloat());
//    }
//    else
//    {
//        throw std::runtime_error("Missing Max Time");
//    }
//
//}
//
//void Timer::serializeSpecialData(typeRT & data, rapidjson::Document & doc)
//{
//    assert(data.getVariableName() == "Node Render Data");
//
//    float max_time = data.member(maxTimeStr).getFloat();
//
//    if (doc.HasMember(maxTimeStr))
//    {
//        doc[maxTimeStr].SetFloat(max_time);
//    }
//    else
//    {
//        doc.AddMember(maxTimeStr, max_time, doc.GetAllocator()); 
//    }
//}

void Timer::updateFromFile(const rapidjson::Value & value)
{
    this->maxTime = value[maxTimeStr].GetFloat();
}

void Timer::Init()
{
#ifdef DEBUGGING_NODES
    printDebug(DEBUG_MESSAGE_TYPE::INIT);
#endif

    // do not call decorator's init, since we do not give task to child immediately

    // reset timer
    GetBlackboard().setValue<float>("currentTime", 0);

    // call update next frame
    GetTask()->SetPhase(BehaviorPhase::PROGRESSING);
}

void Timer::Update(float dt)
{
#ifdef DEBUGGING_NODES
    printDebug(DEBUG_MESSAGE_TYPE::UPDATE);
#endif

    float currentTime = GetBlackboard().getValue<float>("currentTime");
    currentTime += dt;
    GetBlackboard().setValue<float>("currentTime", currentTime);

    // if timer finished then activate child node
    if (currentTime >= this->maxTime)
    {
        // we will be done when the child hands back the task to us
        GetTask()->SetPhase(BehaviorPhase::DONE);

        // send task to child so child can take care of it
        GiveToChild(GetTask());
    }
}

#endif
