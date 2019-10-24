/*!***************************************************************************************
\file       Agent.cpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This component is used for our AI system which includes behavior trees.
*****************************************************************************************/
#include "Agent.hpp"
#include "GameObject.hpp"
#include "BehaviorTask.hpp"
#include "BehaviorTree.hpp"
#include "Nodes/Behaviors/Behavior.hpp"

// given a task and a tree, introduce the task to the tree
void IntroduceTaskToTree(GameObjectPtr parent, BehaviorTreePtr tree, BehaviorTaskPtr task)
{
    // clear task of any state
    task->ClearHistory();

    // push new state onto task
    task->RegisterNewNode(tree->GetRoot()->getId());

    task->SetActor(parent);

    // set new tree
    task->SetTreePtr(tree);

    // set task pointer for all behaviors to acess
    tree->SetTask(task);

    // allow root of tree to take task first
    tree->GetRoot()->TakeTask(task);
}

bool TaskDone(BehaviorTaskPtr task, BehaviorTreePtr tree)
{
    return (task->GetPhase() == BehaviorPhase::DONE) && 
        (tree->GetRoot().get() == task->GetCurrentBehavior());
}

Agent::Agent()
{
    task_ = std::make_shared<BehaviorTask>();
}

BehaviorTaskPtr Agent::GetTask()
{
    return task_;
}

void Agent::Update(float dt, BehaviorTreePtr tree)
{
    GameObjectPtr parentObject = this->getGameObject().lock();

    // if first time using tree
    if (task_->GetTree() != tree || !task_->WorkingWithTree())
    {
        IntroduceTaskToTree(parentObject, tree, task_);
    }
    else
    {
        // otherwise merely tick tree
        task_->GetCurrentBehavior()->tick(dt);
    }
}

componentType const Agent::type() const
{
    return getType();
}

typeRT Agent::toTypeRT() const
{
    // set type and variable name
    typeRT result;
    result.setTypeName(getType().name());
    result.setVariableName(getType().name());

    // agent should know what tree it belongs to
    typeRT parentTree("parentTree", this->treeName);
    
    // this should set the parent tree, since an agent should only have 1 tree
    result.insertMember(parentTree);

    return result;
}

void Agent::updateFromTypeRT(typeRT & p_type)
{
    if (p_type.members().find("parentTree") != p_type.members().end())
    {
        auto name = p_type.member("parentTree").getString();
        if (name.size() > 0)
        {
            this->treeName = p_type.member("parentTree").getString();
        }
    }
    else
    {
        std::cout << "ERROR updatefrom type rt in Agent component was missing parentTree name" << std::endl;
    }
}

std::string Agent::getQueryableType()
{
    return std::string(type().name());
}

std::string Agent::getQueryableID()
{
    if (!getGameObject().expired())
    {
        return std::to_string(getGameObject().lock()->getID());
    }
    else { return "orphan agent component"; }
}

std::string Agent::getQueryableProperty(std::string p_name)
{
    return toTypeRT().getString();

    ////convert this object to a typeRT
    //typeRT temp = toTypeRT();
    ////access the member with the name given
    //typeRT temp2 = temp.member(p_name);
    ////returns a typeRT equivalent to the type of the member variable
    ////return string consisting of each data using string conversion defined in typeRT class
    //std::string result;
    //result += temp2.member("x").to_csv_format();
    //result += temp2.member("y").to_csv_format();
    //result += temp2.member("z").to_csv_format();
    //return result;
}

std::vector<std::string> Agent::getQueryablePropertyNames()
{
    return { "parentTree" };
}

bool Agent::hasTreeName()
{
    return !treeName.empty();
}

std::string Agent::getTreeName()
{
    return treeName;
}


