/*!***************************************************************************************
\file       MathFunctions.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

struct quaternion;
struct vector3;

float RadToDeg(float radians);
float DegToRad(float degrees);
vector3 toEulerAngle(const quaternion & q);
quaternion toQuaternion(vector3 p_eular);


