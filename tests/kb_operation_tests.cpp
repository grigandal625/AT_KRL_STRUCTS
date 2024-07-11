#include <gtest/gtest.h>
#include "kb_operation.h"
#include "kb_reference.h"
#include "non_factor.h"
#include <iomanip>
#include <sstream>
#include <limits>
#include "utils.h"

class KBOperationTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Initialization code here
    }

    void TearDown() override
    {
        // Clean-up code here
    }
};

TEST(KBOperationTest, TestKBOperationCreation)
{
    // Test creation of KBOperation with binary operator
    KBReference *left = new KBReference("left");
    KBReference *right = new KBReference("right");
    KBOperation op("+", left, right);

    string opSign = op.getSign();
    bool opIsBinary = op.isBinary();
    const KBReference *opLeft = dynamic_cast<const KBReference*>(op.getLeft());
    const KBReference *opRight = dynamic_cast<const KBReference*>(op.getRight());

    EXPECT_EQ(opSign, "+");
    EXPECT_TRUE(opIsBinary);
    EXPECT_EQ(opLeft, left);
    EXPECT_EQ(opRight, right);

    // Test creation of KBOperation with unary operator
    KBOperation op2("!", left);

    string op2Sign = op2.getSign();
    bool op2IsBinary = op2.isBinary();
    const KBReference *op2Left = dynamic_cast<const KBReference*>(op2.getLeft());
    const KBReference *op2Right = dynamic_cast<const KBReference*>(op2.getRight());

    EXPECT_EQ(op2Sign, "!");
    EXPECT_FALSE(op2IsBinary);
    EXPECT_EQ(op2Left, left);
    EXPECT_EQ(op2Right, nullptr);
}

TEST_F(KBOperationTest, TestKBOperationToJSON)
{
    // Test conversion to JSON
    KBReference *left = new KBReference("left");
    KBReference *right = new KBReference("right");
    NonFactor *non_factor = new NonFactor(0.5, 0.6, 0.7);

    KBOperation op("+", left, right, non_factor);
    Json::Value json = op.toJSON();

    string jsonSign = json["sign"].asString();
    string jsonLeftId = json["left"]["id"].asString();
    string jsonRightId = json["right"]["id"].asString();
    double jsonNonFactorBelief = json["non_factor"]["belief"].asDouble();
    double jsonNonFactorProbability = json["non_factor"]["probability"].asDouble();
    double jsonNonFactorAccuracy = json["non_factor"]["accuracy"].asDouble();

    EXPECT_EQ(jsonSign, "+");
    EXPECT_EQ(jsonLeftId, "left");
    EXPECT_EQ(jsonRightId, "right");
    EXPECT_EQ(jsonNonFactorBelief, 0.5);
    EXPECT_EQ(jsonNonFactorProbability, 0.6);
    EXPECT_EQ(jsonNonFactorAccuracy, 0.7);
}

TEST_F(KBOperationTest, TestKBOperationFromJSON)
{
    // Test creation from JSON
    Json::Value json;
    json["sign"] = "+";
    json["left"]["id"] = "left";
    json["left"]["tag"] = "ref";
    json["right"]["id"] = "right";
    json["right"]["tag"] = "ref";
    json["non_factor"]["belief"] = 0.5;
    json["non_factor"]["probability"] = 0.6;
    json["non_factor"]["accuracy"] = 0.7;

    KBOperation *op = KBOperation::fromJSON(json);

    string opSign = op->getSign();
    string opLeftId = dynamic_cast<const KBReference*>(op->getLeft())->getId();
    string opRightId = dynamic_cast<const KBReference*>(op->getRight())->getId();
    double opNonFactorBelief = op->getNonFactor()->getBelief();
    double opNonFactorProbability = op->getNonFactor()->getProbability();
    double opNonFactorAccuracy = op->getNonFactor()->getAccuracy();

    EXPECT_EQ(opSign, "+");
    EXPECT_EQ(opLeftId, "left");
    EXPECT_EQ(opRightId, "right");
    EXPECT_EQ(opNonFactorBelief, 0.5);
    EXPECT_EQ(opNonFactorProbability, 0.6);
    EXPECT_EQ(opNonFactorAccuracy, 0.7);

    delete op;
}

TEST_F(KBOperationTest, TestKBOperationToXML)
{
    // Test conversion to XML
    KBReference *left = new KBReference("left");
    KBReference *right = new KBReference("right");
    NonFactor *non_factor = new NonFactor(0.5, 0.6, 0.7);

    KBOperation op("+", left, right, non_factor);
    xmlNodePtr xml = op.toXML();

    ASSERT_STREQ((const char*)xml->name, "add");

    xmlNodePtr leftRefNode = findNodeByName(xml->children, "ref");
    ASSERT_NE(leftRefNode, nullptr) << "<ref id=\"left\"> node not found.";
    ASSERT_STREQ((const char*)xmlGetProp(leftRefNode, (const xmlChar *)"id"), "left");

    xmlNodePtr rightRefNode = leftRefNode->next;
    while (rightRefNode && (rightRefNode->type != XML_ELEMENT_NODE || xmlStrcmp(rightRefNode->name, (const xmlChar *)"ref") != 0)) {
        rightRefNode = rightRefNode->next;
    }
    ASSERT_NE(rightRefNode, nullptr) << "<ref id=\"right\"> node not found.";
    ASSERT_STREQ((const char*)xmlGetProp(rightRefNode, (const xmlChar *)"id"), "right");

    xmlNodePtr withNode = findNodeByName(xml->children, "with");
    ASSERT_NE(withNode, nullptr) << "<with> node not found.";
    ASSERT_STREQ((const char*)xmlGetProp(withNode, (const xmlChar *)"belief"), "0.5");
    ASSERT_STREQ((const char*)xmlGetProp(withNode, (const xmlChar *)"probability"), "0.6");
    ASSERT_STREQ((const char*)xmlGetProp(withNode, (const xmlChar *)"accuracy"), "0.7");
}

TEST_F(KBOperationTest, TestKBOperationFromXML)
{
    // Test creation from XML
    string xmlText = R"(
    <eq>
        <value>true</value>
        <ref id="TEST_OBJ">
            <ref id="X">
                <with belief="50" probability="100" accuracy="0"/>
            </ref>
            <with belief="50" probability="100" accuracy="0"/>
        </ref>
        <with belief="50" probability="100" accuracy="0"/>
    </eq>)";
    xmlNodePtr xml = parseXmlString(xmlText);

    KBOperation *op = KBOperation::fromXML(xml);

    string opSign = op->getSign();
    const Evaluatable *opLeft = op->getLeft();
    const Evaluatable *opRight = op->getRight();
    double opNonFactorBelief = op->getNonFactor()->getBelief();
    double opNonFactorProbability = op->getNonFactor()->getProbability();
    double opNonFactorAccuracy = op->getNonFactor()->getAccuracy();

    EXPECT_EQ(opSign, "+");
    EXPECT_NE(dynamic_cast<const KBValue*>(opLeft), nullptr);
    EXPECT_NE(dynamic_cast<const KBReference*>(opRight), nullptr);
    EXPECT_EQ(opNonFactorBelief, 0.5);
    EXPECT_EQ(opNonFactorProbability, 0.6);
    EXPECT_EQ(opNonFactorAccuracy, 0.7);

    delete op;
    xmlFreeNode(xml);
}

TEST_F(KBOperationTest, TestKBOperationKRL)
{
    // Test KRL string generation
    KBReference *left = new KBReference("left");
    KBReference *right = new KBReference("right");
    NonFactor *non_factor = new NonFactor(0.5, 0.6, 0.7);

    KBOperation op("+", left, right, non_factor);
    string krl = op.KRL();

    string expectedNonFactorKRL = "УВЕРЕННОСТЬ [" + doubleToString(0.5) + "; " + doubleToString(0.6) + "] ТОЧНОСТЬ " + doubleToString(0.7);
    EXPECT_EQ(krl, "(left) + (right) " + expectedNonFactorKRL);

    KBOperation op2("!", left, nullptr, non_factor);
    string krl2 = op2.KRL();

    EXPECT_EQ(krl2, "! (left) " + expectedNonFactorKRL);
}