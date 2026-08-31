#pragma once

#include <cstddef>
namespace Marco
{
	enum class TomlErrorType
	{
		InvalidFormat,
		WrongType,
		InvalidNumberFormat,
		NoError
	};

	struct TomlError
	{
		TomlErrorType errorType;
		std::size_t index;
	};
}
