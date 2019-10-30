#include "BehaviorVisitor.hpp"

#include <functional>

#include <map>

#include "Nodes/Behaviors/DefaultLeaf.hpp"
#include "Nodes/Behaviors/JumpUp.hpp"

// decorators
#include "Nodes/Behaviors/Inverter.hpp"
#include "Nodes/Behaviors/Repeater.hpp"
#include "Nodes/Behaviors/RepeatUntilN.hpp"
#include "Nodes/Behaviors/RepeatUntilFailure.hpp"
#include "Nodes/Behaviors/RepeatUntilSuccess.hpp"
#include "Nodes/Behaviors/ReturnTrue.hpp"
#include "Nodes/Behaviors/Timer.hpp"

//composites
#include "Nodes/Behaviors/Selector.hpp"
#include "Nodes/Behaviors/Sequencer.hpp"


// NOTE: Nodes with special rendering data must implement
// fillSpecialRenderData and serializeSpecialData functions
// and put then into maps below



// map from behavior name to behavior renderer function that returns all variables to 
// render to screen
static std::map<std::string, std::function<typeRT(void)>> renderGetters
{
    // leaves
{"DefaultLeaf", DefaultLeaf::onRender},
{"JumpUp", JumpUp::onRender},

// composites
{"Selector", Selector::onRender},
{"Sequencer", Sequencer::onRender},

// decorators
{"Inverter",Inverter::onRender},
{"Repeater", Repeater::onRender},
{"RepeatUntilN", RepeatUntilN::getRenderData},
{"RepeatUntilFailure",RepeatUntilFailure::onRender},
{"RepeatUntilSuccess", RepeatUntilSuccess::onRender},
{"ReturnTrue", ReturnTrue::onRender},
{"Timer", Timer::getRenderData},
};

// map of behavior name to its corresponding function that deals with reading special values
// from a json object and serializes them into the typeRT
static std::map<std::string, std::function<void(const rapidjson::Value &, typeRT&)>> specialRenderers
{
    {"Timer", Timer::fillSpecialRenderData},
    {"RepeatUntilN", RepeatUntilN::fillSpecialRenderData}
};

// serialization
static std::map < std::string, std::function<void(typeRT&, rapidjson::Document &)>> serializers
{
    {"Timer", Timer::serializeSpecialData},
    {"RepeatUntilN", RepeatUntilN::serializeSpecialData}
};


bool BehaviorVisitor::BehaviorIsSpecialized(const std::string & name)
{
    return specialRenderers.find(name) != specialRenderers.end();
}

typeRT BehaviorVisitor::GetRenderData(const std::string & name)
{
    if (renderGetters.find(name) != renderGetters.end())
    {
        return renderGetters[name]();
    }

    throw std::exception("No getRenderData function");
}

typeRT BehaviorVisitor::GetRenderData(const rapidjson::Value & v, const std::string & name)
{
    // get generic data of node
    auto data = GetRenderData(name);

    // if behavior has specialized data
    if (BehaviorIsSpecialized(name))
    {
        // fill in specialized data
        specialRenderers[name](v, data.member("Node Render Data"));
    }
    
    return data;
}

void BehaviorVisitor::SaveToFile(const std::string & name, typeRT & data, rapidjson::Document & doc)
{
    serializers[name](data.member("Node Render Data"), doc);
}

