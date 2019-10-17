/*!***************************************************************************************
\file       Vector3.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is the definition of the Vector3 class. 
*****************************************************************************************/

//========Self Include==================================================================//
#include "Vector3.hpp"

//========1st Party Includes============================================================//
#include "Vector4.hpp"
#include "Quaternion.hpp"
#include <sstream>

#ifndef _WIN32
#include <cmath>
#endif

//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

    ///////========================================================================///////
    ///////   Public                                                               ///////
    ///////========================================================================///////

        //==============================================================================//
        //        Constructor                                                           //
        //==============================================================================//
        vector3::vector3() noexcept : x(0),y(0),z(0){}

        vector3::vector3(float p_x, float p_y, float p_z) : x(p_x), y(p_y), z(p_z)
        { }

        vector3::vector3(const vector3 & p_other) : x(p_other.x), y(p_other.y), z(p_other.z)
        { }

		vector3::vector3(const vector4 & p_other) : x(p_other.x), y(p_other.y), z(p_other.z)
		{ }

        //==============================================================================//
        //        Destructor                                                            //
        //==============================================================================//

        //==============================================================================//
        //        Operators                                                             //
        //==============================================================================//

        vector3& vector3::operator=(const vector3 & rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;

            return *this;
        }

        float& vector3::operator[](const unsigned & p_index)
        {
            return toFloatPtr()[p_index];
        }

        float vector3::operator[](const unsigned & p_index) const
        {
            return toFloatPtr()[p_index];
        }

				bool vector3::operator==(const vector3& p_other) const
				{
					return x == p_other.x && y == p_other.y && z == p_other.z;
				}

				bool vector3::operator!=(const vector3& p_other) const
				{
					return x != p_other.x || y != p_other.y || z != p_other.z;
				}

				vector3 vector3::operator-() const
        {
            return vector3(-x, -y, -z);
        }


        void vector3::operator+=(const vector3 & p_rhs)
        {
            x += p_rhs.x;
            y += p_rhs.y;
            z += p_rhs.z;
        }

        void vector3::operator-=(const vector3 & p_rhs)
        {
            x -= p_rhs.x;
            y -= p_rhs.y;
            z -= p_rhs.z;
        }

        void vector3::operator*=(const vector3 & p_rhs)
        {
            x *= p_rhs.x;
            y *= p_rhs.y;
            z *= p_rhs.z;
        }

        void vector3::operator/=(const vector3 & p_rhs)
        {
            x /= p_rhs.x;
            y /= p_rhs.y;
            z /= p_rhs.z;
        }

        void vector3::operator+=(const float & p_rhs)
        {
            x += p_rhs;
            y += p_rhs;
            z += p_rhs;
        }

        void vector3::operator-=(const float & p_rhs)
        {
            x -= p_rhs;
            y -= p_rhs;
            z -= p_rhs;
        }

        void vector3::operator*=(const float & p_rhs)
        {
            x *= p_rhs;
            y *= p_rhs;
            z *= p_rhs;
        }

        void vector3::operator/=(const float & p_rhs)
        {
            x /= p_rhs;
            y /= p_rhs;
            z /= p_rhs;
        }

        vector3 vector3::operator+(const vector3 & p_rhs) const
        {
            vector3 l_temp = *this;
            l_temp += p_rhs;
            return l_temp;
        }

        vector3 vector3::operator-(const vector3 & p_rhs) const
        {
            vector3 l_temp = *this;
            l_temp -= p_rhs;
            return l_temp;
        }

        vector3 vector3::operator*(const  vector3 & p_rhs) const
        {
            vector3 l_temp = *this;
            l_temp *= p_rhs;
            return l_temp;
        }

        vector3 vector3::operator/(const vector3 & p_rhs) const
        {
            vector3 l_temp = *this;
            l_temp /= p_rhs;
            return l_temp;
        }

        vector3 vector3::operator+(const float & p_rhs) const
        {
            vector3 l_temp = *this;
            l_temp += p_rhs;
            return l_temp;
        }

        vector3 vector3::operator-(const float & p_rhs) const
        {
            vector3 l_temp = *this;
            l_temp -= p_rhs;
            return l_temp;
        }

        vector3 vector3::operator*(const float & p_rhs) const
        {
            vector3 l_temp = *this;
            l_temp *= p_rhs;
            return l_temp;
        }

        vector3 vector3::operator/(const float & p_rhs) const
        {
            vector3 l_temp = *this;
            l_temp /= p_rhs;
            return l_temp;
        }

        bool vector3::operator==(const vector3& p_rhs)
        {
            return x == p_rhs.x && y == p_rhs.y && z == p_rhs.z;
        }

        //==============================================================================//
        //        Getters & Setters                                                     //
        //==============================================================================//

        /////========================================================================/////
        /////     Functions                                                          /////
        /////========================================================================/////
        float vector3::distance() const
        {
            return sqrt(distanceSquared());
        }

				float vector3::distanceSquared() const
				{
					return x*x + y*y + z*z;
				}

        float vector3::dotP(const vector3 p_rhs) const
        {
            return x * p_rhs.x + y * p_rhs.y + z * p_rhs.z;
        }

				vector3 vector3::crossP(const vector3 p_rhs) const
				{
					 return vector3(y * p_rhs.z - z * p_rhs.y, z * p_rhs.x - x * p_rhs.z, x * p_rhs.y - y * p_rhs.x);
				}

        vector3 vector3::normalized() const
        {

						float length = std::sqrt(x * x + y * y + z * z);

            return *this / length;
        }

	
				void vector3::normalize()
				{

					float length = std::sqrt(x * x + y * y + z * z);

					*this /= length;
				}


        const float* vector3::toFloatPtr() const 
        {
           return reinterpret_cast<const float*>(this);
        }

        float* vector3::toFloatPtr()
        {
            return reinterpret_cast<float*>(this);
        }

        std::string vector3::toStringPtr() const
        {
            std::stringstream l_ss;
            l_ss << "(" << x << ", " << y << ", " << z << ")";
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
        const vector3 vector3::UnitX = vector3(1.0f, 0.0f, 0.0f);
        const vector3 vector3::UnitY = vector3(0.0f, 1.0f, 0.0f);
        const vector3 vector3::UnitZ = vector3(0.0f, 0.0f, 1.0f);
				const vector3 vector3::Zero = vector3(0.0f, 0.0f, 0.0f);
        ////==========================================================================////
        ////      Non-Virtual                                                         ////
        ////==========================================================================////

       //==============================================================================//
        //        Helper                                                                //
        //==============================================================================//

				void PrintTo(const vector3& p_obj, std::ostream* p_os) {
					*p_os << p_obj.toStringPtr();  // whatever needed to print bar to os
				}

				vector3 operator*(float lhs, const vector3 & rhs)
				{
					return rhs * lhs;
				}


		    vector3 vector3::rotate(quaternion rotation)
		    {

		        quaternion conjugate = rotation.Conjugated();

		        quaternion w = rotation * *this * conjugate;

		        return vector3(w.x, w.y, w.z);

		    }

				void vector3::splat(float p_value)
				{
					x = p_value;
					y = p_value;
					z = p_value;
				}
