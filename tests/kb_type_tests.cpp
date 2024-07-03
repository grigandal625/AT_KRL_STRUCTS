#include "kb_entity.h"
#include "kb_type.h"
#include "membership_function.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <json/json.h>

using namespace std;


// Test KBType class
TEST(KBTypeTest, Initialization) {
    KBType type("type1", "description1");
    EXPECT_EQ(type.getId(), "type1");
    EXPECT_STREQ(type.getDesc(), "description1");
    EXPECT_EQ(type.getMeta(), "abstract");
    EXPECT_EQ(type.getKRLType(), "АБСТРАКТНЫЙ");
    EXPECT_EQ(type.KRL(), "ТИП type1\nАБСТРАКТНЫЙ\nКОММЕНТАРИЙ description1\n");
}

// Test KBNumericType class
TEST(KBNumericTypeTest, Initialization) {
    KBNumericType numericType("numType", 0.0, 10.0, "numeric description");
    EXPECT_EQ(numericType.getId(), "numType");
    EXPECT_STREQ(numericType.getDesc(), "numeric description");
    EXPECT_EQ(numericType.getMeta(), "number");
    EXPECT_EQ(numericType.getKRLType(), "ЧИСЛО");
    EXPECT_EQ(numericType.KRL(), "ТИП numType\nЧИСЛО\nОТ 0\nДО 10\nКОММЕНТАРИЙ numeric description\n");
}

// Test KBSymbolicType class
TEST(KBSymbolicTypeTest, Initialization) {
    vector<string> values = {"val1", "val2", "val3"};
    KBSymbolicType symbolicType("symType", values, "symbolic description");
    EXPECT_EQ(symbolicType.getId(), "symType");
    EXPECT_STREQ(symbolicType.getDesc(), "symbolic description");
    EXPECT_EQ(symbolicType.getMeta(), "string");
    EXPECT_EQ(symbolicType.getKRLType(), "СИМВОЛ");
    EXPECT_EQ(symbolicType.KRL(), "ТИП symType\nСИМВОЛ\n\"val1\"\n\"val2\"\n\"val3\"\nКОММЕНТАРИЙ symbolic description\n");
}

// Test XML serialization and deserialization for KBNumericType
TEST(KBNumericTypeTest, XMLSerialization) {
    KBNumericType numericType("numType", 0.0, 10.0, "numeric description");

    xmlNodePtr xml = numericType.toXML();
    const char* id = (const char*)xmlGetProp(xml, BAD_CAST "id");
    const char* desc = (const char*)xmlGetProp(xml, BAD_CAST "desc");
    const char* meta = (const char*)xmlGetProp(xml, BAD_CAST "meta");

    EXPECT_STREQ(id, "numType");
    EXPECT_STREQ(desc, "numeric description");
    EXPECT_STREQ(meta, "number");

    xmlNodePtr fromNode = xmlFirstElementChild(xml);
    const char* fromName = (const char*)fromNode->name;
    const char* fromContent = (const char*)xmlNodeGetContent(fromNode);

    EXPECT_STREQ(fromName, "from");
    EXPECT_STREQ(fromContent, "0");

    xmlNodePtr toNode = xmlNextElementSibling(fromNode);
    const char* toName = (const char*)toNode->name;
    const char* toContent = (const char*)xmlNodeGetContent(toNode);

    EXPECT_STREQ(toName, "to");
    EXPECT_STREQ(toContent, "10");

    KBNumericType* deserialized = KBNumericType::fromXML(xml);
    const string& deserializedId = deserialized->getId();
    const char* deserializedDesc = deserialized->getDesc();
    const string& deserializedMeta = deserialized->getMeta();
    const string& deserializedKRL = deserialized->KRL();

    EXPECT_EQ(deserializedId, "numType");
    EXPECT_STREQ(deserializedDesc, "numeric description");
    EXPECT_EQ(deserializedMeta, "number");
    EXPECT_EQ(deserializedKRL, "ТИП numType\nЧИСЛО\nОТ 0\nДО 10\nКОММЕНТАРИЙ numeric description\n");

    xmlFreeNode(xml);
    delete deserialized;
}

// Test XML serialization and deserialization for KBSymbolicType
TEST(KBSymbolicTypeTest, XMLSerialization) {
    vector<string> values = {"val1", "val2", "val3"};
    KBSymbolicType symbolicType("symType", values, "symbolic description");

    xmlNodePtr xml = symbolicType.toXML();
    const char* id = (const char*)xmlGetProp(xml, BAD_CAST "id");
    const char* desc = (const char*)xmlGetProp(xml, BAD_CAST "desc");
    const char* meta = (const char*)xmlGetProp(xml, BAD_CAST "meta");

    EXPECT_STREQ(id, "symType");
    EXPECT_STREQ(desc, "symbolic description");
    EXPECT_STREQ(meta, "string");

    vector<string> deserializedValues;
    for (xmlNodePtr valueNode = xmlFirstElementChild(xml); valueNode; valueNode = xmlNextElementSibling(valueNode)) {
        deserializedValues.push_back((const char*)xmlNodeGetContent(valueNode));
    }

    EXPECT_EQ(deserializedValues.size(), 3);
    EXPECT_EQ(deserializedValues[0], "val1");
    EXPECT_EQ(deserializedValues[1], "val2");
    EXPECT_EQ(deserializedValues[2], "val3");

    KBSymbolicType* deserialized = KBSymbolicType::fromXML(xml);
    const string& deserializedId = deserialized->getId();
    const char* deserializedDesc = deserialized->getDesc();
    const string& deserializedMeta = deserialized->getMeta();
    const string& deserializedKRL = deserialized->KRL();

    EXPECT_EQ(deserializedId, "symType");
    EXPECT_STREQ(deserializedDesc, "symbolic description");
    EXPECT_EQ(deserializedMeta, "string");
    EXPECT_EQ(deserializedKRL, "ТИП symType\nСИМВОЛ\n\"val1\"\n\"val2\"\n\"val3\"\nКОММЕНТАРИЙ symbolic description\n");

    xmlFreeNode(xml);
    delete deserialized;
}

// Test JSON serialization and deserialization for KBNumericType
TEST(KBNumericTypeTest, JSONSerialization) {
    KBNumericType numericType("numType", 0.0, 10.0, "numeric description");

    Json::Value json = numericType.toJSON();
    string id = json["id"].asString();
    string desc = json["desc"].asString();
    string meta = json["meta"].asString();
    double from = json["from"].asDouble();
    double to = json["to"].asDouble();

    EXPECT_EQ(id, "numType");
    EXPECT_EQ(desc, "numeric description");
    EXPECT_EQ(meta, "number");
    EXPECT_EQ(from, 0.0);
    EXPECT_EQ(to, 10.0);

    KBNumericType* deserialized = KBNumericType::fromJSON(json);
    const string& deserializedId = deserialized->getId();
    const char* deserializedDesc = deserialized->getDesc();
    const string& deserializedMeta = deserialized->getMeta();
    const string& deserializedKRL = deserialized->KRL();

    EXPECT_EQ(deserializedId, "numType");
    EXPECT_STREQ(deserializedDesc, "numeric description");
    EXPECT_EQ(deserializedMeta, "number");
    EXPECT_EQ(deserializedKRL, "ТИП numType\nЧИСЛО\nОТ 0\nДО 10\nКОММЕНТАРИЙ numeric description\n");

    delete deserialized;
}

// Test JSON serialization and deserialization for KBSymbolicType
TEST(KBSymbolicTypeTest, JSONSerialization) {
    vector<string> values = {"val1", "val2", "val3"};
    KBSymbolicType symbolicType("symType", values, "symbolic description");

    Json::Value json = symbolicType.toJSON();
    string id = json["id"].asString();
    string desc = json["desc"].asString();
    string meta = json["meta"].asString();
    int valuesSize = json["values"].size();
    string value0 = json["values"][0].asString();
    string value1 = json["values"][1].asString();
    string value2 = json["values"][2].asString();

    EXPECT_EQ(id, "symType");
    EXPECT_EQ(desc, "symbolic description");
    EXPECT_EQ(meta, "string");
    EXPECT_EQ(valuesSize, 3);
    EXPECT_EQ(value0, "val1");
    EXPECT_EQ(value1, "val2");
    EXPECT_EQ(value2, "val3");

    KBSymbolicType* deserialized = KBSymbolicType::fromJSON(json);
    const string& deserializedId = deserialized->getId();
    const char* deserializedDesc = deserialized->getDesc();
    const string& deserializedMeta = deserialized->getMeta();
    const string& deserializedKRL = deserialized->KRL();

    EXPECT_EQ(deserializedId, "symType");
    EXPECT_STREQ(deserializedDesc, "symbolic description");
    EXPECT_EQ(deserializedMeta, "string");
    EXPECT_EQ(deserializedKRL, "ТИП symType\nСИМВОЛ\n\"val1\"\n\"val2\"\n\"val3\"\nКОММЕНТАРИЙ symbolic description\n");

    delete deserialized;
}

TEST(KBFuzzyTypeTests, GetMetaReturnsFuzzy) {
    vector<MembershipFunction*> membership_functions;
    KBFuzzyType fuzzy_type("fuzzy_id", membership_functions);
    EXPECT_EQ(fuzzy_type.getMeta(), "fuzzy");
}

TEST(KBFuzzyTypeTests, GetKRLTypeReturnsFuzzy) {
    vector<MembershipFunction*> membership_functions;
    KBFuzzyType fuzzy_type("fuzzy_id", membership_functions);
    EXPECT_EQ(fuzzy_type.getKRLType(), "НЕЧЕТКИЙ");
}

TEST(KBFuzzyTypeTests, GetInnerKRLReturnsCorrectString) {
    vector<MembershipFunction*> membership_functions = {};
    MFPoint* p1 = new MFPoint(0.0, 0.0);
    MFPoint* p2 = new MFPoint(1.0, 1.0);
    MembershipFunction* mf1 = new MembershipFunction("mf1", 0.0, 1.0, {p1, p2});
    membership_functions.push_back(mf1);
    KBFuzzyType fuzzy_type("fuzzy_id", membership_functions);

    string expected_inner_krl = "1\n\"mf1\" 0 1 2 ={0|0; 1|1}";
    EXPECT_EQ(fuzzy_type.getInnerKRL(), expected_inner_krl);
}

TEST(KBFuzzyTypeTests, ToJSONReturnsCorrectJson) {
    // Создание тестовых данных
    MFPoint* p1 = new MFPoint(0.0, 0.0);
    MFPoint* p2 = new MFPoint(1.0, 1.0);
    MembershipFunction mf1("mf1", 0.0, 1.0, {p1, p2});
    vector<MembershipFunction*> membership_functions = {&mf1};
    KBFuzzyType fuzzy_type("fuzzy_id", membership_functions);

    // Получение JSON от объекта
    Json::Value actual_json = fuzzy_type.toJSON();

    // Проверка каждого значения по отдельности
    string id = actual_json["id"].asString();
    const char *desc = actual_json["desc"].asCString();
    string meta = actual_json["meta"].asString();
    Json::Value membership_functions_json = actual_json["membership_functions"];
    EXPECT_EQ(id, "fuzzy_id");
    EXPECT_STREQ(desc, "fuzzy_id");
    EXPECT_EQ(meta, "fuzzy");
    EXPECT_EQ(membership_functions_json.size(), 1);

    Json::Value mf1_json = membership_functions_json[0];
    string name = mf1_json["name"].asString();
    double min = mf1_json["min"].asDouble();
    double max = mf1_json["max"].asDouble();
    Json::Value points_json = mf1_json["points"];
    EXPECT_EQ(name, "mf1");
    EXPECT_EQ(min, 0.0);
    EXPECT_EQ(max, 1.0);
    EXPECT_EQ(points_json.size(), 2);

    double p1_x = points_json[0]["x"].asDouble();
    double p1_y = points_json[0]["y"].asDouble();
    double p2_x = points_json[1]["x"].asDouble();
    double p2_y = points_json[1]["y"].asDouble();
    EXPECT_EQ(p1_x, 0.0);
    EXPECT_EQ(p1_y, 0.0);
    EXPECT_EQ(p2_x, 1.0);
    EXPECT_EQ(p2_y, 1.0);

    delete p1;
    delete p2;
}

TEST(KBFuzzyTypeTests, KRLReturnsCorrectString) {
    vector<MembershipFunction*> membership_functions;
    MFPoint* p1 = new MFPoint(0.0, 0.0);
    MFPoint* p2 = new MFPoint(1.0, 1.0);
    MFPoint* p3 = new MFPoint(-1.0, 0.5);
    MFPoint* p4 = new MFPoint(1.0, 0.5);
    MembershipFunction mf1("mf1", 0.0, 1.0, {p1, p2});
    MembershipFunction mf2("mf2", -1.0, 1.0, {p3, p4});
    membership_functions.push_back(&mf1);
    membership_functions.push_back(&mf2);
    KBFuzzyType fuzzy_type("fuzzy_id", membership_functions);

    string expected_krl = "ТИП fuzzy_id\n" +
                          fuzzy_type.getKRLType() + "\n" +
                          fuzzy_type.getInnerKRL() + "\n" +
                          "КОММЕНТАРИЙ fuzzy_id\n";
    
    EXPECT_EQ(fuzzy_type.KRL(), expected_krl);
}