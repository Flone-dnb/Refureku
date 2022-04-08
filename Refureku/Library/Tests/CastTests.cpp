#include <gtest/gtest.h>
#include <Refureku/Refureku.h>

#include "TestCast.h"

//=========================================================
//========== rfk::dynamicCast<Target, Source> =============
//=========================================================

TEST(Rfk_dynamicCast_Target_Source, CastToStaticSelfType)
{
	//Doesn't compile as expected
	//rfk::dynamicCast<GrandChild1>(&grandChild1);
}

TEST(Rfk_dynamicCast_Target_Source, CastToSourceParentStaticType)
{
	//Doesn't compile as expected because upcast
	//rfk::dynamicCast<Child1>(&grandChild1);
}

TEST(Rfk_dynamicCast_Target_Source, SuccessfulDownCastToSelfType)
{
	GrandChild1 grandChild1;
	Base* grandChild1AsBase = &grandChild1;

	EXPECT_EQ(rfk::dynamicCast<GrandChild1>(grandChild1AsBase), &grandChild1);
}

TEST(Rfk_dynamicCast_Target_Source, SuccessfulDownCastNotConstToNotConst)
{
	GrandChild1 grandChild1;
	Base* grandChild1AsBase = &grandChild1;
	Child1* grandChild1AsChild1 = &grandChild1;

	EXPECT_EQ(rfk::dynamicCast<Child1>(grandChild1AsBase), grandChild1AsChild1);
}

TEST(Rfk_dynamicCast_Target_Source, SuccessfulDownCastConstToConst)
{
	Child1 child1;
	GrandChild1 grandChild1;

	Base const* grandChild1AsBase = &grandChild1;
	EXPECT_EQ(rfk::dynamicCast<GrandChild1 const>(grandChild1AsBase), &grandChild1);

	Child1 const* grandChild1AsChild1 = &grandChild1;
	EXPECT_EQ(rfk::dynamicCast<Child1 const>(grandChild1AsBase), grandChild1AsChild1);
}

TEST(Rfk_dynamicCast_Target_Source, SuccessfulDownCastNotConstToConst)
{
	Child1 child1;
	GrandChild1 grandChild1;

	Base* grandChild1AsBase = &grandChild1;
	EXPECT_EQ(rfk::dynamicCast<GrandChild1 const>(grandChild1AsBase), &grandChild1);

	Child1* grandChild1AsChild1 = &grandChild1;
	EXPECT_EQ(rfk::dynamicCast<Child1 const>(grandChild1AsBase), grandChild1AsChild1);
}

TEST(Rfk_dynamicCast_Target_Source, DownCastConstToNotConst)
{
	Child1 child1;
	GrandChild1 grandChild1;

	//This doesn't compile as expected: const -> non-const implicit conversion forbidden
	/*
	Base const* grandChild1AsBase = &grandChild1;
	EXPECT_EQ(rfk::dynamicCast<GrandChild1>(grandChild1AsBase), &grandChild1);

	Child1 const* grandChild1AsChild1 = &grandChild1;
	EXPECT_EQ(rfk::dynamicCast<Child1>(grandChild1AsBase), grandChild1AsChild1);
	*/
}

TEST(Rfk_dynamicCast_Target_Source, FailedDownCast)
{
	Child1 child1;

	EXPECT_EQ(rfk::dynamicCast<GrandChild1>(&child1), nullptr);
}

TEST(Rfk_dynamicCast_Target_Source, SuccessfulDynamicCastWithMultipleInheritance)
{
	Child3 child3;

	Base*	child3AsBase = &child3;
	Child1*	child3AsChild1 = &child3;
	Base2*	child3AsBase2 = &child3;

	EXPECT_EQ(rfk::dynamicCast<Child1>(child3AsBase), child3AsChild1);
	EXPECT_EQ(rfk::dynamicCast<Base2>(child3AsBase), child3AsBase2);
	EXPECT_EQ(rfk::dynamicCast<Base>(child3AsBase2), child3AsBase);
}