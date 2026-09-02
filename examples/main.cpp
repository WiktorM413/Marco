#include "marco/toml/Toml.h"
#include "marco/toml/TomlReader.h"
#include <iostream>

int main()
{
	std::string tomlString = R"(
# This is a comment in a TOML file
# TOML files are case-sensitive and rely on key = value pairs

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

[numbers]
large_number = 1_000_000
binary_mode = 0b1010
octal_mode = 0o755
hex_color = 0xDEADBEEF

[collections]
tags = [ "production", "web", "api" ]
ports = [
  8080,
  8081,
  8082
]

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

	Marco::TomlReader reader{};
	Marco::Toml toml = reader.Parse(tomlString);

	std::cout << "========== PARSER ==========\n";
	std::cout << "Valid: " << reader.IsValid() << '\n';
	std::cout << "Error: " << static_cast<int>(reader.Error().errorType)
			  << " at " << reader.Error().index << "\n\n";

	std::cout << "========== ROOT ==========\n";

	std::cout << "title:\n";
	std::cout << "  empty:  " << toml["title"].IsEmpty() << '\n';
	std::cout << "  string: " << toml["title"].IsString() << '\n';

	auto title = toml["title"].AsString();
	if (title)
		std::cout << "  value:  " << title.value().get() << '\n';
	else
		std::cout << "  ERROR:  " << static_cast<int>(title.error().errorType) << '\n';

	std::cout << "\n========== APP ==========\n";

	std::cout << "app.name:\n";
	std::cout << "  string: " << toml["app"]["name"].IsString() << '\n';
	auto appName = toml["app"]["name"].AsString();
	if (appName)
		std::cout << "  value: " << appName.value().get() << '\n';

	std::cout << "app.version:\n";
	std::cout << "  string: " << toml["app"]["version"].IsString() << '\n';
	auto appVersion = toml["app"]["version"].AsString();
	if (appVersion)
		std::cout << "  value: " << appVersion.value().get() << '\n';

	std::cout << "app.description:\n";
	std::cout << "  string: " << toml["app"]["description"].IsString() << '\n';
	auto appDescription = toml["app"]["description"].AsString();
	if (appDescription)
		std::cout << "  value: " << appDescription.value().get() << '\n';

	std::cout << "app.production_ready:\n";
	std::cout << "  bool: " << toml["app"]["production_ready"].IsBool() << '\n';
	auto productionReady = toml["app"]["production_ready"].AsBool();
	if (productionReady)
		std::cout << "  value: " << productionReady.value() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(productionReady.error().errorType) << '\n';

	std::cout << "app.max_connections:\n";
	std::cout << "  number: " << toml["app"]["max_connections"].IsNumber() << '\n';
	auto maxConnections = toml["app"]["max_connections"].AsNumber();
	if (maxConnections)
		std::cout << "  value: " << maxConnections.value() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(maxConnections.error().errorType) << '\n';

	std::cout << "app.pi_value:\n";
	std::cout << "  number: " << toml["app"]["pi_value"].IsNumber() << '\n';
	auto piValue = toml["app"]["pi_value"].AsNumber();
	if (piValue)
		std::cout << "  value: " << piValue.value() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(piValue.error().errorType) << '\n';

	std::cout << "\n========== STRINGS ==========\n";

	std::cout << "strings.escaped:\n";
	std::cout << "  string: " << toml["strings"]["escaped"].IsString() << '\n';
	auto escaped = toml["strings"]["escaped"].AsString();
	if (escaped)
		std::cout << "  value: [" << escaped.value().get() << "]\n";
	else
		std::cout << "  ERROR: " << static_cast<int>(escaped.error().errorType) << '\n';

	std::cout << "strings.literal_path:\n";
	std::cout << "  string: " << toml["strings"]["literal_path"].IsString() << '\n';
	auto literalPath = toml["strings"]["literal_path"].AsString();
	if (literalPath)
		std::cout << "  value: [" << literalPath.value().get() << "]\n";
	else
		std::cout << "  ERROR: " << static_cast<int>(literalPath.error().errorType) << '\n';

	std::cout << "strings.multi_line_basic:\n";
	std::cout << "  string: " << toml["strings"]["multi_line_basic"].IsString() << '\n';
	auto multiLineBasic = toml["strings"]["multi_line_basic"].AsString();
	if (multiLineBasic)
		std::cout << "  value: [" << multiLineBasic.value().get() << "]\n";
	else
		std::cout << "  ERROR: " << static_cast<int>(multiLineBasic.error().errorType) << '\n';

	std::cout << "strings.multi_line_literal:\n";
	std::cout << "  string: " << toml["strings"]["multi_line_literal"].IsString() << '\n';
	auto multiLineLiteral = toml["strings"]["multi_line_literal"].AsString();
	if (multiLineLiteral)
		std::cout << "  value: [" << multiLineLiteral.value().get() << "]\n";
	else
		std::cout << "  ERROR: " << static_cast<int>(multiLineLiteral.error().errorType) << '\n';

	std::cout << "\n========== NUMBERS ==========\n";

	std::cout << "numbers.large_number:\n";
	std::cout << "  number: " << toml["numbers"]["large_number"].IsNumber() << '\n';
	auto largeNumber = toml["numbers"]["large_number"].AsNumber();
	if (largeNumber)
		std::cout << "  value: " << largeNumber.value() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(largeNumber.error().errorType) << '\n';

	std::cout << "numbers.binary_mode:\n";
	std::cout << "  number: " << toml["numbers"]["binary_mode"].IsNumber() << '\n';
	auto binaryMode = toml["numbers"]["binary_mode"].AsNumber();
	if (binaryMode)
		std::cout << "  value: " << binaryMode.value() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(binaryMode.error().errorType) << '\n';

	std::cout << "numbers.octal_mode:\n";
	std::cout << "  number: " << toml["numbers"]["octal_mode"].IsNumber() << '\n';
	auto octalMode = toml["numbers"]["octal_mode"].AsNumber();
	if (octalMode)
		std::cout << "  value: " << octalMode.value() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(octalMode.error().errorType) << '\n';

	std::cout << "numbers.hex_color:\n";
	std::cout << "  number: " << toml["numbers"]["hex_color"].IsNumber() << '\n';
	auto hexColor = toml["numbers"]["hex_color"].AsNumber();
	if (hexColor)
		std::cout << "  value: " << hexColor.value() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(hexColor.error().errorType) << '\n';

	std::cout << "\n========== COLLECTIONS ==========\n";

	std::cout << "collections.tags:\n";
	std::cout << "  array: " << toml["collections"]["tags"].IsArray() << '\n';

	auto tags = toml["collections"]["tags"].AsArray();
	if (tags)
	{
		std::cout << "  size: " << tags.value().get().size() << '\n';

		for (size_t i = 0; i < tags.value().get().size(); ++i)
		{
			std::cout << "  [" << i << "] string: "
					  << tags.value().get()[i].IsString() << '\n';

			auto value = tags.value().get()[i].AsString();
			if (value)
				std::cout << "  [" << i << "] value: "
						  << value.value().get() << '\n';
			else
				std::cout << "  [" << i << "] ERROR: "
						  << static_cast<int>(value.error().errorType) << '\n';
		}
	}
	else
	{
		std::cout << "  ERROR: "
				  << static_cast<int>(tags.error().errorType) << '\n';
	}

	std::cout << "collections.ports:\n";
	std::cout << "  array: " << toml["collections"]["ports"].IsArray() << '\n';

	auto collectionPorts = toml["collections"]["ports"].AsArray();
	if (collectionPorts)
	{
		std::cout << "  size: " << collectionPorts.value().get().size() << '\n';

		for (size_t i = 0; i < collectionPorts.value().get().size(); ++i)
		{
			std::cout << "  [" << i << "] number: "
					  << collectionPorts.value().get()[i].IsNumber() << '\n';

			auto value = collectionPorts.value().get()[i].AsNumber();
			if (value)
				std::cout << "  [" << i << "] value: "
						  << value.value() << '\n';
			else
				std::cout << "  [" << i << "] ERROR: "
						  << static_cast<int>(value.error().errorType) << '\n';
		}
	}
	else
	{
		std::cout << "  ERROR: "
				  << static_cast<int>(collectionPorts.error().errorType) << '\n';
	}

	std::cout << "collections.coordinates:\n";
	std::cout << "  object: "
			  << toml["collections"]["coordinates"].IsObject() << '\n';

	std::cout << "collections.coordinates.x:\n";
	std::cout << "  number: "
			  << toml["collections"]["coordinates"]["x"].IsNumber() << '\n';

	auto x = toml["collections"]["coordinates"]["x"].AsNumber();
	if (x)
		std::cout << "  value: " << x.value() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(x.error().errorType) << '\n';

	std::cout << "collections.coordinates.y:\n";
	std::cout << "  number: "
			  << toml["collections"]["coordinates"]["y"].IsNumber() << '\n';

	auto y = toml["collections"]["coordinates"]["y"].AsNumber();
	if (y)
		std::cout << "  value: " << y.value() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(y.error().errorType) << '\n';

	std::cout << "collections.coordinates.label:\n";
	std::cout << "  string: "
			  << toml["collections"]["coordinates"]["label"].IsString() << '\n';

	auto label = toml["collections"]["coordinates"]["label"].AsString();
	if (label)
		std::cout << "  value: " << label.value().get() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(label.error().errorType) << '\n';

	std::cout << "\n========== DATABASE ==========\n";

	std::cout << "database.server:\n";
	std::cout << "  string: " << toml["database"]["server"].IsString() << '\n';

	auto server = toml["database"]["server"].AsString();
	if (server)
		std::cout << "  value: " << server.value().get() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(server.error().errorType) << '\n';

	std::cout << "database.ports:\n";
	std::cout << "  array: " << toml["database"]["ports"].IsArray() << '\n';

	auto databasePorts = toml["database"]["ports"].AsArray();
	if (databasePorts)
	{
		std::cout << "  size: " << databasePorts.value().get().size() << '\n';

		for (size_t i = 0; i < databasePorts.value().get().size(); ++i)
		{
			std::cout << "  [" << i << "] number: "
					  << databasePorts.value().get()[i].IsNumber() << '\n';

			auto value = databasePorts.value().get()[i].AsNumber();
			if (value)
				std::cout << "  [" << i << "] value: "
						  << value.value() << '\n';
			else
				std::cout << "  [" << i << "] ERROR: "
						  << static_cast<int>(value.error().errorType) << '\n';
		}
	}
	else
	{
		std::cout << "  ERROR: "
				  << static_cast<int>(databasePorts.error().errorType) << '\n';
	}

	std::cout << "database.connection_max:\n";
	std::cout << "  number: " << toml["database"]["connection_max"].IsNumber() << '\n';

	auto connectionMax = toml["database"]["connection_max"].AsNumber();
	if (connectionMax)
		std::cout << "  value: " << connectionMax.value() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(connectionMax.error().errorType) << '\n';

	std::cout << "database.enabled:\n";
	std::cout << "  bool: " << toml["database"]["enabled"].IsBool() << '\n';

	auto enabled = toml["database"]["enabled"].AsBool();
	if (enabled)
		std::cout << "  value: " << enabled.value() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(enabled.error().errorType) << '\n';

	std::cout << "database.credentials:\n";
	std::cout << "  object: "
			  << toml["database"]["credentials"].IsObject() << '\n';

	std::cout << "database.credentials.username:\n";
	std::cout << "  string: "
			  << toml["database"]["credentials"]["username"].IsString() << '\n';

	auto username = toml["database"]["credentials"]["username"].AsString();
	if (username)
		std::cout << "  value: " << username.value().get() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(username.error().errorType) << '\n';

	std::cout << "database.credentials.password:\n";
	std::cout << "  string: "
			  << toml["database"]["credentials"]["password"].IsString() << '\n';

	auto password = toml["database"]["credentials"]["password"].AsString();
	if (password)
		std::cout << "  value: " << password.value().get() << '\n';
	else
		std::cout << "  ERROR: " << static_cast<int>(password.error().errorType) << '\n';

	std::cout << "\n========== PRODUCTS ==========\n";

	std::cout << "products:\n";
	std::cout << "  array: " << toml["products"].IsArray() << '\n';

	auto products = toml["products"].AsArray().value().get();

	if (! products.empty())
	{
		

		std::cout << "  size: " << products.size() << '\n';

		for (size_t i = 0; i < products.size(); ++i)
		{
			std::cout << "\nproducts[" << i << "]:\n";

			std::cout << "  object: "
					  << products[i].IsObject() << '\n';

			std::cout << "  name:\n";
			std::cout << "    string: "
					  << products[i]["name"].AsString().value().get() << '\n';

			auto productName = products[i]["name"].AsString();

			if (productName)
				std::cout << "    value: "
						  << productName.value().get() << '\n';
			else
				std::cout << "    ERROR: "
						  << static_cast<int>(productName.error().errorType) << '\n';

			std::cout << "  sku:\n";
			std::cout << "    number: "
					  << products[i]["sku"].IsNumber() << '\n';

			auto productSku = products[i]["sku"].AsNumber();

			if (productSku)
				std::cout << "    value: "
						  << productSku.value() << '\n';
			else
				std::cout << "    ERROR: "
						  << static_cast<int>(productSku.error().errorType) << '\n';

			std::cout << "  price:\n";
			std::cout << "    number: "
					  << products[i]["price"].IsNumber() << '\n';

			auto productPrice = products[i]["price"].AsNumber();

			if (productPrice)
				std::cout << "    value: "
						  << productPrice.value() << '\n';
			else
				std::cout << "    ERROR: "
						  << static_cast<int>(productPrice.error().errorType) << '\n';
		}
	}

	return 0;
}
