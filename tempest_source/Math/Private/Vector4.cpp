/*!***************************************************************************************
\file       Vector4.cpp
\author     Cody Cannell
\date       5/22/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief      This is the definition of a simple Vector class of four values
*****************************************************************************************/

//========Self Include==================================================================//
#include "Vector4.hpp"
//========1st Party Includes============================================================//
#include "Vector3.hpp"
//========3rd Party Includes============================================================//
#include <sstream>

#ifndef _WIN32
#include <cmath>
#endif

//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//



    ///////========================================================================///////
    ///////   Public                                                               ///////
    ///////========================================================================///////

				vector4::vector4() noexcept
					: x(0), y(0), z(0), w(0)
				{	}

        //==============================================================================//
        //        Constructor                                                           //
        //==============================================================================//
        vector4::vector4(float p_x, float p_y, float p_z, float p_w) noexcept : x(p_x), y(p_y), z(p_z), w(p_w)
        {}

        vector4::vector4(vector3 p_vec3, float p_w) noexcept : x(p_vec3.x), y(p_vec3.y), z(p_vec3.z), w(p_w)
        {}

		vector4::vector4(const vector4 & p_other) noexcept : x(p_other.x), y(p_other.y), z(p_other.z), w(p_other.w)
		{}

		vector4::vector4(const float * p_other)
		{
			x = p_other[0];
			y = p_other[1];
			z = p_other[2];
			w = p_other[3];
		}

        

        //==============================================================================//
        //        Destructor                                                            //
        //==============================================================================//

        //==============================================================================//
        //        Operators                                                             //
        //==============================================================================//

        vector4& vector4::operator=(const vector4 & rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;

            return *this;
        }

        vector4 vector4::operator-() const
        {
            return vector4(-x, -y, -z, -w);
        }


        void vector4::operator+=(const vector4 & p_rhs)
        {
            x += p_rhs.x;
            y += p_rhs.y;
            z += p_rhs.z;
						w += p_rhs.w;
        }

        void vector4::operator-=(const vector4 & p_rhs)
        {
            x -= p_rhs.x;
            y -= p_rhs.y;
            z -= p_rhs.z;
						w -= p_rhs.w;
        }

        void vector4::operator*=(const vector4 & p_rhs)
        {
            x *= p_rhs.x;
            y *= p_rhs.y;
            z *= p_rhs.z;
						w *= p_rhs.w;
        }

        void vector4::operator/=(const vector4 & p_rhs)
        {
            x /= p_rhs.x;
            y /= p_rhs.y;
            z /= p_rhs.z;
						w /= p_rhs.w;
        }

        void vector4::operator+=(const float & p_rhs)
        {
            x += p_rhs;
            y += p_rhs;
            z += p_rhs;
						w += p_rhs;
        }

        void vector4::operator-=(const float & p_rhs)
        {
            x -= p_rhs;
            y -= p_rhs;
            z -= p_rhs;
						w -= p_rhs;
        }

        void vector4::operator*=(const float & p_rhs)
        {
            x *= p_rhs;
            y *= p_rhs;
            z *= p_rhs;
						w *= p_rhs;
        }

        void vector4::operator/=(const float & p_rhs)
        {
            x /= p_rhs;
            y /= p_rhs;
            z /= p_rhs;
						w /= p_rhs;
        }

        vector4 vector4::operator+(const vector4 & p_rhs) const
        {
            vector4 l_temp = *this;
            l_temp += p_rhs;
            return l_temp;
        }

        vector4 vector4::operator-(const vector4 & p_rhs) const
        {
            vector4 l_temp = *this;
            l_temp -= p_rhs;
            return l_temp;
        }

        vector4 vector4::operator*(const vector4 & p_rhs) const
        {
            vector4 l_temp = *this;
            l_temp *= p_rhs;
            return l_temp;
        }

        vector4 vector4::operator/(const vector4 & p_rhs) const
        {
            vector4 temp = *this;
            temp /= p_rhs;
            return temp;
        }

        vector4 vector4::operator+(const float & p_rhs) const
        {
            vector4 l_temp = *this;
            l_temp += p_rhs;
            return l_temp;
        }

        vector4 vector4::operator-(const float & p_rhs) const
        {
            vector4 temp = *this;
            temp -= p_rhs;
            return temp;
        }

        vector4 vector4::operator*(const float & p_rhs) const
        {
            vector4 l_temp = *this;
            l_temp *= p_rhs;
            return l_temp;
        }

        vector4 vector4::operator/(const float & p_rhs) const
        {
            vector4 l_temp = *this;
            l_temp /= p_rhs;
            return l_temp;
        }

        float& vector4::operator[](const unsigned & p_index)
        {
            return toFloatPtr()[p_index];
        }

        float vector4::operator[](const unsigned & p_index) const
        {
					return toFloatPtr()[p_index];
        }

				bool vector4::operator==(const vector4& p_other) const
				{
					return x == p_other.x && y == p_other.y && z == p_other.z && w == p_other.w;
				}

				bool vector4::operator!=(const vector4& p_other) const
				{
					return x != p_other.x || y != p_other.y || z != p_other.z || w != p_other.w;
				}

				//==============================================================================//
        //        Getters & Setters                                                     //
        //==============================================================================//

        /////========================================================================/////
        /////     Functions                                                          /////
        /////========================================================================/////

				float vector4::dotP(const vector4 p_rhs) const
				{
					return x * p_rhs.x + y * p_rhs.y + z * p_rhs.z + w * p_rhs.w;
				}

				vector4 vector4::crossP(const vector4 p_rhs) const
				{
					return vector4(y * p_rhs.z - z * p_rhs.y, z * p_rhs.x - x * p_rhs.z, x * p_rhs.y - y * p_rhs.x, 1 );
				}

        void vector4::normalize()
        {          
					float length = std::sqrt(x * x + y * y + z * z + w * w);

					*this /= length;		
        }

				vector4 vector4::normalized()
				{
					float length = std::sqrt(x * x + y * y + z * z);

					return *this / length;

				}

				const float* vector4::toFloatPtr() const
				{
					return reinterpret_cast<const float*>(this);
				}

        float * vector4::toFloatPtr()
        {
            return reinterpret_cast<float*>(this);
        }

        std::string vector4::toStringPtr() const
        {
            std::stringstream l_ss;
            l_ss << "(" << x << ", " << y << ", " << z <<", " << w << ")";
            return l_ss.str();
        }

				vector3 vector4::toVector3() { return vector3(x, y, z); }

				/*float dotP(const vector4& p_left, const vector4& p_right)
				{
					return p_left.x * p_right.x
							 + p_left.y * p_right.y
							 + p_left.z * p_right.z
							 + p_left.w * p_right.w;
				}*/


				void PrintTo(const vector4& p_obj, std::ostream* p_os) 
				{
					*p_os << p_obj.toStringPtr();  // whatever needed to print bar to os
				}
