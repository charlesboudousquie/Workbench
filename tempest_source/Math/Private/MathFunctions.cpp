/*!***************************************************************************************
\file       MathFunctions.cpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "MathFunctions.hpp"
#include "Quaternion.hpp"
#include "Vector3.hpp"
#include "MathConstants.hpp"

#ifndef _WIN32
#include <cmath>
#endif

float RadToDeg(float radians)
{
    return (180.0f / c_Pi) * radians;
}

float DegToRad(float degrees)
{
    return (c_Pi / 180.0f) * degrees;
}

vector3 toEulerAngle(const quaternion& p_q)
{

	//https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

    vector3 eular;  // yaw (Z), pitch (Y), roll (X)

    // roll (x-axis rotation)
    float sinr_cosp = +2.0f * (p_q.w * p_q.x + p_q.y * p_q.z);
    float cosr_cosp = +1.0f - 2.0f * (p_q.x * p_q.x + p_q.y * p_q.y);
    eular.x = atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    float sinp = +2.0f * (p_q.w * p_q.y - p_q.z * p_q.x);
    if (fabs(sinp) >= 1)
        eular.y = copysign(c_Pi / 2.0f, sinp); // use 90 degrees if out of range
    else
        eular.y = asin(sinp);

    // yaw (z-axis rotation)
    float siny_cosp = +2.0f * (p_q.w * p_q.z + p_q.x * p_q.y);
    float cosy_cosp = +1.0f - 2.0f * (p_q.y * p_q.y + p_q.z * p_q.z);
    eular.z = atan2(siny_cosp, cosy_cosp);

    eular.x = RadToDeg(eular.x);
    eular.y = RadToDeg(eular.y);
    eular.z = RadToDeg(eular.z);


    return eular;
}

quaternion toQuaternion(vector3 p_eular)  // yaw (Z), pitch (Y), roll (X)
{

	 

    //https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

    // Abbreviations for the various angular functions
    float cy = cos(DegToRad(p_eular.z) * 0.5f);
    float sy = sin(DegToRad(p_eular.z) * 0.5f);
    float cp = cos(DegToRad(p_eular.y) * 0.5f);
    float sp = sin(DegToRad(p_eular.y) * 0.5f);
    float cr = cos(DegToRad(p_eular.x) * 0.5f);
    float sr = sin(DegToRad(p_eular.x) * 0.5f);

    quaternion l_q;
    l_q.w = cy * cp * cr + sy * sp * sr;
    l_q.x = cy * cp * sr - sy * sp * cr;
    l_q.y = sy * cp * sr + cy * sp * cr;
    l_q.z = sy * cp * cr - cy * sp * sr;


    return l_q;
}


