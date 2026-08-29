#include "marco/json/Json.h"
#include "marco/json/JsonValue.h"
#include "marco/json/JsonWriter.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>

TEST(JsonWriterObject, EmptyObjectProducesEmptyBraces)
{
	Marco::Json json{};
	Marco::JsonWriter writer;

	EXPECT_EQ(writer.Write(json), "{}");
}

TEST(JsonWriterObject, SinglePropertyIsIndentedOneLevel)
{
	Marco::Json json{{"key", "value"}};
	Marco::JsonWriter writer;

	EXPECT_EQ(writer.Write(json), "{\n    \"key\": \"value\"\n}");
}

TEST(JsonWriterObject, NestedObjectIndentsOneLevelDeeper)
{
	Marco::JsonObject inner{{"inner", "value"}};
	Marco::Json       json{{"outer", inner}};
	Marco::JsonWriter writer;

	EXPECT_EQ(writer.Write(json), "{\n    \"outer\": {\n        \"inner\": \"value\"\n    }\n}");
}

TEST(JsonWriterObject, MultiplePropertiesAppearOnSeparateLines)
{
	Marco::Json json{{"a", 1}, {"b", 2}};
	Marco::JsonWriter writer;

	EXPECT_EQ(writer.Write(json), "{\n    \"a\": 1,\n    \"b\": 2\n}");
}

TEST(JsonWriterArray, EmptyArrayProducesEmptyBrackets)
{
	Marco::Json json{{"arr", Marco::JsonArray{}}};
	Marco::JsonWriter writer;

	EXPECT_EQ(writer.Write(json), "{\n    \"arr\": []\n}");
}

TEST(JsonWriterArray, ArrayOfScalarsIndentsOneLevelDeeperThanObject)
{
	Marco::JsonArray arr{1, 2, 3};
	Marco::Json      json{{"arr", arr}};
	Marco::JsonWriter writer;

	EXPECT_EQ(writer.Write(json), "{\n    \"arr\": [\n        1,\n        2,\n        3\n    ]\n}");
}

TEST(JsonWriterArray, ArrayOfObjectsIndentsNestedPropertiesTwoLevelsDeeper)
{
	Marco::JsonObject user{{"name", "John"}, {"age", 21}};
	Marco::JsonArray  users{user};
	Marco::Json       json{{"users", users}};
	Marco::JsonWriter writer;

	EXPECT_EQ(writer.Write(json), "{\n    \"users\": [\n        {\n            \"name\": \"John\",\n            \"age\": 21\n        }\n    ]\n}");
}

TEST(JsonWriterArray, NestedArrayIndentsOneLevelDeeper)
{
	Marco::JsonArray  inner{1, 2};
	Marco::JsonArray  outer{inner};
	Marco::Json       json{{"arr", outer}};
	Marco::JsonWriter writer;

	EXPECT_EQ(writer.Write(json), "{\n    \"arr\": [\n        [\n            1,\n            2\n        ]\n    ]\n}");
}

TEST(JsonWriterScalar, WriterIntegerNumber)
{
	Marco::Json json{{"key", 42}};
	Marco::JsonWriter writer;

	EXPECT_EQ(writer.Write(json), "{\n    \"key\": 42\n}");
}

TEST(JsonWriterScalar, WritesDecimalNumber)
{
	Marco::Json json{{"key", 3.14}};
	Marco::JsonWriter writer;

	EXPECT_EQ(writer.Write(json), "{\n    \"key\": 3.14\n}");
}

TEST(JsonWriterScalar, WritesNegativeNumber)
{
	Marco::Json json{{"key", -42}};
	Marco::JsonWriter writer;

	EXPECT_EQ(writer.Write(json), "{\n    \"key\": -42\n}");
}

TEST(JsonWriterScalar, WritesTrueBool)
{
	Marco::Json json{{"key", Marco::JsonValue(true)}};
	Marco::JsonWriter writer;
 
	EXPECT_EQ(writer.Write(json), "{\n    \"key\": true\n}");
}


TEST(JsonWriterScalar, WritesFalseBool)
{
	Marco::Json json{{"key", Marco::JsonValue(false)}};
	Marco::JsonWriter writer;
 
	EXPECT_EQ(writer.Write(json), "{\n    \"key\": false\n}");
}

TEST(JsonWriterScalar, WritesNull)
{
	Marco::Json json{{"key", Marco::JsonValue(nullptr)}};
	Marco::JsonWriter writer;

	EXPECT_EQ(writer.Write(json), "{\n    \"key\": null\n}");
}

TEST(JsonWriterString, EscapesQuoteBackslashNewlineAndTab)
{
	std::string value = "a\"b\\c\nd\te";
	Marco::Json json{{"key", Marco::JsonValue(value)}};
	Marco::JsonWriter writer;
 
	EXPECT_EQ(writer.Write(json), "{\n    \"key\": \"a\\\"b\\\\c\\nd\\te\"\n}");
}
 
TEST(JsonWriterString, EscapesControlCharacterAsUnicodeSequence)
{
	std::string value = "a\x01" "b";
	Marco::Json json{{"key", Marco::JsonValue(value)}};
	Marco::JsonWriter writer;
 
	EXPECT_EQ(writer.Write(json), "{\n    \"key\": \"a\\u0001b\"\n}");
}

TEST(JsonWriterNumber, ThrowsOnNaN)
{
	double nan = std::numeric_limits<double>::quiet_NaN();
	Marco::Json json{{"key", Marco::JsonValue(nan)}};
	Marco::JsonWriter writer;
 
	EXPECT_THROW(writer.Write(json), std::runtime_error);
}
 
TEST(JsonWriterNumber, ThrowsOnPositiveInfinity)
{
	double inf = std::numeric_limits<double>::infinity();
	Marco::Json json{{"key", Marco::JsonValue(inf)}};
	Marco::JsonWriter writer;
 
	EXPECT_THROW(writer.Write(json), std::runtime_error);
}
 
TEST(JsonWriterNumber, ThrowsOnNegativeInfinity)
{
	double negInf = -std::numeric_limits<double>::infinity();
	Marco::Json json{{"key", Marco::JsonValue(negInf)}};
	Marco::JsonWriter writer;
 
	EXPECT_THROW(writer.Write(json), std::runtime_error);
}

TEST(JsonWriterFile, WriteToProducesFileMatchingWriteOutput)
{
	Marco::Json json{{"key", Marco::JsonValue("value")}};
	Marco::JsonWriter writer;
	std::string expected = writer.Write(json);
 
	std::string path = "/tmp/marco_json_writer_test.json";
	writer.WriteTo(json, path);
 
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
 
	EXPECT_EQ(buffer.str(), expected);
 
	std::filesystem::remove(path);
}
