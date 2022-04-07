/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

template <typename CallerType, typename ReturnType, typename... ArgTypes>
MemberFunction<CallerType, ReturnType(ArgTypes...)>::MemberFunction(FunctionPrototype function) noexcept:
	_function{function},
	_isConst{false}
#if (defined(_WIN32) || defined(_WIN64)) && RFK_DEBUG
	, _originalFunctionSize{sizeof(FunctionPrototype)}
#endif
{}

template <typename CallerType, typename ReturnType, typename... ArgTypes>
MemberFunction<CallerType, ReturnType(ArgTypes...)>::MemberFunction(ConstFunctionPrototype function) noexcept:
	_constFunction{function},
	_isConst{true}
#if (defined(_WIN32) || defined(_WIN64)) && RFK_DEBUG
	, _originalFunctionSize{sizeof(ConstFunctionPrototype)}
#endif
{}

#if (defined(_WIN32) || defined(_WIN64)) && RFK_DEBUG
template <typename CallerType, typename ReturnType, typename... ArgTypes>
std::size_t MemberFunction<CallerType, ReturnType(ArgTypes...)>::getOriginalFunctionSize() const noexcept
{
	return _originalFunctionSize;
}
#endif

template <typename CallerType, typename ReturnType, typename... ArgTypes>
ReturnType MemberFunction<CallerType, ReturnType(ArgTypes...)>::operator()(CallerType& caller, ArgTypes&&... args) const
{
	return _isConst ?	(caller.*_constFunction)(std::forward<ArgTypes>(args)...) :
						(caller.*_function)(std::forward<ArgTypes>(args)...);
}

template <typename CallerType, typename ReturnType, typename... ArgTypes>
ReturnType MemberFunction<CallerType, ReturnType(ArgTypes...)>::operator()(CallerType const& caller, ArgTypes&&... args) const
{
	assert(_isConst);

	return (caller.*_constFunction)(std::forward<ArgTypes>(args)...);
}