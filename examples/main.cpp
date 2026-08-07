#include "marco/JsonReader.h"
#include <iostream>
#include <marco/JsonValue.h>

int main()
{
	std::string jsonString = R"(
		{
			"age": 28,
			"name": "Anna",
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

	auto s = jr["skills"][0].AsString();

	if (s)
	{
		std::cout << s.value() << std::endl;
	}
	else
	{
		std::cout << "Encountered error: " << (int)s.error() << std::endl;
	}
	
	return 0;
}
