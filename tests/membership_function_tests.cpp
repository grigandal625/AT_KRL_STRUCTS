#include <gtest/gtest.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "membership_function.h"
#include <json/json.h>

using namespace std;

// Тесты для класса MFPoint
TEST(MFPointTest, Initialization) {
    MFPoint point(1.5, 2.5);
    EXPECT_EQ(point.x, 1.5);
    EXPECT_EQ(point.y, 2.5);
}

TEST(MFPointTest, GetAttrs) {
    MFPoint point(1.5, 2.5);
    auto attrs = point.getAttrs();
    EXPECT_EQ(attrs["x"], "1.5");
    EXPECT_EQ(attrs["y"], "2.5");
}

TEST(MFPointTest, KRL) {
    MFPoint point(1.5, 2.5);
    EXPECT_EQ(point.KRL(), "1.5|2.5");
}

TEST(MFPointTest, ToJSON) {
    MFPoint point(1.5, 2.5);
    auto json = point.toJSON();
    EXPECT_EQ(json["x"].asDouble(), 1.5);
    EXPECT_EQ(json["y"].asDouble(), 2.5);
}

TEST(MFPointTest, FromXML) {
    const char* xmlStr = "<point x=\"1.5\" y=\"2.5\"/>";
    xmlDocPtr doc = xmlReadMemory(xmlStr, strlen(xmlStr), nullptr, nullptr, 0);
    xmlNodePtr root = xmlDocGetRootElement(doc);
    MFPoint* point = MFPoint::fromXML(root);
    EXPECT_EQ(point->x, 1.5);
    EXPECT_EQ(point->y, 2.5);
    xmlFreeDoc(doc);
    delete point;
}

TEST(MFPointTest, FromJSON) {
    Json::Value json;
    json["x"] = 1.5;
    json["y"] = 2.5;
    MFPoint* point = MFPoint::fromJSON(json);
    EXPECT_EQ(point->x, 1.5);
    EXPECT_EQ(point->y, 2.5);
    delete point;
}

// Тесты для класса MembershipFunction
TEST(MembershipFunctionTest, Initialization) {
    vector<MFPoint*> points = {new MFPoint(1.5, 2.5), new MFPoint(3.5, 4.5)};
    MembershipFunction mf("TestFunction", 0.0, 5.0, points);
    EXPECT_EQ(mf.name, "TestFunction");
    EXPECT_EQ(mf.min, 0.0);
    EXPECT_EQ(mf.max, 5.0);
    EXPECT_EQ(mf.points.size(), 2);
}

TEST(MembershipFunctionTest, GetAttrs) {
    vector<MFPoint*> points = {new MFPoint(1.5, 2.5), new MFPoint(3.5, 4.5)};
    MembershipFunction mf("TestFunction", 0.0, 5.0, points);
    auto attrs = mf.getAttrs();
    EXPECT_EQ(attrs["min-value"], "0.000000");
    EXPECT_EQ(attrs["max-value"], "5.000000");
}

TEST(MembershipFunctionTest, ToJSON) {
    vector<MFPoint*> points = {new MFPoint(1.5, 2.5), new MFPoint(3.5, 4.5)};
    MembershipFunction mf("TestFunction", 0.0, 5.0, points);
    auto json = mf.toJSON();
    EXPECT_EQ(json["name"].asString(), "TestFunction");
    EXPECT_EQ(json["min"].asDouble(), 0.0);
    EXPECT_EQ(json["max"].asDouble(), 5.0);
    EXPECT_EQ(json["points"].size(), 2);
}

TEST(MembershipFunctionTest, FromXML) {
    const char* xmlStr = R"(
        <parameter min-value="0.0" max-value="5.0">
            <value>TestFunction</value>
            <mf>
                <point x="1.5" y="2.5"/>
                <point x="3.5" y="4.5"/>
            </mf>
        </parameter>
    )";
    xmlDocPtr doc = xmlReadMemory(xmlStr, strlen(xmlStr), nullptr, nullptr, 0);
    xmlNodePtr root = xmlDocGetRootElement(doc);
    MembershipFunction* mf = MembershipFunction::fromXML(root);
    EXPECT_EQ(mf->name, "TestFunction");
    EXPECT_EQ(mf->min, 0.0);
    EXPECT_EQ(mf->max, 5.0);
    EXPECT_EQ(mf->points.size(), 2);
    EXPECT_EQ(mf->points[0]->x, 1.5);
    EXPECT_EQ(mf->points[0]->y, 2.5);
    EXPECT_EQ(mf->points[1]->x, 3.5);
    EXPECT_EQ(mf->points[1]->y, 4.5);
    xmlFreeDoc(doc);
    delete mf;
}

TEST(MembershipFunctionTest, FromJSON) {
    Json::Value json;
    json["name"] = "TestFunction";
    json["min"] = 0.0;
    json["max"] = 5.0;
    Json::Value point1;
    point1["x"] = 1.5;
    point1["y"] = 2.5;
    Json::Value point2;
    point2["x"] = 3.5;
    point2["y"] = 4.5;
    json["points"].append(point1);
    json["points"].append(point2);
    MembershipFunction* mf = MembershipFunction::fromJSON(json);
    EXPECT_EQ(mf->name, "TestFunction");
    EXPECT_EQ(mf->min, 0.0);
    EXPECT_EQ(mf->max, 5.0);
    EXPECT_EQ(mf->points.size(), 2);
    EXPECT_EQ(mf->points[0]->x, 1.5);
    EXPECT_EQ(mf->points[0]->y, 2.5);
    EXPECT_EQ(mf->points[1]->x, 3.5);
    EXPECT_EQ(mf->points[1]->y, 4.5);
    delete mf;
}

TEST(MembershipFunctionTest, KRL) {
    vector<MFPoint*> points = {new MFPoint(1.5, 2.5), new MFPoint(3.5, 4.5)};
    MembershipFunction mf("TestFunction", 0.0, 5.0, points);
    EXPECT_EQ(mf.KRL(), "\"TestFunction\" 0 5 2 ={1.5|2.5; 3.5|4.5}");
}
