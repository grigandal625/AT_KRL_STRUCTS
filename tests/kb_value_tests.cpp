#include <gtest/gtest.h>
#include "kb_value.h"
#include "non_factor.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <json/json.h>

TEST(KBValueTest, TestFromXMLBoolean) {
    xmlNodePtr node = xmlNewNode(nullptr, BAD_CAST "value");
    xmlNodeSetContent(node, BAD_CAST "True");

    KBValue* value = KBValue::fromXML(node);
    ASSERT_TRUE(value != nullptr);

    // Проверка шаблонного метода
    EXPECT_EQ(value->getContent<bool>(), true);
    // Проверка обычного метода
    EXPECT_EQ(dynamic_cast<KBBooleanValue*>(value)->getContent(), true);

    delete value;
    xmlFreeNode(node);
}

TEST(KBValueTest, TestFromXMLNumeric) {
    xmlNodePtr node = xmlNewNode(nullptr, BAD_CAST "value");
    xmlNodeSetContent(node, BAD_CAST "42.0");

    KBValue* value = KBValue::fromXML(node);
    ASSERT_TRUE(value != nullptr);

    // Проверка шаблонного метода
    EXPECT_EQ(value->getContent<double>(), 42.0);
    // Проверка обычного метода
    EXPECT_EQ(dynamic_cast<KBNumericValue*>(value)->getContent(), 42.0);

    delete value;
    xmlFreeNode(node);
}

TEST(KBValueTest, TestFromXMLSymbolic) {
    xmlNodePtr node = xmlNewNode(nullptr, BAD_CAST "value");
    xmlNodeSetContent(node, BAD_CAST "hello");

    KBValue* value = KBValue::fromXML(node);
    ASSERT_TRUE(value != nullptr);

    // Проверка шаблонного метода
    EXPECT_EQ(value->getContent<string>(), "hello");
    // Проверка обычного метода
    EXPECT_EQ(dynamic_cast<KBSymbolicValue*>(value)->getContentAsString(), "hello");

    delete value;
    xmlFreeNode(node);
}

TEST(KBValueTest, TestFromJSONBoolean) {
    Json::Value json;
    json["content"] = true;

    KBValue* value = KBValue::fromJSON(json);
    ASSERT_TRUE(value != nullptr);

    // Проверка шаблонного метода
    EXPECT_EQ(value->getContent<bool>(), true);
    // Проверка обычного метода
    EXPECT_EQ(dynamic_cast<KBBooleanValue*>(value)->getContent(), true);

    delete value;
}

TEST(KBValueTest, TestFromJSONNumeric) {
    Json::Value json;
    json["content"] = 42.0;

    KBValue* value = KBValue::fromJSON(json);
    ASSERT_TRUE(value != nullptr);

    // Проверка шаблонного метода
    EXPECT_EQ(value->getContent<double>(), 42.0);
    // Проверка обычного метода
    EXPECT_EQ(dynamic_cast<KBNumericValue*>(value)->getContent(), 42.0);

    delete value;
}

TEST(KBValueTest, TestFromJSONSymbolic) {
    Json::Value json;
    json["content"] = "hello";

    KBValue* value = KBValue::fromJSON(json);
    ASSERT_TRUE(value != nullptr);

    // Проверка шаблонного метода
    EXPECT_EQ(value->getContent<string>(), "hello");
    // Проверка обычного метода
    EXPECT_EQ(dynamic_cast<KBSymbolicValue*>(value)->getContentAsString(), "hello");

    delete value;
}

TEST(KBValueTest, TestBadCastFromBooleanToNumeric) {
    Json::Value json;
    json["content"] = true;

    KBValue* value = KBValue::fromJSON(json);
    ASSERT_TRUE(value != nullptr);

    EXPECT_THROW(value->getContent<double>(), std::bad_cast);

    delete value;
}

TEST(KBValueTest, TestBadCastFromNumericToBoolean) {
    Json::Value json;
    json["content"] = 42.0;

    KBValue* value = KBValue::fromJSON(json);
    ASSERT_TRUE(value != nullptr);

    EXPECT_THROW(value->getContent<bool>(), std::bad_cast);

    delete value;
}

TEST(KBValueTest, TestBadCastFromSymbolicToNumeric) {
    Json::Value json;
    json["content"] = "hello";

    KBValue* value = KBValue::fromJSON(json);
    ASSERT_TRUE(value != nullptr);

    EXPECT_THROW(value->getContent<double>(), std::bad_cast);

    delete value;
}

TEST(KBValueTest, TestBadCastFromSymbolicToBoolean) {
    Json::Value json;
    json["content"] = "hello";

    KBValue* value = KBValue::fromJSON(json);
    ASSERT_TRUE(value != nullptr);

    EXPECT_THROW(value->getContent<bool>(), std::bad_cast);

    delete value;
}
