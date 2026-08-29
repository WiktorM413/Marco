#include <marco/json/Json.h>
#include <marco/json/JsonWriter.h>
#include <marco/json/JsonReader.h>
#include <iostream>
#include <marco/json/JsonValue.h>

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
	Marco::Json json = jr.Parse(jsonString);

	Marco::JsonWriter jw;
	std::string s = jw.Write(json);

	std::cout << s << std::endl;
		
	return 0;
}
