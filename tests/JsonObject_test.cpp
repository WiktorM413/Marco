#include "marco/JsonError.h"
#include "marco/JsonObject.h"
#include "marco/JsonValue.h"
#include <gtest/gtest.h>
#include <utility>

TEST(JsonObjectConstruction, DefaultConstructedObjectIsEmpty)
{
	Marco::JsonObject obj;

	EXPECT_EQ(obj.size(), 0);
}

TEST(JsonObjectConstruction, EmptyInitializerListProducesEmptyObject)
{
	Marco::JsonObject obj{};

	EXPECT_EQ(obj.size(), 0);
}

TEST(JsonObjectConstruction, InitializerListPopulatesEntries)
{
	Marco::JsonObject obj{{"a", 1}, {"b", 2}};

	EXPECT_EQ(obj.size(), 2);
}

TEST(JsonObjectConstruction, InitializerListWithDuplicateKeyKeepsLastValue)
{
	Marco::JsonObject obj{{"a", 1}, {"a", 2}};

	EXPECT_EQ(obj.size(), 1);
	EXPECT_EQ(obj.at("a").AsNumber().value(), 2);
}

TEST(JsonObjectConstruction, InitializerListPreservesInsertionOrder)
{
	Marco::JsonObject obj{{"c", 1}, {"b", 2}, {"a", 3}};

	auto it = obj.begin();
	EXPECT_EQ(it->first, "c");
	it++;
	EXPECT_EQ(it->first, "b");
	it++;
	EXPECT_EQ(it->first, "a");
}

TEST(JsonObjectCopy, CopyConstructorDuplicatesEntries)
{
	Marco::JsonObject original{{"a", 1}, {"b", 2}};
	Marco::JsonObject copy(original);

	EXPECT_EQ(copy.size(), 2);
	EXPECT_EQ(copy.at("a").AsNumber().value(), 1);
	EXPECT_EQ(copy.at("b").AsNumber().value(), 2);
}

TEST(JsonObjectCopy, CopyConstructorProducesIndependentObject)
{
	Marco::JsonObject original{{"a", 1}};
	Marco::JsonObject copy(original);

	copy["a"] = "changed";

	EXPECT_EQ(original.at("a").AsNumber().value(), 1);
	EXPECT_EQ(copy.at("a").AsString().value().get(), "changed");
}

TEST(JsonObjectCopy, CopyConstructorAllowsFindingEntriesByKey)
{
	Marco::JsonObject original{{"a", 1}};
	Marco::JsonObject copy(original);

	EXPECT_NE(copy.find("a"), copy.end());
}

TEST(JsonObjectCopy, CopyAssignmentDuplicatesEntries)
{
	Marco::JsonObject original{{"a", 1}, {"b", 2}};
	Marco::JsonObject target{{"c", 3}};

	target = original;

	EXPECT_EQ(target.size(), 2);
	EXPECT_EQ(target.at("a").AsNumber().value(), 1);
	EXPECT_EQ(target.find("c"), target.end());
}

TEST(JsonObjectCopy, CopyAssignmentToSelfLeavesObjectUnchanged)
{
	Marco::JsonObject obj{{"a", 1}, {"b", 2}};

	obj = obj;

	EXPECT_EQ(obj.size(), 2);
	EXPECT_EQ(obj.at("a").AsNumber().value(), 1);
	EXPECT_EQ(obj.at("b").AsNumber().value(), 2);
}

TEST(JsonObjectCopy, CopyAssignmentProducesIndependentObject)
{
	Marco::JsonObject original{{"a", 1}};
	Marco::JsonObject copy{};
	copy = original;

	copy["a"] = "changed";

	EXPECT_EQ(original.at("a").AsNumber().value(), 1);
}

TEST(JsonObjectMove, MoveConstructorTransfersEntries)
{
	Marco::JsonObject original{{"a", 1}, {"b", 2}};
	Marco::JsonObject moved(std::move(original));

	EXPECT_EQ(moved.size(), 2);
	EXPECT_EQ(moved.at("a").AsNumber().value(), 1);
	EXPECT_EQ(moved.at("b").AsNumber().value(), 2);
}

TEST(JsonObjectMove, MoveAssignmentTransfersEntries)
{
	Marco::JsonObject original{{"a", 1}, {"b", 2}};
	Marco::JsonObject target{{"c", 3}};

	target = std::move(original);

	EXPECT_EQ(target.size(), 2);
	EXPECT_EQ(target.at("a").AsNumber().value(), 1);
	EXPECT_EQ(target.find("c"), target.end());
}

TEST(JsonObjectMove, MovedFromObjectCanBeReused)
{
	Marco::JsonObject original{{"a", 1}};
	Marco::JsonObject moved(std::move(original));

	original["b"] = 2;

	EXPECT_EQ(original.size(), 1);
	EXPECT_EQ(original.at("b").AsNumber().value(), 2);
}

TEST(JsonObjectIteration, MutableIteratorAllowsModifyingValues)
{
	Marco::JsonObject obj{{"a", 1}};

	for (auto& entry : obj)
	{
		entry.second = "changed";
	}

	EXPECT_EQ(obj.at("a").AsString().value().get(), "changed");
}

TEST(JsonObjectIteration, ConstIteratorVisitsAllEntries)
{
	const Marco::JsonObject obj{{"a", 1}, {"b", 2}};

	size_t count = 0;
	for (const auto& entry : obj)
	{
		count++;
	}

	EXPECT_EQ(count, 2);
}

TEST(JsonObjectIteration, EmptyObjectHasEqualBeginAndEnd)
{
	Marco::JsonObject obj;

	EXPECT_EQ(obj.begin(), obj.end());
}

TEST(JsonObjectAt, ReturnsReferenceToExistingValue)
{
	Marco::JsonObject obj{{"a", 1}};

	EXPECT_EQ(obj.at("a").AsNumber().value(), 1);
}

TEST(JsonObjectAt, ThrowsOutOfRangeForMissingKey)
{
	Marco::JsonObject obj{{"a", 1}};

	EXPECT_THROW(obj.at("missing"), std::out_of_range);
}

TEST(JsonObjectAt, ReturnedReferenceAllowsMutation)
{
	Marco::JsonObject obj{{"a", 1}};

	obj.at("a") = "changed";

	EXPECT_EQ(obj.at("a").AsString().value().get(), "changed");
}

TEST(JsonObjectFind, ReturnsIteratorToExistingEntry)
{
	Marco::JsonObject obj{{"a", 1}};

	auto it = obj.find("a");

	ASSERT_NE(it, obj.end());
	EXPECT_EQ(it->first, "a");
}

TEST(JsonObjectFind, ReturnsEndIteratorForMissingKey)
{
	Marco::JsonObject obj{{"a", 1}};

	EXPECT_EQ(obj.find("missing"), obj.end());
}

TEST(JsonObjectFind, ConstOverloadReturnsIteratorToExistingEntry)
{
	const Marco::JsonObject obj{{"a", 1}};

	auto it = obj.find("a");

	ASSERT_NE(it, obj.end());
	EXPECT_EQ(it->first, "a");
}

TEST(JsonObjectFind, ConstOverloadReturnsEndIteratorForMissingKey)
{
	const Marco::JsonObject obj{{"a", 1}};

	EXPECT_EQ(obj.find("missing"), obj.end());
}

TEST(JsonObjectSize, ReturnsZeroForEmptyObject)
{
	Marco::JsonObject obj;

	EXPECT_EQ(obj.size(), 0);
}

TEST(JsonObjectSize, ReturnsNumberOfUniqueKeys)
{
	Marco::JsonObject obj{{"a", 1}, {"b", 2}, {"a", 3}};

	EXPECT_EQ(obj.size(), 2);
}

TEST(JsonObjectIndexOperator, CreatesNullEntryForMissingKey)
{
	Marco::JsonObject obj;

	obj["a"];

	EXPECT_EQ(obj.size(), 1);
	EXPECT_NE(obj.find("a"), obj.end());
	EXPECT_TRUE(obj.at("a").IsNull());
}

TEST(JsonObjectIndexOperator, DoesNotDuplicateExistingKey)
{
	Marco::JsonObject obj{{"a", 1}};

	obj["a"];

	EXPECT_EQ(obj.size(), 1);
}

TEST(JsonObjectIndexOperator, AllowsAssigningNewValues)
{
	Marco::JsonObject obj{};

	obj["a"] = 1;

	EXPECT_EQ(obj.at("a").AsNumber().value(), 1);
}

TEST(JsonObjectConstIndexOperator, ReturnsValueForExistingKey)
{
	const Marco::JsonObject obj{{"a", 1}};

	auto result = obj["a"];

	ASSERT_TRUE(result.has_value());
	EXPECT_EQ(result->get().AsNumber().value(), 1);
}

TEST(JsonObjectConstIndexOperator, ReturnsUnexpectedForMissingKey)
{
	const Marco::JsonObject obj{{"a", 1}};

	auto result = obj["missing"];

	ASSERT_FALSE(result.has_value());
	EXPECT_EQ(result.error(), Marco::JsonErrorType::KeyNotFound);
}

TEST(JsonObjectConstIndexOperator, DoesNotModifyObjectOnMissingKey)
{
	const Marco::JsonObject obj{{"a", 1}};

	auto value = obj["missing"];
	(void)value;
	
	EXPECT_EQ(obj.size(), 1);
}

TEST(JsonObjectConstAccess, ChainedConstAtCallsReachNestedObjectValue)
{
	Marco::JsonObject inner{{"x", 1}};
	const Marco::JsonObject obj{{"outer", inner}};

	EXPECT_EQ(obj.at("outer").AsObject().value().get().at("x").AsNumber().value(), 1);
}

TEST(JsonObjectConstAccess, ConstFindOnNestedObjectLocatesNestedKey)
{
	Marco::JsonObject inner{{"x", 1}};
	const Marco::JsonObject obj{{"outer", inner}};

	auto outerIt = obj.find("outer");
	ASSERT_NE(outerIt, obj.end());

	auto nestedObject = outerIt->second.AsObject();
	ASSERT_TRUE(nestedObject.has_value());
	EXPECT_NE(nestedObject->get().find("x"), nestedObject->get().end());
}

TEST(JsonObjectConstAccess, ConstIndexOperatorOnNestedObjectReturnsNestedValue)
{
	Marco::JsonObject inner{{"x", 1}};
	const Marco::JsonObject obj{{"outer", inner}};

	auto outerResult = obj["outer"];
	ASSERT_TRUE(outerResult.has_value());

	auto nestedObject = outerResult->get().AsObject();
	ASSERT_TRUE(nestedObject.has_value());
	EXPECT_EQ(nestedObject->get().at("x").AsNumber().value(), 1);
}

TEST(JsonObjectConstAccess, MultipleConstAccessesDoNotModifyObject)
{
	const Marco::JsonObject obj{{"a", 1}, {"b", 2}};

	obj.at("a");
	obj.find("b");
	auto value = obj["a"];
	(void)value;
	
	EXPECT_EQ(obj.size(), 2);
}