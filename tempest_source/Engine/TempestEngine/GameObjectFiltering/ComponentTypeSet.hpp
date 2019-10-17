/*!***************************************************************************************
\file       ComponentTypeSet.hpp
\author     Aaron Damyen
\date       5/10/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the componentTypeSet class.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include <ComponentType.hpp>

//========3rd Party Includes============================================================//
#include <memory>
#include <unordered_map>
#include <vector>

namespace GameObjectFiltering
{

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////
/*!***************************************************************************************
\par class: componentTypeSet
\brief   This is the componentTypeSet class used to filter game objects by the types of components
         they contain
*****************************************************************************************/
class componentTypeSet
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	/*!***************************************************************************************
	\brief  Enables the set entry for the specified component type.
	\param p_type - The type of the component to be added to the set.
	*****************************************************************************************/
	void setType(componentType p_type);

	void reset();

	/*!***************************************************************************************
	\brief  Determines the equality between this set and another.
	\param p_other - The other set to determine equality with.
	\return bool - Returns true if, for both sets, all entries in one are contained in the other.
	*****************************************************************************************/
	bool operator==(const componentTypeSet & p_other) const;
	/*!***************************************************************************************
	\brief  Determines if this set is a proper subset of the other set.
	\param p_other - The other set to compare.
	\return bool - Returns true if all entries in this set are contained in the other set and
	               the two sets are not equal.
	*****************************************************************************************/
	bool operator<(const componentTypeSet & p_other) const;
	/*!***************************************************************************************
	\brief  Determines if this set is a subset of the other set.
	\param p_other - The other set to compare.
	\return bool - Returns true if all entries in this set are contained in the other set.
	*****************************************************************************************/
	bool operator<=(const componentTypeSet & p_other) const;

	/*!***************************************************************************************
	\brief  Determines if this set contains no entries.
	\return bool - Returns true if there are no entries in this set, false otherwise.
	*****************************************************************************************/
	bool isEmpty() const;

	/*!***************************************************************************************
	\brief  Calculates the hash value of this set.
	\return std::size_t - Returns the hash value of this set.
	\relates std::hash<GameObjectFiltering::componentTypeSet>
	*****************************************************************************************/
	std::size_t hashValue() const;

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	/*!***************************************************************************************
	\brief  Retrieves a reference to an empty set.
	\return componentTypeSet - The empty set.
	*****************************************************************************************/
	static componentTypeSet Empty();

	static const componentType typeForIndex(unsigned int p_combined_index);

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	static constexpr unsigned int s_blockCount = 1; //!< Number of size_t to use for bitset
	size_t bits[s_blockCount] = { 0 }; //!< The bits for indicating members of a component type
	
	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////
	
	/*!***************************************************************************************
	\brief  Finds the index of a component type within the dynamically manages list of types.
	\param p_type - The type of the component to find in the list.
	\return unsigned int - The location (combined index) of the specified type.
	*****************************************************************************************/
	static unsigned int indexOf(componentType p_type);
	static void indicesOf(componentType p_type, std::vector<unsigned int> & p_result);
	/*!***************************************************************************************
	\brief  Calculates the element index and bit index given a combined index
	\param p_combined_index - the combined index to break apart
	\param p_element_index - out - the calculated element index containing the combined index
	\param p_bit_index - out - the calculated bit index containing the combined index
	*****************************************************************************************/
	static void breakdownIndex(unsigned int p_combined_index, unsigned int & p_element_index, unsigned int & p_bit_index);

	static unsigned int nextCombinedIndex; //!< The next available component type (combined) index
	static std::unordered_map<componentType, unsigned int /*combined index*/> indexMap; //!< map of component types to their assigned index

};

} // namespace GameObjectFiltering


namespace std 
{
////////==========================================================================////////
////////  Structure                                                               ////////
////////==========================================================================////////
/*!*************************************************************************************
\par struct: hash<componentTypeSet>
\brief   Implementation of the has<componentTypeSet> template specialization so the componentTypeSet
         class can be used as an STL container key value type.
***************************************************************************************/
template <>
struct hash<GameObjectFiltering::componentTypeSet>
{
	/*!***************************************************************************************
	\brief  Executes the hash value calculation.
	\param p_key - The set to calculate the hash value for.
	\return std::size_t - The calculated hash value.
	*****************************************************************************************/
	std::size_t operator()(const GameObjectFiltering::componentTypeSet & p_key) const
	{
		return p_key.hashValue();
	}
};

} // namespace std

