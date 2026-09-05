#include "marco/toml/Toml.h"
#include "marco/toml/TomlReader.h"

#include <iostream>
#include <string>

int main()
{
	std::string tomlString = R"(
		title = "TOML Example Configuration"

		[app]
		name = "MyApp"
		version = "1.2.3"
		description = "A sleek, minimal web service."
		production_ready = true
		max_connections = 5000
		pi_value = 3.14159

		[strings]
		escaped = "Line 1\nLine 2\tTabbed"
		literal_path = 'C:\Users\admin\Documents'

		multi_line_basic = """
		The quick brown fox
		jumps over the lazy dog.
		"""

		multi_line_literal = '''
		I [dw]on't need \d{2} apples
		Regex patterns work perfectly here.
		'''

		[numbers_and_dates]
		large_number = 1_000_000
		binary_mode = 0b1010
		hex_color = 0xDEADBEEF
		created_at = 2026-09-01T17:15:00Z
		local_date = 2026-09-01

		[collections]
		tags = [ "production", "web", "api" ]
		ports = [ 8080, 8081, 8082 ]
		coordinates = { x = 12, y = -5, label = "Target Location" }

		[database]
		server = "192.168.1.100"
		ports = [ 5432, 5433 ]
		connection_max = 5000
		enabled = true

		[database.credentials]
		username = "db_admin"
		password = "super_secure_password_123"

		[[products]]
		name = "Wireless Mouse"
		sku = 940231
		price = 29.99

		[[products]]
		name = "Mechanical Keyboard"
		sku = 940232
		price = 89.99
	)";

	Marco::TomlReader rd;
	Marco::Toml toml = rd.Parse(tomlString);

	if (!rd.IsValid())
	{
		std::cout << "Parse failed\n";
		return 1;
	}

	// Root
	std::cout << "title: "
			  << toml["title"].AsString()->get()
			  << '\n';

	// App
	std::cout << "app.name: "
			  << toml["app"]["name"].AsString()->get()
			  << '\n';

	std::cout << "app.version: "
			  << toml["app"]["version"].AsString()->get()
			  << '\n';

	std::cout << "app.description: "
			  << toml["app"]["description"].AsString()->get()
			  << '\n';

	std::cout << "app.production_ready: "
			  << toml["app"]["production_ready"].AsBool().value()
			  << '\n';

	std::cout << "app.max_connections: "
			  << toml["app"]["max_connections"].AsNumber().value()
			  << '\n';

	std::cout << "app.pi_value: "
			  << toml["app"]["pi_value"].AsNumber().value()
			  << '\n';

	// Strings
	std::cout << "strings.escaped: "
			  << toml["strings"]["escaped"].AsString()->get()
			  << '\n';

	std::cout << "strings.literal_path: "
			  << toml["strings"]["literal_path"].AsString()->get()
			  << '\n';

	std::cout << "strings.multi_line_basic: "
			  << toml["strings"]["multi_line_basic"].AsString()->get()
			  << '\n';

	std::cout << "strings.multi_line_literal: "
			  << toml["strings"]["multi_line_literal"].AsString()->get()
			  << '\n';

	// Numbers / dates
	std::cout << "numbers.large_number: "
			  << toml["numbers_and_dates"]["large_number"].AsNumber().value()
			  << '\n';

	std::cout << "numbers.binary_mode: "
			  << toml["numbers_and_dates"]["binary_mode"].AsNumber().value()
			  << '\n';

	std::cout << "numbers.hex_color: "
			  << toml["numbers_and_dates"]["hex_color"].AsNumber().value()
			  << '\n';

	std::cout << "numbers.created_at: "
			  << toml["numbers_and_dates"]["created_at"].AsDate().value().AsString()
			  << '\n';

	std::cout << "numbers.local_date: "
			  << toml["numbers_and_dates"]["local_date"].AsDate().value().AsString()
			  << '\n';

	// Arrays
	std::cout << "collections.tags[0]: "
			  << toml["collections"]["tags"][0].AsString()->get()
			  << '\n';

	std::cout << "collections.tags[1]: "
			  << toml["collections"]["tags"][1].AsString()->get()
			  << '\n';

	std::cout << "collections.tags[2]: "
			  << toml["collections"]["tags"][2].AsString()->get()
			  << '\n';

	std::cout << "collections.ports[0]: "
			  << toml["collections"]["ports"][0].AsNumber().value()
			  << '\n';

	std::cout << "collections.ports[1]: "
			  << toml["collections"]["ports"][1].AsNumber().value()
			  << '\n';

	std::cout << "collections.ports[2]: "
			  << toml["collections"]["ports"][2].AsNumber().value()
			  << '\n';

	// Inline table
	std::cout << "coordinates.x: "
			  << toml["collections"]["coordinates"]["x"].AsNumber().value()
			  << '\n';

	std::cout << "coordinates.y: "
			  << toml["collections"]["coordinates"]["y"].AsNumber().value()
			  << '\n';

	std::cout << "coordinates.label: "
			  << toml["collections"]["coordinates"]["label"].AsString()->get()
			  << '\n';

	// Database
	std::cout << "database.server: "
			  << toml["database"]["server"].AsString()->get()
			  << '\n';

	std::cout << "database.ports[0]: "
			  << toml["database"]["ports"][0].AsNumber().value()
			  << '\n';

	std::cout << "database.ports[1]: "
			  << toml["database"]["ports"][1].AsNumber().value()
			  << '\n';

	std::cout << "database.connection_max: "
			  << toml["database"]["connection_max"].AsNumber().value()
			  << '\n';

	std::cout << "database.enabled: "
			  << toml["database"]["enabled"].AsBool().value()
			  << '\n';

	std::cout << "database.credentials.username: "
			  << toml["database"]["credentials"]["username"].AsString()->get()
			  << '\n';

	std::cout << "database.credentials.password: "
			  << toml["database"]["credentials"]["password"].AsString()->get()
			  << '\n';

	// Array of tables
	std::cout << "products[0].name: "
			  << toml["products"][0]["name"].AsString()->get()
			  << '\n';

	std::cout << "products[0].sku: "
			  << toml["products"][0]["sku"].AsNumber().value()
			  << '\n';

	std::cout << "products[0].price: "
			  << toml["products"][0]["price"].AsNumber().value()
			  << '\n';

	std::cout << "products[1].name: "
			  << toml["products"][1]["name"].AsString()->get()
			  << '\n';

	std::cout << "products[1].sku: "
			  << toml["products"][1]["sku"].AsNumber().value()
			  << '\n';

	std::cout << "products[1].price: "
			  << toml["products"][1]["price"].AsNumber().value()
			  << '\n';

	return 0;
}