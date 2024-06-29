#include <gtest/gtest.h>
#include "kb_entity.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <json/json.h>

// Проверяет, что метод getAttrs у KBEntity возвращает пустую карту атрибутов по умолчанию.
TEST(KBEntityTest, TestGetAttrs) {
    KBEntity entity("testTag");
    std::map<std::string, std::string> attrs = entity.getAttrs();
    EXPECT_TRUE(attrs.empty());
}

// Проверяет, что метод toXML у KBEntity создает XML-узел с правильным тегом.
TEST(KBEntityTest, TestToXML) {
    KBEntity entity("testTag");
    xmlNodePtr node = entity.toXML();
    EXPECT_STREQ((const char*)node->name, "testTag");
    EXPECT_EQ(node->properties, nullptr);
    xmlFreeNode(node);
}

// Проверяет, что метод toJSON у KBEntity создает JSON-объект с правильным тегом.
TEST(KBEntityTest, TestToJSON) {
    KBEntity entity("testTag");
    Json::Value json = entity.toJSON();
    EXPECT_EQ(json["tag"].asString(), "testTag");
}

// Проверяет, что метод setTag у KBEntity правильно изменяет тег.
TEST(KBEntityTest, TestSetTag) {
    KBEntity entity("initialTag");
    entity.setTag("newTag");
    EXPECT_EQ(entity.getTag(), "newTag");
}

// Проверяет, что метод getInnerXML у KBEntity возвращает пустой вектор по умолчанию.
TEST(KBEntityTest, TestGetInnerXML) {
    KBEntity entity("testTag");
    std::vector<xmlNodePtr> innerXML = entity.getInnerXML();
    EXPECT_TRUE(innerXML.empty());
}

// Проверяет, что конструктор KBIdentity правильно инициализирует поля и геттеры возвращают правильные значения.
TEST(KBIdentityTest, TestConstructorAndGetters) {
    KBIdentity identity("testId", "testTag", "testDesc");
    EXPECT_EQ(identity.getId(), "testId");
    EXPECT_EQ(identity.getTag(), "testTag");
    EXPECT_STREQ(identity.getDesc(), "testDesc");
}

// Проверяет, что метод setDesc у KBIdentity правильно изменяет описание.
TEST(KBIdentityTest, TestSetDesc) {
    KBIdentity identity("testId", "testTag", "initialDesc");
    identity.setDesc("newDesc");
    EXPECT_STREQ(identity.getDesc(), "newDesc");
}

// Проверяет, что метод getAttrs у KBIdentity возвращает карту атрибутов с правильными значениями.
TEST(KBIdentityTest, TestGetAttrs) {
    KBIdentity identity("testId", "testTag", "testDesc");
    std::map<std::string, std::string> attrs = identity.getAttrs();
    EXPECT_EQ(attrs["id"], "testId");
    EXPECT_EQ(attrs["desc"], "testDesc");
}

// Проверяет, что метод toXML у KBIdentity создает XML-узел с правильными атрибутами.
TEST(KBIdentityTest, TestToXML) {
    KBIdentity identity("testId", "testTag", "testDesc");
    xmlNodePtr node = identity.toXML();
    EXPECT_STREQ((const char*)node->name, "testTag");
    EXPECT_STREQ((const char*)xmlGetProp(node, BAD_CAST "id"), "testId");
    EXPECT_STREQ((const char*)xmlGetProp(node, BAD_CAST "desc"), "testDesc");
    xmlFreeNode(node);
}

// Проверяет, что метод toJSON у KBIdentity создает JSON-объект с правильными значениями.
TEST(KBIdentityTest, TestToJSON) {
    KBIdentity identity("testId", "testTag", "testDesc");
    Json::Value json = identity.toJSON();
    EXPECT_EQ(json["id"].asString(), "testId");
    EXPECT_EQ(json["desc"].asString(), "testDesc");
    EXPECT_EQ(json["tag"].asString(), "testTag");
}

// Проверяет, что конструктор KBIdentity правильно обрабатывает отсутствие описания.
TEST(KBIdentityTest, TestConstructorNoDesc) {
    KBIdentity identity("testId", "testTag", nullptr);
    EXPECT_EQ(identity.getId(), "testId");
    EXPECT_EQ(identity.getTag(), "testTag");
    EXPECT_EQ(identity.getDesc(), nullptr);
}

// Проверяет, что метод getComment у KBIdentity возвращает описание, если оно задано.
TEST(KBIdentityTest, TestGetCommentWithDesc) {
    KBIdentity identity("testId", "testTag", "testDesc");
    EXPECT_EQ(identity.getComment(), "testDesc");
}

// Проверяет, что метод getComment у KBIdentity возвращает id, если описание не задано.
TEST(KBIdentityTest, TestGetCommentWithoutDesc) {
    KBIdentity identity("testId", "testTag", nullptr);
    EXPECT_EQ(identity.getComment(), "testId");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
