/*!***************************************************************************************
\file       Vector2.hpp
\author     Cody Cannell
\date       5/17/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This is the definition of the Vector3 class.
*****************************************************************************************/
#pragma once

#include <string>

/////////========================================================================/////////
/////////  Class                                                                 /////////
/////////========================================================================/////////
#pragma pack(push, 1)
/*!*************************************************************************************
\par class: Vector2
\brief   Class for managing 2 float values
***************************************************************************************/
struct vector2
{



	//////======================================================================//////
	//////    Static                                                            //////
	//////======================================================================//////
	static const vector2 UnitX;  //!< unit in x direction
	static const vector2 UnitY;  //!< unit in y direction

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
	\brief  sets offset for x component
	\param p_offset - Of set to change x by
	*****************************************************************************************/
	void offsetX(float p_offset) { x += p_offset; }

	/*!***************************************************************************************
	\brief  sets offset for y component
	\param p_offset - Of set to change y by
	*****************************************************************************************/
	void offsetY(float p_offset) { y += p_offset; }


	//======================================================================================//
	//        Constructor                                                                   //
	//======================================================================================//

	/*!***************************************************************************************
	\brief  Constructor
	*****************************************************************************************/
	vector2() = default;

	/*!***************************************************************************************
	\brief  Constructor built from xyz
	\param p_x - The x element
	\param p_y - The y element
	*****************************************************************************************/
	vector2(float p_x, float p_y);

	/*!***************************************************************************************
	\brief  Copy Constructor
	\param  p_other - other vec2
	*****************************************************************************************/
	//explicit vector2(const glm::vec2 & p_other);

	/*!***************************************************************************************
	\brief  defult destructor
	*****************************************************************************************/
	~vector2() = default;


	//======================================================================================//
	//       Operators                                                                      //
	//======================================================================================//

	/*!***************************************************************************************
	\brief  Gets unsigned element from vec2
	\param  p_index - The element you want
	\return ref to the element
	*****************************************************************************************/
	float& operator[](unsigned p_index);

	/*!***************************************************************************************
	\brief  Gets unsigned element from vec2
	\param  p_index - The element you want
	\return copy of the element
	*****************************************************************************************/
	float operator[](const unsigned & p_index) const;//TODO (cody): change to return ref

	/*!***************************************************************************************
	\brief  Unary Operator
	\return Negated vec2
	*****************************************************************************************/
	vector2 operator-() const;

	/*!***************************************************************************************
	\brief  Adds right-hand vec2 to this vec2
	\param  p_rhs - Right-hand vec2
	*****************************************************************************************/
	void operator+=(const vector2 & p_rhs);

	/*!***************************************************************************************
	\brief  Subtracts right hand vec2 to this vec2
	\param  p_rhs - Right-hand vec2
	*****************************************************************************************/
	void operator-=(const vector2 & p_rhs);

	/*!***************************************************************************************
	\brief  Multiples right-hand vec2 to this vec2
	\param  p_rhs - Right hand vec2
	*****************************************************************************************/
	void operator*=(const vector2 & p_rhs);

	/*!***************************************************************************************
	\brief  Divides right-hand vec2 to this vec2
	\param  p_rhs - Right-hand vec2
	*****************************************************************************************/
	void operator/=(const vector2 & p_rhs);

	/*!***************************************************************************************
	\brief  Adds right-hand float to all elements of this vec2
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	void operator+=(const float & p_rhs);

	/*!***************************************************************************************
	\brief  Subtracts right-hand float to all elements of this vec2
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	void operator-=(const float & p_rhs);

	/*!***************************************************************************************
	\brief  Multiples right-hand float to all elements of this vec2
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	void operator*=(const float & p_rhs);

	/*!***************************************************************************************
	\brief  Divides right-hand float to all elements of this vec2
	\param  p_rhs - Right-hand float
	*****************************************************************************************/
	void operator/=(const float & p_rhs);

	/*!***************************************************************************************
	\brief  Makes temp copy of vec2 and += right-hand vec2 to this vec2
	\param  p_rhs - Right-hand vec2
	\return copy of temp vec2
	*****************************************************************************************/
	vector2 operator+(const vector2 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec2 and -= right-hand vec2 to this vec2
	\param  p_rhs - Right-hand vec2
	\return copy of temp vec2
	*****************************************************************************************/
	vector2 operator-(const vector2 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec2 and *= right-hand vec2 to this vec2
	\param  p_rhs - Right-hand vec2
	\return copy of temp vec2
	*****************************************************************************************/
	vector2 operator*(const vector2 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec2 and /= right-hand vec2 to this vec2
	\param  p_rhs - Right-hand vec2
	\return copy of temp vec2
	*****************************************************************************************/
	vector2 operator/(const vector2 & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec2 and += right-hand float to this vec2
	\param  p_rhs - Right-hand vec2
	\return copy of temp vec2
	*****************************************************************************************/
	vector2 operator+(const float & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec2 and -= right-hand float to this vec2
	\param  p_rhs - Right-hand float
	\return copy of temp vec2
	*****************************************************************************************/
	vector2 operator-(const float & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec2 and *= right-hand float to this vec2
	\param  p_rhs - Right-hand float
	\return copy of temp vec2
	*****************************************************************************************/
	vector2 operator*(const float & p_rhs) const;

	/*!***************************************************************************************
	\brief  Makes temp copy of vec2 and /= right-hand float to this vec2
	\param  p_rhs - Right-hand float
	\return copy of temp vec2
	*****************************************************************************************/
	vector2 operator/(const float & p_rhs) const;


	/*!***************************************************************************************
	\brief  Gets the distence of the vec2 using distance formula(not implemented)
	\return Distance
	*****************************************************************************************/
	float distance() const;

	/*!***************************************************************************************
	\brief  Gets the distance squared of the vec3 using distance formula (faster than distance())
	\return Distance squared
	*****************************************************************************************/
	float distanceSquared() const;
	
	/*!***************************************************************************************
	\brief  Gets the dot product of two vectors
	\param  p_rhs - Right-hand vec2
	\return dot product
	*****************************************************************************************/
	float dotP(const vector2 & p_rhs) const;

	/*!***************************************************************************************
	\brief  normalizes the vec2
	\return normalized vector
	*****************************************************************************************/
	vector2 normalize();

	/*!***************************************************************************************
	\brief  Gets a pointer to the start of the data memory
	\return A float pointer to data
	*****************************************************************************************/
	float const * toFloatPtr() const;

	float * toFloatPtr();

	/*!***************************************************************************************
	\brief  Make string for output
	\return Output string
	*****************************************************************************************/
	std::string toStringPtr();



	//////============================================================================//////
	//////     Data                                                                   //////
	//////============================================================================//////
	float x, y;



	//glm::vec2 m_data;  //!< GLM vec2

};

#pragma pack(pop)
