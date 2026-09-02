#pragma once

#include <cstddef>
namespace Marco
{
	enum class TomlErrorType
	{
		NoError,
		InvalidFormat,
		WrongType,
		InvalidNumberFormat
	};

	struct TomlError
	{
		TomlErrorType errorType;
		std::size_t index;
	};
}
