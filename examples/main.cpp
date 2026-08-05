#include <iostream>
#include <marco/JsonValue.h>

int main()
{
	Marco::JsonValue jv;

	jv["name"]["first"] = "Wiktor";
	
	std::cout << jv["name"]["first"].AsString().value();

	return 0;
}
