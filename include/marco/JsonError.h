#pragma once

#include <cstddef>
namespace Marco
{
	enum class JsonErrorType
	{
		InvalidFormat,
		WrongType,
		NumberParseError,
		NumberOutOfRange,
		NotObject,
		NoError
	};

	struct JsonError
	{
		JsonErrorType errorType;
		std::size_t   index;
	};
}