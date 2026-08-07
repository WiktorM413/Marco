#include "marco/JsonReader.h"
#include <iostream>
#include <marco/JsonValue.h>

int main()
{
	std::string jsonString = R"(
		{
			"name": "Anna",
			"age": 28,
			"isStudent": false,
			"skills": ["Python", "HTML", "JSON"],
			"address": {
				"city": "Wrocław",
				"zipCode": "51-317"
			},
			"phone": null
		}
	)";

	Marco::JsonReader jr(jsonString);

	auto s = jr["name"].AsString();

	if (s)
	{
		std::cout << jr["name"].AsString().value() << std::endl;
	}
	else
	{
		std::cout << "Encountered error: " << (int)s.error() << std::endl;
	}
	
	return 0;
}
