#include "marco/JsonError.h"
#include "marco/JsonValue.h"
#include <gtest/gtest.h>

TEST(JsonValueConstruct, DefaultIsNull)
{
	Marco::JsonValue value;
	EXPECT_TRUE(value.IsNull());
}

TEST(JsonValueConstruct, NullptrIsNull)
{
	Marco::JsonValue value(nullptr);
	EXPECT_TRUE(value.IsNull());
}

TEST(JsonValueConstruct, BoolIsBool)
{
	Marco::JsonValue value(true);
	EXPECT_TRUE(value.IsBool());
	EXPECT_FALSE(value.IsNumber());
}

TEST(JsonValueConstruct, DoubleIsNumber)
{
	Marco::JsonValue value(3.14);
	EXPECT_TRUE(value.IsNumber());
}

TEST(JsonValueConstruct, IntIsNumber)
{
	Marco::JsonValue value(42);
	EXPECT_TRUE(value.IsNumber());
	ASSERT_TRUE(value.AsNumber().has_value());
	EXPECT_DOUBLE_EQ(value.AsNumber().value(), 42.0);
}

TEST(JsonValueConstruct, StdStringIsString)
{
	Marco::JsonValue value(std::string("hello"));
	EXPECT_TRUE(value.IsString());
}

TEST(JsonValueConstruct, CStringIsString)
{
	Marco::JsonValue value("hello");
	EXPECT_TRUE(value.IsString());
}

TEST(JsonValueConstruct, ArrayIsArray)
{
	Marco::JsonArray arr{1, 2, 3};
	Marco::JsonValue value(arr);
	EXPECT_TRUE(value.IsArray());
}

TEST(JsonValueConstruct, ObjectIsObject)
{
	Marco::JsonObject obj{{"key", Marco::JsonValue("value")}};
	Marco::JsonValue value(obj);
	EXPECT_TRUE(value.IsObject());
}

TEST(JsonValueIs, OnlyOneTypeTrueAtTime)
{
	Marco::JsonValue value;
	EXPECT_TRUE(value.IsNull());
	EXPECT_FALSE(value.IsBool());
	EXPECT_FALSE(value.IsNumber());
	EXPECT_FALSE(value.IsString());
	EXPECT_FALSE(value.IsArray());
	EXPECT_FALSE(value.IsObject());
}

TEST(JsonValueAs, AsNullSuccess)
{
	Marco::JsonValue value(nullptr);
	auto result = value.AsNull();
	ASSERT_TRUE(result.has_value());
}

TEST(JsonValueAs, AsNullWrongType)
{
	Marco::JsonValue value(true);
	auto result = value.AsNull();
	ASSERT_FALSE(result.has_value());
	EXPECT_EQ(result.error().errorType, Marco::JsonErrorType::WrongType);
}

TEST(JsonValueAs, AsBoolSuccess)
{
	Marco::JsonValue value(false);
	auto result = value.AsBool();
	ASSERT_TRUE(result.has_value());
	EXPECT_FALSE(result.value());
}

TEST(JsonValueAs, AsBoolWrongType)
{
	Marco::JsonValue value(1);
	auto result = value.AsBool();
	ASSERT_FALSE(result.has_value());
	EXPECT_EQ(result.error().errorType, Marco::JsonErrorType::WrongType);
}

TEST(JsonValueAs, AsNumberSuccess)
{
	Marco::JsonValue value(4.2);
	auto result = value.AsNumber();
	ASSERT_TRUE(result.has_value());
	EXPECT_DOUBLE_EQ(result.value(), 4.2);
}

TEST(JsonValueAs, AsNumberWrongType)
{
	Marco::JsonValue value("not a number");
	auto result = value.AsNumber();
	ASSERT_FALSE(result.has_value());
	EXPECT_EQ(result.error().errorType, Marco::JsonErrorType::WrongType);
}

TEST(JsonValueAs, AsStringSuccess)
{
	Marco::JsonValue value("hello");
	auto result = value.AsString();
	ASSERT_TRUE(result.has_value());
	EXPECT_EQ(result.value(), "hello");
}

TEST(JsonValueAs, AsStringWrongType)
{
	Marco::JsonValue value(nullptr);
	auto result = value.AsString();
	ASSERT_FALSE(result.has_value());
	EXPECT_EQ(result.error().errorType, Marco::JsonErrorType::WrongType);
}

TEST(JsonValueAs, AsArraySuccess)
{
	Marco::JsonArray arr{1, 2, 3};
	Marco::JsonValue value(arr);
	auto result = value.AsArray();
	ASSERT_TRUE(result.has_value());
	EXPECT_EQ(result.value().get().size(), 3);
}

TEST(JsonValueAs, AsArrayWrongType)
{
	Marco::JsonValue value(nullptr);
	auto result = value.AsArray();
	ASSERT_FALSE(result.has_value());
	EXPECT_EQ(result.error().errorType, Marco::JsonErrorType::WrongType);
}

TEST(JsonValueAs, AsObjectSuccess)
{
	Marco::JsonObject obj{{"key", Marco::JsonValue("value")}};
	Marco::JsonValue value(obj);
	auto result  = value.AsObject();
	ASSERT_TRUE(result.has_value());
	EXPECT_EQ(result.value().get().size(), 1);
}

TEST(JsonValueAs, AsObjectWrongType)
{
	Marco::JsonValue value(nullptr);
	auto result = value.AsObject();
	ASSERT_FALSE(result.has_value());
	EXPECT_EQ(result.error().errorType, Marco::JsonErrorType::WrongType);
}

TEST(JsonValuePushBack, AppendToEmptyValue)
{
	Marco::JsonValue value;
	value.PushBack(1);
	value.PushBack(2);
	value.PushBack(3);

	EXPECT_TRUE(value.IsArray());
	auto arr = value.AsArray();
	ASSERT_TRUE(arr.has_value());
	EXPECT_EQ(arr.value().get().size(), 3);
}

TEST(JsonValuePushBack, AppendToExistingArray)
{
	Marco::JsonArray initial{1};
	Marco::JsonValue value(initial);
	value.PushBack(2);
	value.PushBack(3);

	auto arr = value.AsArray();
	ASSERT_TRUE(arr.has_value());
	EXPECT_EQ(arr.value().get().size(), 3);
}

TEST(JsonValuePushBack, OverwritesNonArrayValue)
{
	Marco::JsonValue value(true);
	value.PushBack(1);

	EXPECT_TRUE(value.IsArray());
	auto arr = value.AsArray();
	ASSERT_TRUE(arr.has_value());
	EXPECT_EQ(arr.value().get().size(), 1);
}

TEST(JsonValuePushBack, ReturnsReferenceToAppendedElement)
{
	Marco::JsonValue value;
	Marco::JsonValue& ref = value.PushBack(42);

	ASSERT_TRUE(ref.IsNumber());
	EXPECT_DOUBLE_EQ(ref.AsNumber().value(), 42.0);
}

TEST(JsonValueSubscriptString, AutoVivifiesEmptyValueIntoObject)
{
	Marco::JsonValue value;
	value["key"] = "value";

	EXPECT_TRUE(value.IsObject());
	auto result = value.AsString();
	EXPECT_FALSE(result.has_value());
}

TEST(JsonValueSubscriptString, OverwritesNonObjectValue)
{
	Marco::JsonValue value(1);
	value["key"] = true;

	EXPECT_TRUE(value.IsObject());
	ASSERT_TRUE(value.AsObject().has_value());
	EXPECT_EQ(value.AsObject().value().get().size(), 1);
}

TEST(JsonValueSubscriptString, MutableAccessReadsBackWrittenValue)
{
	Marco::JsonValue value;
	value["key"] = "value";

	const Marco::JsonValue& constView = value;
	auto result = constView["key"];

	ASSERT_TRUE(result.has_value());
	EXPECT_EQ(result.value().get().AsString().value(), "value");
}

TEST(JsonValueSubscriptString, ConstAccessMissingKeyFails)
{
	Marco::JsonObject obj{{"key", "value"}};
	const Marco::JsonValue value(obj);

	auto result = value["missing"];
	ASSERT_FALSE(result.has_value());
	EXPECT_EQ(result.error().errorType, Marco::JsonErrorType::InvalidFormat);
}

TEST(JsonValueSubscriptString, ConstAccessOnNonBojectFails)
{
	const Marco::JsonValue value(1.0);

	auto result = value["key"];
	ASSERT_FALSE(result.has_value());
	EXPECT_EQ(result.error().errorType, Marco::JsonErrorType::InvalidFormat);
}

TEST(JsonValueSubscriptIndex, AutoVivifiesEmptyValueIntoArray)
{
	Marco::JsonValue value;
	value[0] = 1;

	EXPECT_TRUE(value.IsArray());
	ASSERT_TRUE(value.AsArray().has_value());
	EXPECT_EQ(value.AsArray().value().get().size(), 1);
}

TEST(JsonValueSubscriptIndex, OverwriteNonArrayValue)
{
	Marco::JsonValue value(true);
	value[0] = 1;

	EXPECT_TRUE(value.IsArray());
}

TEST(JsonValueSubscriptIndex, MutableAccessReadsBackWrittenValue)
{
	Marco::JsonValue value;
	value.PushBack(10);
	value.PushBack(20);
	value[1] = 99;

	const Marco::JsonValue& constView = value;
	auto result = constView[1];
	ASSERT_TRUE(result.has_value());
	EXPECT_DOUBLE_EQ(result.value().get().AsNumber().value(), 99.0);
}

TEST(JsonValueSubscriptIndex, ConstAccessOutOfBoundsFails)
{
	Marco::JsonArray arr{1};
	const Marco::JsonValue value(arr);

	auto result = value[5];
	ASSERT_FALSE(result.has_value());
	EXPECT_EQ(result.error().errorType, Marco::JsonErrorType::InvalidFormat);
}

TEST(JsonValueSubscriptIndex, ConstAccessOnNonArrayFails)
{
	const Marco::JsonValue value(1);

	auto result = value[0];
	ASSERT_FALSE(result.has_value());
	EXPECT_EQ(result.error().errorType, Marco::JsonErrorType::InvalidFormat);
}

TEST(JsonValueNested, MutableSubscriptChainingBuildNestedStructure)
{
	Marco::JsonValue value;
	value["users"][0]["name"] = "John";
	value["users"][0]["age"]  = 21;

	EXPECT_TRUE(value.IsObject());
	
	Marco::JsonValue& users = value["users"];
	EXPECT_TRUE(users.IsArray());
 
	Marco::JsonValue& first = users[0];
	EXPECT_TRUE(first.IsObject());
	EXPECT_EQ(first["name"].AsString().value(), "John");
	EXPECT_DOUBLE_EQ(first["age"].AsNumber().value(), 21);
}

TEST(JsonValueNested, ConstSubscriptChainingReadsNestedStructure)
{
	Marco::JsonValue value;
	value["users"][0]["name"] = "John";
	value["users"][0]["age"]  = 21;
 
	const Marco::JsonValue& constV = value;
	auto users = constV["users"];
	ASSERT_TRUE(users.has_value());
	EXPECT_TRUE(users.value().get().IsArray());
 
	auto first = users.value().get()[static_cast<size_t>(0)];
	ASSERT_TRUE(first.has_value());
 
	auto name = first.value().get()["name"];
	ASSERT_TRUE(name.has_value());
	EXPECT_EQ(name.value().get().AsString().value(), "John");
 
	auto age = first.value().get()["age"];
	ASSERT_TRUE(age.has_value());
	EXPECT_DOUBLE_EQ(age.value().get().AsNumber().value(), 21);
}


TEST(JsonValueNested, ConstAsObjectReferenceStaysValidWhileParentAlive)
{
	Marco::JsonValue value;
	value["key"] = "value";

	auto objResult = value.AsObject();
	ASSERT_TRUE(objResult.has_value());
	const Marco::JsonObject& obj = objResult.value().get();
	EXPECT_EQ(obj.at("key").AsString().value(), "value");
}