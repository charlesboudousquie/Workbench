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

class RepeatUntilN : public Decorator
{
private:
    int counter;
    int maximumSuccesses;
public:

    static typeRT onRender();

    void Init()override;
    void Update(float) override;
};
#endif
