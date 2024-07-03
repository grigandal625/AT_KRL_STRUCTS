#include <gtest/gtest.h>
#include "non_factor.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <json/json.h>

TEST(NonFactorTest, TestDefaults) {
    NonFactor nf;
    EXPECT_EQ(nf.getBelief(), 50.0);
    EXPECT_EQ(nf.getProbability(), 100.0);
    EXPECT_EQ(nf.getAccuracy(), 0.0);
    EXPECT_TRUE(nf.isDefault());
}

TEST(NonFactorTest, TestCustomValues) {
    NonFactor nf(75.0, 80.0, 90.0);
    EXPECT_EQ(nf.getBelief(), 75.0);
    EXPECT_EQ(nf.getProbability(), 80.0);
    EXPECT_EQ(nf.getAccuracy(), 90.0);
    EXPECT_FALSE(nf.isDefault());
}

TEST(NonFactorTest, TestToJSON) {
    NonFactor nf(75.0, 80.0, 90.0);
    Json::Value json = nf.toJSON();
    EXPECT_EQ(json["belief"].asDouble(), 75.0);
    EXPECT_EQ(json["probability"].asDouble(), 80.0);
    EXPECT_EQ(json["accuracy"].asDouble(), 90.0);
}

TEST(NonFactorTest, TestFromJSON) {
    Json::Value json;
    json["belief"] = 75.0;
    json["probability"] = 80.0;
    json["accuracy"] = 90.0;

    NonFactor* nf = NonFactor::fromJSON(json);
    EXPECT_EQ(nf->getBelief(), 75.0);
    EXPECT_EQ(nf->getProbability(), 80.0);
    EXPECT_EQ(nf->getAccuracy(), 90.0);
    EXPECT_FALSE(nf->isDefault());

    delete nf;
}

TEST(NonFactorTest, TestToXML) {
    NonFactor nf(75.0, 80.0, 90.0);
    xmlNodePtr node = nf.toXML();
    EXPECT_STREQ((const char*)xmlGetProp(node, BAD_CAST "belief"), "75");
    EXPECT_STREQ((const char*)xmlGetProp(node, BAD_CAST "probability"), "80");
    EXPECT_STREQ((const char*)xmlGetProp(node, BAD_CAST "accuracy"), "90");
    xmlFreeNode(node);
}

TEST(NonFactorTest, TestFromXML) {
    xmlNodePtr node = xmlNewNode(nullptr, BAD_CAST "with");
    xmlNewProp(node, BAD_CAST "belief", BAD_CAST "75.0");
    xmlNewProp(node, BAD_CAST "probability", BAD_CAST "80.0");
    xmlNewProp(node, BAD_CAST "accuracy", BAD_CAST "90.0");

    NonFactor* nf = NonFactor::fromXML(node);
    EXPECT_EQ(nf->getBelief(), 75.0);
    EXPECT_EQ(nf->getProbability(), 80.0);
    EXPECT_EQ(nf->getAccuracy(), 90.0);
    EXPECT_FALSE(nf->isDefault());

    delete nf;
    xmlFreeNode(node);
}
