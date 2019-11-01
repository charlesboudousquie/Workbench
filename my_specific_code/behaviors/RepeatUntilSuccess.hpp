/*!***************************************************************************************
\file       RepeatUntilSuccess.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents a node that continually activates its child until 
            it succeeds.
*****************************************************************************************/
#include "Decorator.hpp"

#ifndef TESTING_NODES

class RepeatUntilSuccess : public Decorator
{
public:

    static typeRT onRender();

    void Update(float)override;
};

#endif // !TESTING_NODES
