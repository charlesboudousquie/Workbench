/*!***************************************************************************************
\file       ComponentTypeSet.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the ComponentTypeSet class member functions.
*****************************************************************************************/

//======== Self Include ================================================================//
#include "ComponentTypeSet.hpp"
//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <cassert>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations ========================================================//


namespace GameObjectFiltering
{
	//======== Static Data Definitions ===================================================//
  unsigned int componentTypeSet::nextCombinedIndex = 0;
  std::unordered_map<componentType, unsigned int /*index*/> componentTypeSet::indexMap;

	//======== Constructors & Destructor ===================================================//

	//======== Getters & Setters ===========================================================//
  std::size_t componentTypeSet::hashValue() const
  {
	  size_t seed = bits[0];
	  for (unsigned int i = 1; i < s_blockCount; ++i)
	  {
		  seed ^= bits[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2); // https://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html
	  }

	  return seed;
  }

  componentTypeSet componentTypeSet::Empty()
  {
	  return componentTypeSet();
  }

	const componentType componentTypeSet::typeForIndex(unsigned p_combined_index)
	{
		for(auto const & l_iter : indexMap)
		{
			if (l_iter.second == p_combined_index)
				return l_iter.first;
		}
		return componentType::forAbstract<void>();
	}

	bool componentTypeSet::isEmpty() const
  {
	  for (unsigned int i = 0; i < s_blockCount; ++i)
		  if (bits[i] != 0)
			  return false;

	  return true;
  }

  void componentTypeSet::setType(componentType p_type)
  {
	  unsigned int l_element_index = 0;
	  unsigned int l_bit_index = 0;
	  std::vector<unsigned int> l_indices;
	  indicesOf(p_type, l_indices);
	  for (unsigned int l_combined_index : l_indices)
	  {
		  breakdownIndex(l_combined_index, l_element_index, l_bit_index);
		  bits[l_element_index] |= static_cast<size_t>(1) << l_bit_index;
	  }
  }

	void componentTypeSet::reset()
	{
		*this = componentTypeSet();
	}

	//======== Overrides ===================================================================//
  bool componentTypeSet::operator==(const componentTypeSet & p_other) const
  {
	  for (unsigned int i = 0; i < s_blockCount; ++i)
		  if (bits[i] != p_other.bits[i])
			  return false;

	  return true;
  }

  bool componentTypeSet::operator<(const componentTypeSet& p_other) const
  {
	  //return (bits & p_other.bits) == bits && bits != p_other.bits;
	  for (unsigned int i = 0; i < s_blockCount; ++i)
		  if ((bits[i] & ~p_other.bits[i]) != 0)
			  return false;
	  return !(*this == p_other);
  }

  bool componentTypeSet::operator<=(const componentTypeSet& p_other) const
  {
	  return *this < p_other || *this == p_other;
  }

	//======== Functionality ===============================================================//

	//======== Helper Functions ============================================================//
	unsigned int componentTypeSet::indexOf(componentType p_type)
	{
		// find if the type is already in the map
		auto l_iter = indexMap.find(p_type);
		if (l_iter == indexMap.end())
		{
			unsigned int l_result = nextCombinedIndex++;

			// add to the map
			indexMap.emplace(p_type, l_result);

			return l_result;
		}

		return l_iter->second; // return the existing index
	}

	void componentTypeSet::indicesOf(componentType p_type, std::vector<unsigned>& p_result)
	{
		p_result.push_back(indexOf(p_type));
		if(p_type.hasParent())
		{
			indicesOf(p_type.getParentType(), p_result);
		}
	}

	void componentTypeSet::breakdownIndex(unsigned int p_combined_index, unsigned int & p_element_index, unsigned int & p_bit_index)
	{
		assert(p_combined_index >= 0);
		constexpr unsigned int s_bitCountPerElement = sizeof(size_t) * 8;
		p_element_index = p_combined_index / s_bitCountPerElement;
		p_bit_index = p_combined_index % s_bitCountPerElement;
	}

} // namespace GameObjectFiltering


