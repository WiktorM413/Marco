#include <marco/JsonReader.h>
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

	Marco::JsonReader jr;

	auto json = jr.Parse(jsonString);

	auto s = json["phone"].AsNull();

	if (s)
	{
		std::cout << "value: " << s.value() << std::endl;
	}
	else
	{
		std::cout << "Error number: " << (int)s.error().errorType << std::endl;
	}
		
	return 0;
}
