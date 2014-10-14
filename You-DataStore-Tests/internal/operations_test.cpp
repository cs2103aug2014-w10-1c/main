#include "stdafx.h"
#include "CppUnitTest.h"

#include "../dummy_values.h"
#include "internal/operations/erase_operation.h"
#include "internal/operations/post_operation.h"
#include "internal/operations/put_operation.h"
#include "internal/operations/serialization_operation.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace DataStore {
namespace UnitTests {

/// Unit Test Class for DataStore class
TEST_CLASS(DataStoreOperationsTest) {
public:
	TEST_METHOD(serializeOperation) {
		pugi::xml_document document;
		pugi::xml_node taskNode = document.append_child();
		Internal::SerializationOperation::serialize(task1, taskNode);

		Assert::AreEqual(task1.at(TASK_ID).c_str(),
			taskNode.child(TASK_ID.c_str()).child_value());
		Assert::AreEqual(task1.at(DESCRIPTION).c_str(),
			taskNode.child(DESCRIPTION.c_str()).child_value());
		Assert::AreEqual(task1.at(DEADLINE).c_str(),
			taskNode.child(DEADLINE.c_str()).child_value());
		Assert::AreEqual(task1.at(PRIORITY).c_str(),
			taskNode.child(PRIORITY.c_str()).child_value());
		Assert::AreEqual(task1.at(DEPENDENCIES).c_str(),
			taskNode.child(DEPENDENCIES.c_str()).child_value());
	}

	TEST_METHOD(deserializeOperation) {
		pugi::xml_document document;
		pugi::xml_node taskNode = document.append_child();
		taskNode.append_child(L"elementName").
			append_child(pugi::xml_node_type::node_pcdata).set_value(L"pcdata");
		SerializedTask task =
			Internal::SerializationOperation::deserialize(taskNode);
		Assert::AreEqual(static_cast<Value>(L"pcdata"), task.at(L"elementName"));
	}

	TEST_METHOD(postWithNewId) {
		pugi::xml_document document;
		Assert::IsTrue(document.first_child().empty());

		Internal::PostOperation post(0, task1);
		bool status = post.run(document);
		Assert::IsTrue(status);

		// Check the content
		pugi::xml_node taskNode = document.child(L"task");
		Assert::AreEqual(task1.at(TASK_ID).c_str(),
			taskNode.child(TASK_ID.c_str()).child_value());
		Assert::AreEqual(task1.at(DESCRIPTION).c_str(),
			taskNode.child(DESCRIPTION.c_str()).child_value());
		Assert::AreEqual(task1.at(DEADLINE).c_str(),
			taskNode.child(DEADLINE.c_str()).child_value());
		Assert::AreEqual(task1.at(PRIORITY).c_str(),
			taskNode.child(PRIORITY.c_str()).child_value());
		Assert::AreEqual(task1.at(DEPENDENCIES).c_str(),
			taskNode.child(DEPENDENCIES.c_str()).child_value());
	}

	TEST_METHOD(postWithUsedId) {
		pugi::xml_document document;
		pugi::xml_node node = document.append_child(L"task");
		node.append_attribute(L"id").set_value(L"0");

		Internal::PostOperation post(0, task1);
		bool status = post.run(document);
		Assert::IsFalse(status);
		
		// Check the content
		pugi::xml_node taskNode = document.child(L"task");
		Assert::IsTrue(taskNode.first_child().empty());
	}

	TEST_METHOD(putWithExistingId) {
		pugi::xml_document document;
		pugi::xml_node node = document.append_child(L"task");
		node.append_attribute(L"id").set_value(L"0");

		Internal::PutOperation put(0, task1);
		bool status = put.run(document);
		Assert::IsTrue(status);

		// Check the content
		pugi::xml_node taskNode = document.child(L"task");
		Assert::AreEqual(task1.at(TASK_ID).c_str(),
			taskNode.child(TASK_ID.c_str()).child_value());
		Assert::AreEqual(task1.at(DESCRIPTION).c_str(),
			taskNode.child(DESCRIPTION.c_str()).child_value());
		Assert::AreEqual(task1.at(DEADLINE).c_str(),
			taskNode.child(DEADLINE.c_str()).child_value());
		Assert::AreEqual(task1.at(PRIORITY).c_str(),
			taskNode.child(PRIORITY.c_str()).child_value());
		Assert::AreEqual(task1.at(DEPENDENCIES).c_str(),
			taskNode.child(DEPENDENCIES.c_str()).child_value());
	}

	TEST_METHOD(putNonExistentId) {
		pugi::xml_document document;
		pugi::xml_node node = document.append_child(L"task");
		node.append_attribute(L"id").set_value(L"0");

		Internal::PutOperation put(1, task1);
		bool status = put.run(document);
		Assert::IsFalse(status);

		// Check the content
		pugi::xml_node taskNode = document.child(L"task");
		Assert::AreEqual(L"0", taskNode.attribute(L"id").value());
	}

	TEST_METHOD(eraseExistingId) {
		pugi::xml_document document;
		pugi::xml_node node = document.append_child(L"task");
		node.append_attribute(L"id").set_value(L"0");

		Internal::EraseOperation erase(0);
		bool status = erase.run(document);
		Assert::IsTrue(status);
		Assert::IsTrue(document.first_child().empty());
	}

	TEST_METHOD(eraseNonExistentId) {
		pugi::xml_document document;
		pugi::xml_node node = document.append_child(L"task");
		node.append_attribute(L"id").set_value(L"0");

		Internal::EraseOperation erase(1);
		bool status = erase.run(document);
		Assert::IsFalse(status);
		Assert::IsFalse(document.first_child().empty());
	}
};
}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You
