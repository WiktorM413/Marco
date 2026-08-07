#pragma once

namespace Marco
{
	enum class JsonError
	{
		InvalidFormat,
		WrongType,
		NumberParseError,
		NumberOutOfRange,
		NoError
	};
}