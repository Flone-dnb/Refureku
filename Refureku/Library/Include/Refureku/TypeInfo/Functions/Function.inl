/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

template <typename ReturnType, typename... ArgTypes>
ReturnType Function::internalInvoke(ArgTypes&&... arguments) const noexcept
{
	return reinterpret_cast<NonMemberFunction<ReturnType(ArgTypes...)>*>(internalMethod.get())->operator()(std::forward<ArgTypes>(arguments)...);
}

template <typename... ArgTypes>
void Function::invoke(ArgTypes&&... arguments) const noexcept(REFUREKU_RELEASE)
{
	#if REFUREKU_DEBUG

	checkArgumentsCount<ArgTypes...>();

	#endif

	internalInvoke<void, ArgTypes...>(std::forward<ArgTypes>(arguments)...);
}

template <typename ReturnType, typename... ArgTypes>
ReturnType Function::invoke(ArgTypes&&... arguments) const noexcept(REFUREKU_RELEASE)
{
	#if REFUREKU_DEBUG

	checkArgumentsCount<ArgTypes...>();

	#endif

	return internalInvoke<ReturnType, ArgTypes...>(std::forward<ArgTypes>(arguments)...);
}

template <typename... ArgTypes>
void Function::checkedInvoke(ArgTypes&&... arguments) const
{
	checkArguments<ArgTypes...>();

	internalInvoke<void, ArgTypes...>(std::forward<ArgTypes>(arguments)...);
}

template <typename ReturnType, typename... ArgTypes>
ReturnType Function::checkedInvoke(ArgTypes&&... arguments) const
{
	checkReturnType<ReturnType>();
	checkArguments<ArgTypes...>();

	return internalInvoke<ReturnType, ArgTypes...>(std::forward<ArgTypes>(arguments)...);
}