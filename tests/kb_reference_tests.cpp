#include <gtest/gtest.h>
#include "kb_reference.h"
#include "non_factor.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

// Тестирование конструктора и основных методов
TEST(KBReferenceTest, ConstructorAndGetters) {
    KBReference* ref1 = new KBReference("id1");
    EXPECT_EQ(ref1->getId(), "id1");
    EXPECT_EQ(ref1->getRef(), nullptr);

    KBReference* ref2 = new KBReference("id2", ref1);
    EXPECT_EQ(ref2->getId(), "id2");
    EXPECT_EQ(ref2->getRef(), ref1);

    delete ref2; // ref2 owns ref1, so deleting ref2 will also delete ref1
}

// Тестирование метода getAttrs()
TEST(KBReferenceTest, GetAttrs) {
    KBReference* ref = new KBReference("id");
    map<string, string> attrs = ref->getAttrs();
    EXPECT_EQ(attrs["id"], "id");
    delete ref;
}

// Тестирование метода getInnerXML()
TEST(KBReferenceTest, GetInnerXML) {
    KBReference* ref1 = new KBReference("id1");
    KBReference* ref2 = new KBReference("id2", ref1);

    vector<xmlNodePtr> innerXML = ref2->getInnerXML();
    ASSERT_EQ(innerXML.size(), 1);
    EXPECT_STREQ((const char*)innerXML[0]->name, "ref");

    // Cleanup
    xmlFreeNode(innerXML[0]);
    delete ref2;
}

// Тестирование метода toJSON()
TEST(KBReferenceTest, ToJSON) {
    KBReference* ref1 = new KBReference("id1");
    KBReference* ref2 = new KBReference("id2", ref1);
    Json::Value json = ref2->toJSON();

    EXPECT_EQ(json["id"], "id2");
    EXPECT_EQ(json["ref"]["id"], "id1");

    delete ref2;
}

// Тестирование статического метода fromXML()
TEST(KBReferenceTest, FromXML) {
    // Создание XML-узла для теста
    xmlDocPtr xmlDoc = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr root = xmlNewNode(nullptr, BAD_CAST "KBReference");
    xmlNewProp(root, BAD_CAST "id", BAD_CAST "id2");
    xmlNodePtr child = xmlNewNode(nullptr, BAD_CAST "ref");
    xmlNewProp(child, BAD_CAST "id", BAD_CAST "id1");
    xmlAddChild(root, child);
    xmlDocSetRootElement(xmlDoc, root);

    // Тестирование метода fromXML
    KBReference* ref = KBReference::fromXML(root);
    ASSERT_NE(ref, nullptr);
    EXPECT_EQ(ref->getId(), "id2");
    EXPECT_NE(ref->getRef(), nullptr);
    EXPECT_EQ(ref->getRef()->getId(), "id1");

    // Освобождение ресурсов
    xmlFreeDoc(xmlDoc);
    delete ref;
}

// Тестирование метода getInnerKRL()
TEST(KBReferenceTest, GetInnerKRL) {
    KBReference* ref1 = new KBReference("id1");
    KBReference* ref2 = new KBReference("id2", ref1);
    KBReference* ref3 = new KBReference("id3", ref2);

    EXPECT_EQ(ref3->getInnerKRL(), "id3.id2.id1");

    delete ref3;
}

// Тестирование метода KRL(), который использует NonFactor
TEST(KBReferenceTest, KRLWithNonFactor) {
    NonFactor* nonFactor = new NonFactor(70.0, 95.0, 0.5);
    KBReference* ref = new KBReference("id", nullptr, nonFactor);

    EXPECT_EQ(ref->KRL(), "id УВЕРЕННОСТЬ [70; 95] ТОЧНОСТЬ 0.5");

    delete ref;
}

// Тестирование метода toJSON(), который должен включать NonFactor, если он задан
TEST(KBReferenceTest, ToJSONWithNonFactor) {
    NonFactor* nonFactor = new NonFactor(70.0, 95.0, 0.5);
    KBReference* ref = new KBReference("id", nullptr, nonFactor);

    Json::Value json = ref->toJSON();
    EXPECT_EQ(json["id"], "id");
    EXPECT_EQ(json["non_factor"]["belief"], 70.0);
    EXPECT_EQ(json["non_factor"]["probability"], 95.0);
    EXPECT_EQ(json["non_factor"]["accuracy"], 0.5);

    delete ref;
}

// Тестирование метода fromJSON(), который должен корректно восстанавливать объект с NonFactor
TEST(KBReferenceTest, FromJSONWithNonFactor) {
    Json::Value json;
    json["id"] = "id";
    json["non_factor"]["belief"] = 70.0;
    json["non_factor"]["probability"] = 95.0;
    json["non_factor"]["accuracy"] = 0.5;

    KBReference* ref = KBReference::fromJSON(json);
    ASSERT_NE(ref, nullptr);
    EXPECT_EQ(ref->getId(), "id");
    EXPECT_NE(ref->getNonFactor(), nullptr);
    EXPECT_EQ(ref->getNonFactor()->getBelief(), 70.0);
    EXPECT_EQ(ref->getNonFactor()->getProbability(), 95.0);
    EXPECT_EQ(ref->getNonFactor()->getAccuracy(), 0.5);

    delete ref;
}

// Тестирование метода toXML(), который должен включать NonFactor, если он задан
TEST(KBReferenceTest, ToXMLWithNonFactor) {
    NonFactor* nonFactor = new NonFactor(70.0, 95.0, 0.5);
    KBReference* ref = new KBReference("id", nullptr, nonFactor);

    xmlNodePtr xml = ref->toXML();
    ASSERT_NE(xml, nullptr);

    xmlChar* idAttr = xmlGetProp(xml, BAD_CAST "id");
    EXPECT_STREQ((const char*)idAttr, "id");
    xmlFree(idAttr);

    xmlNodePtr nonFactorNode = xmlFirstElementChild(xml);
    ASSERT_NE(nonFactorNode, nullptr);
    EXPECT_STREQ((const char*)nonFactorNode->name, "with");

    xmlChar* beliefAttr = xmlGetProp(nonFactorNode, BAD_CAST "belief");
    xmlChar* probabilityAttr = xmlGetProp(nonFactorNode, BAD_CAST "probability");
    xmlChar* accuracyAttr = xmlGetProp(nonFactorNode, BAD_CAST "accuracy");
    EXPECT_STREQ((const char*)beliefAttr, "70");
    EXPECT_STREQ((const char*)probabilityAttr, "95");
    EXPECT_STREQ((const char*)accuracyAttr, "0.5");

    xmlFree(beliefAttr);
    xmlFree(probabilityAttr);
    xmlFree(accuracyAttr);
    xmlFreeNode(xml);
    
    delete ref;
}

// Тестирование метода fromXML(), который должен корректно восстанавливать объект с NonFactor
TEST(KBReferenceTest, FromXMLWithNonFactor) {
    xmlDocPtr xmlDoc = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr root = xmlNewNode(nullptr, BAD_CAST "ref");
    xmlNewProp(root, BAD_CAST "id", BAD_CAST "id");
    xmlNodePtr nonFactorNode = xmlNewNode(nullptr, BAD_CAST "with");
    xmlNewProp(nonFactorNode, BAD_CAST "belief", BAD_CAST "70");
    xmlNewProp(nonFactorNode, BAD_CAST "probability", BAD_CAST "95");
    xmlNewProp(nonFactorNode, BAD_CAST "accuracy", BAD_CAST "0.5");
    xmlAddChild(root, nonFactorNode);
    xmlDocSetRootElement(xmlDoc, root);

    KBReference* ref = KBReference::fromXML(root);
    ASSERT_NE(ref, nullptr);
    EXPECT_EQ(ref->getId(), "id");
    EXPECT_NE(ref->getNonFactor(), nullptr);
    EXPECT_EQ(ref->getNonFactor()->getBelief(), 70.0);
    EXPECT_EQ(ref->getNonFactor()->getProbability(), 95.0);
    EXPECT_EQ(ref->getNonFactor()->getAccuracy(), 0.5);

    xmlFreeDoc(xmlDoc);
    delete ref;
}
