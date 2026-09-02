#include "marco/json/JsonError.h"
#include "marco/json/JsonReader.h"
#include "marco/json/JsonValue.h"
#include <gtest/gtest.h>

TEST(JsonReaderTest, ParseEmptyObject)
{
	Marco::JsonReader reader;
	reader.Parse("{}");

	EXPECT_TRUE(reader.IsValid());
}

TEST(JsonReaderTest, FailsOnEmptyString)
{
	Marco::JsonReader reader;
	reader.Parse("");

	EXPECT_FALSE(reader.IsValid());
	EXPECT_EQ(reader.Error().errorType, Marco::JsonErrorType::InvalidFormat);
}

TEST(JsonReaderTest, FailsOnWhitespaceOnlyString)
{
	Marco::JsonReader reader;
	reader.Parse("	\t\n	");

	EXPECT_FALSE(reader.IsValid());
}

TEST(JsonReaderTest, FailsWhenNotStartingWithBrace)
{
	Marco::JsonReader reader;
	reader.Parse(R"("key": 1})");

	EXPECT_FALSE(reader.IsValid());
	EXPECT_EQ(reader.Error().errorType, Marco::JsonErrorType::InvalidFormat);
}

TEST(JsonReaderTest, FailsOnMissingClosingBrace)
{
	Marco::JsonReader reader;
	reader.Parse(R"({"key": 1)");

	EXPECT_FALSE(reader.IsValid());
}

TEST(JsonReaderTest, IgnoresLeadingAndTralingWhitespace)
{
	Marco::JsonReader reader;
	reader.Parse("	\n{\"key\":1}\t		");

	EXPECT_TRUE(reader.IsValid());
}

TEST(JsonReaderTest, FailsOnTrailingCommaInObject)
{
	Marco::JsonReader reader;
	reader.Parse(R"({"key":1,})");

	EXPECT_FALSE(reader.IsValid());
}

TEST(JsonReaderTest, IsValidFalseBeforeParseIsCalled)
{
	Marco::JsonReader reader;

	EXPECT_FALSE(reader.IsValid());
}

TEST(JsonReaderTest, IsValidUpdatesAcrossMultipleParseCalls)
{
	Marco::JsonReader reader;

	reader.Parse("not json");
	EXPECT_FALSE(reader.IsValid());

	reader.Parse("{}");
	EXPECT_TRUE(reader.IsValid());

	reader.Parse("not json");
	EXPECT_FALSE(reader.IsValid());
}

TEST(JsonReaderTest, ParsesStringValue)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key": "string"})");

	ASSERT_TRUE(reader.IsValid());

	auto s = value["key"].AsString();
	if (s)
	{
		EXPECT_EQ(s.value().get(), "string");
	}
	else
	{
		FAIL() << "AsString failed with error code: " << (int)s.error().errorType;
	}
}

TEST(JsonReaderTest, ParsesIntegerValue)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key": 42})");

	ASSERT_TRUE(reader.IsValid());

	auto s = value["key"].AsNumber();
	if (s)
	{
		EXPECT_EQ(s.value(), 42);
	}
	else
	{
		FAIL() << "AsString failed with error code: " << (int)s.error().errorType;
	}
}

TEST(JsonReaderTest, ParsesNegativeIntegerValue)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key": -42})");

	ASSERT_TRUE(reader.IsValid());

	auto s = value["key"].AsNumber();
	if (s)
	{
		EXPECT_EQ(s.value(), -42);
	}
	else
	{
		FAIL() << "AsString failed with error code: " << (int)s.error().errorType;
	}
}

TEST(JsonReaderTest, ParsesFloatingPointValue)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key": 3.14})");

	ASSERT_TRUE(reader.IsValid());

	auto s = value["key"].AsNumber();
	if (s)
	{
		EXPECT_EQ(s.value(), 3.14);
	}
	else
	{
		FAIL() << "AsString failed with error code: " << (int)s.error().errorType;
	}
}

TEST(JsonReaderTest, ParsesTrueValue)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key": true})");

	ASSERT_TRUE(reader.IsValid());

	auto s = value["key"].AsBool();
	if (s)
	{
		EXPECT_EQ(s.value(), true);
	}
	else
	{
		FAIL() << "AsString failed with error code: " << (int)s.error().errorType;
	}
}

TEST(JsonReaderTest, ParsesFalseValue)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key": false})");

	ASSERT_TRUE(reader.IsValid());

	auto s = value["key"].AsBool();
	if (s)
	{
		EXPECT_EQ(s.value(), false);
	}
	else
	{
		FAIL() << "AsString failed with error code: " << (int)s.error().errorType;
	}
}

TEST(JsonReaderTest, ParsesNullValue)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key": null})");

	ASSERT_TRUE(reader.IsValid());

	auto s = value["key"].AsNull();
	if (s)
	{
		EXPECT_EQ(s.value(), nullptr);
	}
	else
	{
		FAIL() << "AsString failed with error code: " << (int)s.error().errorType;
	}
}

TEST(JsonReaderTest, FailsOnStringContainingNewLine)
{
	Marco::JsonReader reader;
	reader.Parse("{\"key\":\"line1\nline2\"}");

	EXPECT_FALSE(reader.IsValid());
}

TEST(JsonReaderTest, FailsOnInvalidBooleanToken)
{
	Marco::JsonReader reader;
	reader.Parse(R"({"key": tru})");

	EXPECT_FALSE(reader.IsValid());
}

TEST(JsonReaderTest, FailsOnLoneMinusSignNumber)
{
	Marco::JsonReader reader;
	reader.Parse(R"({"key":-})");

	EXPECT_FALSE(reader.IsValid());
	EXPECT_EQ(reader.Error().errorType, Marco::JsonErrorType::NumberParseError);
}

TEST(JsonReaderTest, FailsOnNumberOutOfRange)
{
	Marco::JsonReader reader;
	reader.Parse(R"({"key":1e400})");

	EXPECT_FALSE(reader.IsValid());
	EXPECT_EQ(reader.Error().errorType, Marco::JsonErrorType::NumberOutOfRange);
}

TEST(JsonReaderTest, ParsesNestedObject)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"a": {"b": 1}})");

	ASSERT_TRUE(reader.IsValid());

	auto n = value["a"]["b"].AsNumber();

	if (n)
	{
		EXPECT_EQ(n.value(), 1);
	}
	else
	{
		FAIL() << "AsNumber failed with error code: " << (int)n.error().errorType;
	}	
}

TEST(JsonReaderTest, ParsesEmptyArray)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":[]})");

	ASSERT_TRUE(reader.IsValid());

	auto arr = value["key"].AsArray();

	if (arr)
	{
		EXPECT_EQ(arr.value().get().size(), 0);
	}
	else
	{
		FAIL() << "AsArray failed with error code: " << (int)arr.error().errorType;
	}
}

TEST(JsonReaderTest, ParsesArrayOfNumbers)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":[1,2,3]})");

	ASSERT_TRUE(reader.IsValid());

	auto arr = value["key"].AsArray();

	if (arr)
	{
		ASSERT_EQ(arr.value().get().size(), 3);
		EXPECT_EQ(value["key"][0].AsNumber().value(), 1);
		EXPECT_EQ(value["key"][1].AsNumber().value(), 2);
		EXPECT_EQ(value["key"][2].AsNumber().value(), 3);
	}
	else
	{
		FAIL() << "AsArray failed with error code: " << (int)arr.error().errorType;
	}
}

TEST(JsonReaderTest, ParsesArrayWithMixedTypes)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":[1,"two",true,null]})");

	ASSERT_TRUE(reader.IsValid());

	auto arr = value["key"].AsArray();

	if (arr)
	{
		ASSERT_EQ(arr.value().get().size(), 4);
		EXPECT_EQ(value["key"][0].AsNumber().value(), 1);
		EXPECT_EQ(value["key"][1].AsString().value().get(), "two");
		EXPECT_EQ(value["key"][2].AsBool().value(), true);
		EXPECT_EQ(value["key"][3].AsNull().value(), nullptr);
	}
	else
	{
		FAIL() << "AsArray failed with error code: " << (int)arr.error().errorType;
	}
}

TEST(JsonReaderTest, ParsesArrayOfObjects)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":[{"x":1},{"y":2}]})");

	ASSERT_TRUE(reader.IsValid());

	auto arr = value["key"].AsArray();

	if (arr)
	{
		ASSERT_EQ(arr.value().get().size(), 2);
		EXPECT_EQ(value["key"][0]["x"].AsNumber().value(), 1);
		EXPECT_EQ(value["key"][1]["y"].AsNumber().value(), 2);
	}
	else
	{
		FAIL() << "AsArray failed with error code: " << (int)arr.error().errorType;
	}
}

TEST(JsonReaderTest, ParsesNestedArrays)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":[[1,2],[3,4]]})");

	ASSERT_TRUE(reader.IsValid());

	auto arr = value["key"].AsArray();

	if (arr)
	{
		ASSERT_EQ(arr.value().get().size(), 2);
		EXPECT_EQ(value["key"][0][1].AsNumber().value(), 2);
		EXPECT_EQ(value["key"][1][1].AsNumber().value(), 4);
	}
	else
	{
		FAIL() << "AsArray failed with error code: " << (int)arr.error().errorType;
	}
}

TEST(JsonReaderTest, FailsOnTrailingCommaInArray)
{
	Marco::JsonReader reader;
	reader.Parse(R"({"key":[1,2,]})");

	EXPECT_FALSE(reader.IsValid());
}

TEST(JsonReaderTest, ParsesObjectWithMultipleKeys)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"a":1,"b":2,"c":3})");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["a"].AsNumber().value(), 1);
	EXPECT_EQ(value["b"].AsNumber().value(), 2);
	EXPECT_EQ(value["c"].AsNumber().value(), 3);
}

TEST(JsonReaderTest, ToleratesWhitespaceAroundTokensAndPunctuation)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({ "a" : 1 , "b" : 2 })");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["a"].AsNumber().value(), 1);
	EXPECT_EQ(value["b"].AsNumber().value(), 2);
}

TEST(JsonReaderTest, OverwritesValueOnDuplicateKey)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"a":1,"a":2})");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["a"].AsNumber().value(), 2);
}

TEST(JsonReaderTest, ParsesFromIstream)
{
	Marco::JsonReader reader;

	std::istringstream stream(R"({"key":1})");
	Marco::JsonValue    value = reader.Parse(stream);

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsNumber().value(), 1);
}

TEST(JsonReaderTest, FailsOnCommaImmediatelyAfterOpenBrace)
{
	Marco::JsonReader reader;
	reader.Parse(R"({,"a":1})");

	EXPECT_FALSE(reader.IsValid());
}

TEST(JsonReaderTest, FailsWhenValueErrorIsNotIgnored)
{
	Marco::JsonReader reader;
	reader.Parse(R"({"a":tru,"b":1})");

	EXPECT_FALSE(reader.IsValid());
}

TEST(JsonReaderTest, ParsesScientificNotationPositiveExponent)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":5E+2})");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsNumber().value(), 500);
}

TEST(JsonReaderTest, ParsesScientificNotationNegativeExponent)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":1.23e-4})");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_DOUBLE_EQ(value["key"].AsNumber().value(), 1.23e-4);
}

TEST(JsonReaderTest, ParsesScientificNotationLowercaseE)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":2e3})");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsNumber().value(), 2000);
}

TEST(JsonReaderTest, ParsesScientificNotationUppercaseE)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":2E3})");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsNumber().value(), 2000);
}

TEST(JsonReaderTest, ParsesScientificNotationInArray)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":[1e1,1e2,1e3]})");

	ASSERT_TRUE(reader.IsValid());

	auto arr = value["key"].AsArray();

	if (arr)
	{
		ASSERT_EQ(arr.value().get().size(), 3);
		EXPECT_EQ(value["key"][0].AsNumber().value(), 10);
		EXPECT_EQ(value["key"][1].AsNumber().value(), 100);
		EXPECT_EQ(value["key"][2].AsNumber().value(), 1000);
	}
	else
	{
		FAIL() << "AsArray failed with error code: " << (int)arr.error().errorType;
	}
}

TEST(JsonReaderTest, ParsesDeeplyNestedObject)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"a":{"b":{"c":{"d":{"e":1}}}}})");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["a"]["b"]["c"]["d"]["e"].AsNumber().value(), 1);
}

TEST(JsonReaderTest, ParsesDeeplyNestedArray)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":[[[[[1]]]]]})");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"][0][0][0][0][0].AsNumber().value(), 1);
}

TEST(JsonReaderTest, ParsesDeeplyNestedMixedStructure)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"a":[{"b":[{"c":[{"d":1}]}]}]})");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["a"][0]["b"][0]["c"][0]["d"].AsNumber().value(), 1);
}

TEST(JsonReaderTest, FailsGracefullyOnUnclosedDeeplyNestedStructure)
{
	Marco::JsonReader reader;
	reader.Parse(R"({"a":{"b":{"c":{"d":{"e":1)");

	EXPECT_FALSE(reader.IsValid());
}

TEST(JsonReaderTest, ParsesStringWithEscapedQuote)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":"say \"hi\""})"); 

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsString().value().get(), R"(say "hi")");
}

TEST(JsonReaderTest, ParsesStringWithEscapedBackslash)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":"C:\\path"})");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsString().value().get(), R"(C:\path)");
}

TEST(JsonReaderTest, ParsesStringWithEscapedForwardSlash)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":"a\/b"})");

	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsString().value().get(), "a/b");
}

TEST(JsonReaderTest, ParsesStringWithEscapedNewline)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":"line1\nline2"})");
	
	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsString().value().get(), "line1\nline2");
}

TEST(JsonReaderTest, ParsesStringWithEscapedTab)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":"a\tb"})");
	
	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsString().value().get(), "a\tb");
}

TEST(JsonReaderTest, ParsesStringWithEscapedCarriageReturn)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":"a\rb"})");
	
	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsString().value().get(), "a\rb");
}

TEST(JsonReaderTest, ParsesStringWithEscapedBackspace)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":"a\bb"})");
	
	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsString().value().get(), "a\bb");
}

TEST(JsonReaderTest, ParsesStringWithEscapedFormFeed)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":"a\fb"})");
	
	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsString().value().get(), "a\fb");
}

TEST(JsonReaderTest, ParsesStringWithMultipleConsecutiveEscapes)
{
	Marco::JsonReader reader;
	Marco::JsonValue  value = reader.Parse(R"({"key":"\n\t\\\""})");
	
	ASSERT_TRUE(reader.IsValid());
	EXPECT_EQ(value["key"].AsString().value().get(), "\n\t\\\"");
}