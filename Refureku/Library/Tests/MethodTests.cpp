#include <stdexcept>	//std::logic_error

#include <gtest/gtest.h>
#include <Refureku/Refureku.h>

#include "TestMethods.h"
#include "ForwardDeclaredClass.h"

//=========================================================
//=================== Method::invoke ======================
//=========================================================

//TEST(Rfk_Method_invoke, CallNonConstMethodOnNonConstInstance)
//{
//	TestMethodClass instance;
//
//	EXPECT_EQ(TestMethodClass::staticGetArchetype().getMethodByName("returnIntNoParam")->invoke<int>(instance), 0);
//	EXPECT_EQ(TestMethodClass::staticGetArchetype().getMethodByName("returnIntParamInt")->invoke<int>(instance, 42), 42);
//}
//
//TEST(Rfk_Method_invoke, CallNonConstMethodOnConstInstance)
//{
//	TestMethodClass const instance;
//
//	EXPECT_THROW(TestMethodClass::staticGetArchetype().getMethodByName("returnIntNoParam")->invoke<int>(instance), rfk::ConstViolation);
//	EXPECT_THROW(TestMethodClass::staticGetArchetype().getMethodByName("returnIntParamInt")->invoke<int>(instance, 42), rfk::ConstViolation);
//}
//
//TEST(Rfk_Method_invoke, CallConstMethodOnNonConstInstance)
//{
//	TestMethodClass instance;
//
//	EXPECT_NO_THROW(TestMethodClass::staticGetArchetype().getMethodByName("constNoReturnNoParam")->invoke(instance));
//}
//
//TEST(Rfk_Method_invoke, CallConstMethodOnConstInstance)
//{
//	TestMethodClass const instance;
//
//	EXPECT_NO_THROW(TestMethodClass::staticGetArchetype().getMethodByName("constNoReturnNoParam")->invoke(instance));
//}
//
//TEST(Rfk_Method_invoke, ThrowingCall)
//{
//	TestMethodClass instance;
//
//	EXPECT_THROW(TestMethodClass::staticGetArchetype().getMethodByName("throwing")->invoke(instance), std::logic_error);
//}

//Call a non-virtual method from self class

//1. self class doesn't inherit from any class
TEST(Rfk_Method_invoke, CallSelfIntroducedMethodOnNoInheritanceNPClass)
{
	NoInheritanceNPClass instance;
	
	EXPECT_EQ(
		NoInheritanceNPClass::staticGetArchetype().getMethodByName("methodNoInheritanceNPClass")->invoke<EMethodTestCallResult>(instance),
		EMethodTestCallResult::NoInheritanceNPClass
	);
}

//2. self class inherits from one non-polymorphic class
TEST(Rfk_Method_invoke, CallSelfIntroducedMethodOnSingleNPInheritanceNPClass)
{
	SingleNPInheritanceNPClass instance;

	EXPECT_EQ(
		SingleNPInheritanceNPClass::staticGetArchetype().getMethodByName("methodSingleNPInheritanceNPClass")->invoke<EMethodTestCallResult>(instance),
		EMethodTestCallResult::SingleNPInheritanceNPClass
	);
}

//3. self class inherits from multiple non-polymorphic classes
TEST(Rfk_Method_invoke, CallSelfIntroducedMethodOnMultipleNPInheritanceNPClass)
{
	MultipleNPInheritanceNPClass instance;

	EXPECT_EQ(
		MultipleNPInheritanceNPClass::staticGetArchetype().getMethodByName("methodMultipleNPInheritanceNPClass")->invoke<EMethodTestCallResult>(instance),
		EMethodTestCallResult::MultipleNPInheritanceNPClass
	);
}

//4. self class inherits from multiple classes including polymorphic classes
TEST(Rfk_Method_invoke, CallSelfIntroducedMethodOnMultiplePInheritancePClass)
{
	MultiplePInheritancePClassMethodOverride instance;

	EXPECT_EQ(
		MultiplePInheritancePClassMethodOverride::staticGetArchetype().getMethodByName("methodMultiplePInheritancePClassMethodOverride")->invoke<EMethodTestCallResult>(instance),
		EMethodTestCallResult::MultiplePInheritancePClassMethodOverride
	);
}

//Call a virtual method from self class

//1. self class doesn't inherit from any class
TEST(Rfk_Method_invoke, CallSelfIntroducedVirtualMethodOnNoInheritancePClass)
{
	NoInheritancePClass instance;

	EXPECT_EQ(
		NoInheritancePClass::staticGetArchetype().getMethodByName("methodNoInheritancePClass")->invoke<EMethodTestCallResult>(instance),
		EMethodTestCallResult::NoInheritancePClass
	);
}

//2. self class inherits from one non-polymorphic class
TEST(Rfk_Method_invoke, CallSelfIntroducedVirtualMethodOnSingleNPInheritancePClass)
{
	SingleNPInheritancePClass instance;

	EXPECT_EQ(
		SingleNPInheritancePClass::staticGetArchetype().getMethodByName("methodSingleNPInheritancePClass")->invoke<EMethodTestCallResult>(instance),
		EMethodTestCallResult::SingleNPInheritancePClass
	);
}

//3. self class inherits from multiple non-polymorphic classes
TEST(Rfk_Method_invoke, CallSelfIntroducedVirtualMethodOnMultipleNPInheritancePClass)
{
	MultipleNPInheritancePClass instance;

	EXPECT_EQ(
		MultipleNPInheritancePClass::staticGetArchetype().getMethodByName("methodMultipleNPInheritancePClass")->invoke<EMethodTestCallResult>(instance),
		EMethodTestCallResult::MultipleNPInheritancePClass
	);
}

//4. self class inherits from multiple classes including polymorphic classes
TEST(Rfk_Method_invoke, CallSelfIntroducedVirtualMethodOnMultiplePInheritancePClass)
{
	MultiplePInheritancePClassMethodOverride instance;

	EXPECT_EQ(
		MultiplePInheritancePClassMethodOverride::staticGetArchetype().getMethodByName("methodMultiplePInheritancePClassMethodOverride")->invoke<EMethodTestCallResult>(instance),
		EMethodTestCallResult::MultiplePInheritancePClassMethodOverride
	);
}

//=========================================================
//================ Method::checkedInvoke ==================
//=========================================================

//TEST(Rfk_Method_checkedInvoke, SuccessfullCall)
//{
//	TestMethodClass instance;
//
//	EXPECT_EQ(TestMethodClass::staticGetArchetype().getMethodByName("returnIntNoParam")->checkedInvoke<int>(instance), 0);
//	EXPECT_EQ(TestMethodClass::staticGetArchetype().getMethodByName("returnIntParamInt")->checkedInvoke<int>(instance, 42), 42);
//}
//
//TEST(Rfk_Method_checkedInvoke, ThrowReturnTypeMismatch)
//{
//	TestMethodClass instance;
//
//	EXPECT_THROW(TestMethodClass::staticGetArchetype().getMethodByName("returnIntNoParam")->checkedInvoke<float>(instance), rfk::ReturnTypeMismatch);
//}
//
//TEST(Rfk_Method_checkedInvoke, ReturnNonReflectedType)
//{
//	TestMethodClass instance;
//	NonReflectedClass nrc;
//
//	//Calling with a different non-reflected type is not catched by the checked call
//	EXPECT_NO_THROW(TestMethodClass::staticGetArchetype().getMethodByName("returnNonReflectedNoParam")->checkedInvoke<NonReflectedClass2&>(instance, nrc));
//	EXPECT_NO_THROW(TestMethodClass::staticGetArchetype().getMethodByName("returnNonReflectedNoParam")->checkedInvoke<NonReflectedClass&>(instance, nrc));
//}
//
//TEST(Rfk_Method_checkedInvoke, ThrowArgCountMismatch)
//{
//	TestMethodClass instance;
//
//	EXPECT_THROW(TestMethodClass::staticGetArchetype().getMethodByName("returnIntParamInt")->checkedInvoke<int>(instance, 1, 2), rfk::ArgCountMismatch);	//2 instead of 1
//	EXPECT_THROW(TestMethodClass::staticGetArchetype().getMethodByName("noReturnNoParam")->checkedInvoke(instance, 1), rfk::ArgCountMismatch);		//1 instead of 0
//	EXPECT_NO_THROW(TestMethodClass::staticGetArchetype().getMethodByName("noReturnNoParam")->checkedInvoke(instance));
//}
//
//TEST(Rfk_Method_checkedInvoke, ThrowArgTypeMismatch)
//{
//	TestMethodClass instance;
//
//	EXPECT_THROW(TestMethodClass::staticGetArchetype().getMethodByName("returnIntParamInt")->checkedInvoke<int>(instance, 1.0f), rfk::ArgTypeMismatch);	//float instead of int
//}
//
//TEST(Rfk_Method_checkedInvoke, CatchForwardDeclaredTypeMismatch)
//{
//	TestMethodClass instance;
//	ForwardDeclaredClass fdc;
//	float value;
//
//	EXPECT_THROW(TestMethodClass::staticGetArchetype().getMethodByName("returnFwdClassParamFwdClass")->checkedInvoke<int&>(instance, &fdc), rfk::ReturnTypeMismatch);
//	EXPECT_THROW(TestMethodClass::staticGetArchetype().getMethodByName("returnFwdClassParamFwdClass")->checkedInvoke<ForwardDeclaredClass*>(instance, value), rfk::ArgTypeMismatch);
//	EXPECT_EQ(TestMethodClass::staticGetArchetype().getMethodByName("returnFwdClassParamFwdClass")->checkedInvoke<ForwardDeclaredClass*>(instance, &fdc), &fdc);	//Normal call
//}
//
//TEST(Rfk_Method_checkedInvoke, NonReflectedTypeBypassArgTypeMismatch)
//{
//	TestMethodClass instance;
//	NonReflectedClass nrc;
//
//	EXPECT_NO_THROW(TestMethodClass::staticGetArchetype().getMethodByName("returnNonReflectedNoParam")->checkedInvoke<NonReflectedClass&>(instance, nrc));
//	EXPECT_EQ(nrc.i, 42);
//}
//
//TEST(Rfk_Method_checkedInvoke, ThrowingCall)
//{
//	TestMethodClass instance;
//
//	EXPECT_THROW(TestMethodClass::staticGetArchetype().getMethodByName("throwing")->checkedInvoke(instance), std::logic_error);
//}