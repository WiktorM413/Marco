#include "marco/toml/Toml.h"
#include "marco/toml/TomlReader.h"
#include <iostream>
#include <string>

int main()
{
	std::string tomlString = R"(
		# This is a comment in a TOML file
		# TOML files are case-sensitive and rely on key = value pairs
		
		title = "TOML Example Configuration"
		
		# ---------------------------------------------------------
		# Basic Data Types (Root-level pairs)
		# ---------------------------------------------------------
		[app]
		name = "MyApp"
		version = "1.2.3"
		description = "A sleek, minimal web service."
		production_ready = true   # Boolean values must be lowercase
		max_connections = 5000     # Integers
		pi_value = 3.14159        # Floats
		
		# ---------------------------------------------------------
		# Strings in Depth
		# ---------------------------------------------------------
		[strings]
		# Basic string with escape sequences
		escaped = "Line 1\nLine 2\tTabbed"
		
		# Literal string (no escaping, great for Windows paths or Regex)
		literal_path = 'C:\Users\admin\Documents'
		
		# Multi-line basic string (preserves formatting and newlines)
		multi_line_basic = """
		The quick brown fox
		jumps over the lazy dog.
		"""
		
		# Multi-line literal string (no escaping whatsoever)
		multi_line_literal = '''
		I [dw]on't need \d{2} apples
		Regex patterns work perfectly here.
		'''
		
		# ---------------------------------------------------------
		# Numbers and Date/Time
		# ---------------------------------------------------------
		[numbers_and_dates]
		# Underscores are allowed to improve readability
		large_number = 1_000_000
		binary_mode = 0b1010
		hex_color = 0xDEADBEEF
		
		# RFC 3339 Date and Time formats
		created_at = 2026-09-01T17:15:00Z
		local_date = 2026-09-01
		
		# ---------------------------------------------------------
		# Arrays and Inline Tables
		# ---------------------------------------------------------
		[collections]
		# Arrays can be single-line or multi-line
		tags = [ "production", "web", "api" ]
		ports = [
  8080,
  8081,
  8082
		]
		
		# Inline tables are dynamic dictionaries (compact definitions)
		coordinates = { x = 12, y = -5, label = "Target Location" }
		
		# ---------------------------------------------------------
		# Nested Tables (Dictionaries/Hash Maps)
		# ---------------------------------------------------------
		[database]
		server = "192.168.1.100"
		ports = [ 5432, 5433 ]
		connection_max = 5000
		enabled = true
		
		# This defines a nested table: database.credentials
		[database.credentials]
		username = "db_admin"
		password = "super_secure_password_123"
		
		# ---------------------------------------------------------
		# Array of Tables (Great for lists of structured objects)
		# ---------------------------------------------------------
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

	if (! rd.IsValid())
	{
		std::cout << "Error code: " << (int)rd.Error().errorType << " at " << rd.Error().index;
	}

	

	return 0;
}