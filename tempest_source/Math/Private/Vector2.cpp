/*!***************************************************************************************
\file       Vector2.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is the definition of the Vector3 class.
*****************************************************************************************/

//========Self Include==================================================================//
#include "Vector2.hpp"
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
#include <glm/glm.hpp>
#include <sstream>
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

///////========================================================================///////
///////   Public                                                               ///////
///////========================================================================///////

//==============================================================================//
//        Constructor                                                           //
//==============================================================================//

vector2::vector2(float p_x, float p_y) : x(p_x), y(p_y)
{ }

//vector2::vector2(const glm::vec2 & p_other) : x(p_other.x), y(p_other.y)
//{ }

//==============================================================================//
//        Destructor                                                            //
//==============================================================================//

//==============================================================================//
//        Operators                                                             //
//==============================================================================//

float& vector2::operator[](unsigned p_index)
{
    return toFloatPtr()[p_index];
}

float vector2::operator[](const unsigned & p_index) const
{
    return toFloatPtr()[p_index];
}

vector2 vector2::operator-() const
{
    return vector2(-x, -y);
}


void vector2::operator+=(const vector2 & p_rhs)
{
    x += p_rhs.x;
    y += p_rhs.y;
}

void vector2::operator-=(const vector2 & p_rhs)
{
    x -= p_rhs.x;
    y -= p_rhs.y;
}

void vector2::operator*=(const vector2 & p_rhs)
{
    x *= p_rhs.x;
    y *= p_rhs.y;
}

void vector2::operator/=(const vector2 & p_rhs)
{
    x /= p_rhs.x;
    y /= p_rhs.y;
}

void vector2::operator+=(const float & p_rhs)
{
    x += p_rhs;
    y += p_rhs;
}

void vector2::operator-=(const float & p_rhs)
{
    x -= p_rhs;
    y -= p_rhs;
}

void vector2::operator*=(const float & p_rhs)
{
    x *= p_rhs;
    y *= p_rhs;
}

void vector2::operator/=(const float & p_rhs)
{
    x /= p_rhs;
    y /= p_rhs;
}

vector2 vector2::operator+(const vector2 & p_rhs) const
{
    vector2 l_temp = *this;
    l_temp += p_rhs;
    return l_temp;
}

vector2 vector2::operator-(const vector2 & p_rhs) const
{
    vector2 l_temp = *this;
    l_temp -= p_rhs;
    return l_temp;
}

vector2 vector2::operator*(const  vector2 & p_rhs) const
{
    vector2 l_temp = *this;
    l_temp *= p_rhs;
    return l_temp;
}

vector2 vector2::operator/(const vector2 & p_rhs) const
{
    vector2 l_temp = *this;
    l_temp /= p_rhs;
    return l_temp;
}

vector2 vector2::operator+(const float & p_rhs) const
{
    vector2 l_temp = *this;
    l_temp += p_rhs;
    return l_temp;
}

vector2 vector2::operator-(const float & p_rhs) const
{
    vector2 l_temp = *this;
    l_temp -= p_rhs;
    return l_temp;
}

vector2 vector2::operator*(const float & p_rhs) const
{
    vector2 l_temp = *this;
    l_temp *= p_rhs;
    return l_temp;
}

vector2 vector2::operator/(const float & p_rhs) const
{
    vector2 l_temp = *this;
    l_temp /= p_rhs;
    return l_temp;
}

//==============================================================================//
//        Getters & Setters                                                     //
//==============================================================================//

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////
float vector2::distance() const
{
    return sqrt(distanceSquared());
}

float vector2::distanceSquared() const
{
	return x * x + y * y;
}

float vector2::dotP(const vector2& p_rhs) const
{
    return x * p_rhs.x + y * p_rhs.y;
}

vector2 vector2::normalize()
{
	if(x == 0 && y == 0)
	{
		return *this;
	}
    glm::vec2 l_temp = glm::normalize(glm::vec2(x, y));

    x = l_temp.x;
    y = l_temp.y;

    return *this;
}

float const * vector2::toFloatPtr() const
{
    return reinterpret_cast<float const *>(this);
}

float * vector2::toFloatPtr()
{
    return reinterpret_cast<float*>(this);
}

std::string vector2::toStringPtr()
{
    std::stringstream l_ss;
    l_ss << "(" << x << ", " << y << ")";
    return l_ss.str();
}

///////========================================================================///////
///////   Private                                                              ///////
///////========================================================================///////

//////======================================================================//////
//////    Non-Static                                                        //////
//////======================================================================//////

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////
const vector2 vector2::UnitX = vector2(1.0f, 0.0f);
const vector2 vector2::UnitY = vector2(0.0f, 1.0f);
////==========================================================================////
////      Non-Virtual                                                         ////
////==========================================================================////

//==============================================================================//
//        Helper                                                                //
//==============================================================================//
