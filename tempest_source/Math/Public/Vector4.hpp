/*!***************************************************************************************
\file       Vector4.hpp
\author     Cody Cannell
\date       5/22/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief      This is the definition of a simple Vector class of four values
*****************************************************************************************/
#pragma once

#include <string>

struct vector2;
struct vector3;

/*!*************************************************************************************
\par class: Vector3
\brief   Class for managing 4 float values
***************************************************************************************/
struct vector4
{

    ////////========================================================================////////
    ////////   Public                                                               ////////
    ////////========================================================================////////
public:

    //======================================================================================//
    //        Constructor                                                                   //
    //======================================================================================//

    /*!***************************************************************************************
    \brief  Default constructor, initializes values to 0
    *****************************************************************************************/
	vector4() noexcept;

    /*!***************************************************************************************
    \brief  Constructor built from xyzw
    \param p_x - The x element
    \param p_y - The y element
    \param p_z - The z element
    \param p_w - The w element
    *****************************************************************************************/
    vector4(float p_x, float p_y, float p_z, float p_w) noexcept;

    vector4(vector3 p_vec3, float p_w) noexcept;

	vector4(const float * p_other);
	
    /*!***************************************************************************************
    \brief  Copy Constructor
    \param  p_other - other vec4
    *****************************************************************************************/
    vector4(const vector4 & p_other) noexcept;

	vector4(vector4&&) noexcept = default;

    /*!***************************************************************************************
    \brief  defult destructor
    *****************************************************************************************/
    ~vector4() = default;

    //==============================================================================//
    //        Getters & Setters                                                     //
    //==============================================================================//

    /*!***************************************************************************************
    \brief  gets x component
    \return x component
    *****************************************************************************************/
    float getX() const { return x; }

     /*!***************************************************************************************
    \brief  gets y component
    \return y component
    *****************************************************************************************/
    float getY() const { return y; }

    /*!***************************************************************************************
    \brief  gets z component
    \return z component
    *****************************************************************************************/
    float getZ() const { return z; }

    /*!***************************************************************************************
    \brief  gets w component
    \return w component
    *****************************************************************************************/
    float getW() const { return w; }

    /*!***************************************************************************************
        \brief  sets x component
        \param p_new_x - The new x element to set
        *****************************************************************************************/
    void setX(float p_new_x) { x = p_new_x; }

    /*!***************************************************************************************
    \brief  sets y component
    \param p_new_y - The new y element to set
    *****************************************************************************************/
    void setY(float p_new_y) { y = p_new_y; }

    /*!***************************************************************************************
    \brief  sets z component
    \param p_new_z - The new z element to set
    *****************************************************************************************/
    void setZ(float p_new_z) { z = p_new_z; }

    /*!***************************************************************************************
    \brief  sets w component
    \param p_new_w - The new w element to set
    *****************************************************************************************/
    void setW(float p_new_w) { w = p_new_w; }

	/*!***************************************************************************************
	\brief  sets offset for x component
	\param p_offset - Of set to change x by
	*****************************************************************************************/
	void offsetX(float p_offset) { x += p_offset; }

	/*!***************************************************************************************
	\brief  sets offset for y component
	\param p_offset - Of set to change y by
	*****************************************************************************************/
	void offsetY(float p_offset) { y += p_offset; }

	/*!***************************************************************************************
	\brief  sets offset for z component
	\param p_offset - Of set to change z by
	*****************************************************************************************/
	void offsetZ(float p_offset) { z += p_offset; }

	/*!***************************************************************************************
	\brief  sets offset for z component
	\param p_offset - Of set to change z by
	*****************************************************************************************/
	void offsetW(float p_offset) { w += p_offset; }


    //======================================================================================//
    //       Operators                                                                      //
    //======================================================================================//


    /*!***************************************************************************************
    \brief  Gets unsigned element from vec4
    \param  p_index - The element you want
    \return ref to the element
    *****************************************************************************************/
    float& operator[](const unsigned & p_index);

    /*!***************************************************************************************
    \brief  Gets unsigned element from vec4
    \param  p_index - The element you want
    \return copy of the element
    *****************************************************************************************/
    float operator[](const unsigned & p_index) const;

    /*!***************************************************************************************
    \brief  Assignment Oerator
    \param  p_rhs - Right-hand vec4
    \return ref to this object after assignment
    *****************************************************************************************/
    vector4 & operator=(const vector4 & p_rhs);

	/*!***************************************************************************************
	\brief  Compares this vector with another for equality
	\param  p_other - The other vector to compare with
	\return bool - true if all properties are equivalent, false otherwise
	*****************************************************************************************/
	bool operator==(const vector4 & p_other) const;

	/*!***************************************************************************************
	\brief  Compares this vector with another for inequality
	\param  p_other - The other vector to compare with
	\return bool - false if all properties are equivalent, true otherwise
	*****************************************************************************************/
	bool operator!=(const vector4 & p_other) const;

		/*!***************************************************************************************
    \brief  Unary Operator
    \return Negated vec4
    *****************************************************************************************/
    vector4 operator-() const;
    
    /*!***************************************************************************************
    \brief  Adds right-hand vec4 to this vec4
    \param  p_rhs - Right-hand vec4
    *****************************************************************************************/
    void operator+=(const vector4 & p_rhs);

    /*!***************************************************************************************
    \brief  Subtracts right hand vec4 to this vec4
    \param  p_rhs - Right-hand vec4
    *****************************************************************************************/
    void operator-=(const vector4 & p_rhs);

    /*!***************************************************************************************
    \brief  Multiples right-hand vec4 to this vec4
    \param  p_rhs - Right hand vec4
    *****************************************************************************************/
    void operator*=(const vector4 & p_rhs);

    /*!***************************************************************************************
    \brief  Divides right-hand vec4 to this vec4
    \param  p_rhs - Right-hand vec4
    *****************************************************************************************/
    void operator/=(const vector4 & p_rhs);

    /*!***************************************************************************************
    \brief  Adds right-hand float to all elements of this vec4
    \param  p_rhs - Right-hand float
    *****************************************************************************************/
    void operator+=(const float & p_rhs);

    /*!***************************************************************************************
    \brief  Subtracts right-hand float to all elements of this vec4
    \param  p_rhs - Right-hand float
    *****************************************************************************************/
    void operator-=(const float & p_rhs);

    /*!***************************************************************************************
    \brief  Multiples right-hand float to all elements of this vec4
    \param  p_rhs - Right-hand float
    *****************************************************************************************/
    void operator*=(const float & p_rhs);

    /*!***************************************************************************************
    \brief  Divides right-hand float to all elements of this vec4
    \param  p_rhs - Right-hand float
    *****************************************************************************************/
    void operator/=(const float & p_rhs);

    /*!***************************************************************************************
    \brief  Makes temp copy of vec4 and += right-hand vec4 to this vec4
    \param  p_rhs - Right-hand vec4
    \return copy of temp vec4
    *****************************************************************************************/
    vector4 operator+(const vector4 & p_rhs) const;

        /*!***************************************************************************************
    \brief  Makes temp copy of vec4 and -= right-hand vec4 to this vec4
    \param  p_rhs - Right-hand vec4
    \return copy of temp vec4
    *****************************************************************************************/
    vector4 operator-(const vector4 & p_rhs) const;

     /*!***************************************************************************************
    \brief  Makes temp copy of vec4 and *= right-hand vec4 to this vec4
    \param  p_rhs - Right-hand vec4
    \return copy of temp vec4
    *****************************************************************************************/
    vector4 operator*(const vector4 & p_rhs) const;


    /*!***************************************************************************************
    \brief  Makes temp copy of vec4 and /= right-hand vec4 to this vec4
    \param  p_rhs - Right-hand vec4
    \return copy of temp vec4
    *****************************************************************************************/
    vector4 operator/(const vector4 & p_rhs) const;

    /*!***************************************************************************************
    \brief  Makes temp copy of vec4 and += right-hand float to this vec4
    \param  p_rhs - Right-hand vec4
    \return copy of temp vec4
    *****************************************************************************************/
    vector4 operator+(const float & p_rhs) const;

    /*!***************************************************************************************
    \brief  Makes temp copy of vec4 and -= right-hand float to this vec4
    \param  p_rhs - Right-hand float
    \return copy of temp vec4
    *****************************************************************************************/
    vector4 operator-(const float & p_rhs) const;

    /*!***************************************************************************************
    \brief  Makes temp copy of vec4 and *= right-hand float to this vec4
    \param  p_rhs - Right-hand float
    \return copy of temp vec4
    *****************************************************************************************/
    vector4 operator*(const float & p_rhs) const;

    /*!***************************************************************************************
    \brief  Makes temp copy of vec4 and /= right-hand float to this vec4
    \param  p_rhs - Right-hand float
    \return copy of temp vec4
    *****************************************************************************************/
    vector4 operator/(const float & p_rhs) const;

		/*!***************************************************************************************
			\brief  Gets the dot product of two vectors
			\param  p_rhs - Right-hand vec4
			\return dot product
			*****************************************************************************************/
		float dotP(const vector4 p_rhs) const;

		/*!***************************************************************************************
		\brief  Gets the Cross product of two vectors
		\param  p_rhs - Right-hand vec4
		\return dot product
		*****************************************************************************************/
		vector4 crossP(const vector4 p_rhs) const;

    /*!***************************************************************************************
    \brief  normalizes the vec4
    \return normalized vector
    *****************************************************************************************/
    void normalize();

		/*!***************************************************************************************
		\brief  normalizes the vec4
		\return normalized vector
		*****************************************************************************************/
		vector4 normalized();

    /*!***************************************************************************************
    \brief  Gets a pointer to the start of the data memory
    \return A float pointer to data
    *****************************************************************************************/
    float * toFloatPtr();
		const float* toFloatPtr() const;

    /*!***************************************************************************************
    \brief  Make string for output
    \return Output string
    *****************************************************************************************/
    std::string toStringPtr() const;

		vector3 toVector3();



    //////============================================================================//////
    //////     Data                                                                   //////
    //////============================================================================//////

    float x, y, z, w;

};

/*!***************************************************************************************
\brief  For google Test pretty printing
\param p_obj - vector to be printed
\param p_os - stream to print to
*****************************************************************************************/
void PrintTo(const vector4& p_obj, std::ostream* p_os);

static float dotP(const vector4 & p_left, const vector4 & p_right);
