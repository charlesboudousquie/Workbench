/*!***************************************************************************************
\file       ComponentTypeSetTests.cpp
\author     Aaron Damyen
\date       1/31/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a set of unit tests for the componentTypeSet data type
*****************************************************************************************/

//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <gtest/gtest.h>
#include <GameObjectFiltering/ComponentTypeSet.hpp>
//======== Types =======================================================================//
using GameObjectFiltering::componentTypeSet;
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Unit Tests ============================================================//

TEST(ComponentTypeSet, Construction)
{
	componentTypeSet l_set;

	ASSERT_TRUE(l_set.isEmpty());
}

TEST(ComponentTypeSet, Empty)
{
	ASSERT_TRUE(componentTypeSet::Empty().isEmpty());
}

TEST(ComponentTypeSet, SetTypeAndOperatorEquals)
{
	componentTypeSet l_left, l_right;

	ASSERT_TRUE(l_left == l_right);

	l_left.setType(componentType::forConcrete<int>());

	ASSERT_FALSE(l_left == l_right);

	l_right.setType(componentType::forConcrete<int>());

	ASSERT_TRUE(l_left == l_right);

	l_right.setType(componentType::forConcrete<int>());

	ASSERT_FALSE(l_left == l_right);
}

TEST(ComponentTypeSet, IsEmpty)
{
	componentTypeSet l_set;

	ASSERT_TRUE(l_set.isEmpty());

	l_set.setType(componentType::forConcrete<int>());

	ASSERT_FALSE(l_set.isEmpty());
}

TEST(ComponentTypeSet, OperatorLessThan)
{
	componentTypeSet l_left, l_right;

	ASSERT_TRUE(l_left.isEmpty());
	ASSERT_TRUE(l_right.isEmpty());
	ASSERT_TRUE(l_left == l_right);
	ASSERT_FALSE(l_left < l_right);
	ASSERT_FALSE(l_right < l_left);

	l_right.setType(componentType::forConcrete<int>());
	ASSERT_TRUE(l_left.isEmpty());
	ASSERT_FALSE(l_right.isEmpty());
	ASSERT_FALSE(l_left == l_right);
	ASSERT_TRUE(l_left < l_right);
	ASSERT_FALSE(l_right < l_left);

	l_left.setType(componentType::forConcrete<int>());
	ASSERT_FALSE(l_left.isEmpty());
	ASSERT_FALSE(l_right.isEmpty());
	ASSERT_TRUE(l_left == l_right);
	ASSERT_FALSE(l_left < l_right);
	ASSERT_FALSE(l_right < l_left);

	l_right.setType(componentType::forConcrete<float>());
	ASSERT_FALSE(l_left.isEmpty());
	ASSERT_FALSE(l_right.isEmpty());
	ASSERT_FALSE(l_left == l_right);
	ASSERT_TRUE(l_left < l_right);
	ASSERT_FALSE(l_right < l_left);

	ASSERT_TRUE(componentTypeSet::Empty() < l_left);
	ASSERT_TRUE(componentTypeSet::Empty() < l_right);

	ASSERT_FALSE(componentTypeSet::Empty() < componentTypeSet::Empty());
	ASSERT_FALSE(l_left < l_left);
	ASSERT_FALSE(l_right < l_right);
}

TEST(ComponentTypeSet, OperatorLessThanOrEqual)
{
	componentTypeSet l_left, l_right;

	ASSERT_TRUE(l_left.isEmpty());
	ASSERT_TRUE(l_right.isEmpty());
	ASSERT_TRUE(l_left == l_right);
	ASSERT_TRUE(l_left <= l_right);
	ASSERT_TRUE(l_right <= l_left);

	l_right.setType(componentType::forConcrete<int>());
	ASSERT_TRUE(l_left.isEmpty());
	ASSERT_FALSE(l_right.isEmpty());
	ASSERT_FALSE(l_left == l_right);
	ASSERT_TRUE(l_left <= l_right);
	ASSERT_FALSE(l_right <= l_left);

	l_left.setType(componentType::forConcrete<int>());
	ASSERT_FALSE(l_left.isEmpty());
	ASSERT_FALSE(l_right.isEmpty());
	ASSERT_TRUE(l_left == l_right);
	ASSERT_TRUE(l_left <= l_right);
	ASSERT_TRUE(l_right <= l_left);

	l_right.setType(componentType::forConcrete<float>());
	ASSERT_FALSE(l_left.isEmpty());
	ASSERT_FALSE(l_right.isEmpty());
	ASSERT_FALSE(l_left == l_right);
	ASSERT_TRUE(l_left <= l_right);
	ASSERT_FALSE(l_right <= l_left);

	ASSERT_TRUE(componentTypeSet::Empty() <= l_left);
	ASSERT_TRUE(componentTypeSet::Empty() <= l_right);

	ASSERT_TRUE(componentTypeSet::Empty() <= componentTypeSet::Empty());
	ASSERT_TRUE(l_left <= l_left);
	ASSERT_TRUE(l_right <= l_right);
}

TEST(ComponentTypeSet, HashValue)
{
	componentTypeSet l_set1, l_set2;

	ASSERT_EQ(l_set1.hashValue(), l_set2.hashValue());

	l_set1.setType(componentType::forConcrete<int>());
	ASSERT_FALSE(l_set1.hashValue() == l_set2.hashValue());

	l_set2.setType(componentType::forConcrete<int>());
	ASSERT_TRUE(l_set1.hashValue() == l_set2.hashValue());

	l_set2.setType(componentType::forConcrete<bool>());
	ASSERT_FALSE(l_set1.hashValue() == l_set2.hashValue());
}
