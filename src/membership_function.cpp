#include "membership_function.h"
#include <sstream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <json/json.h>

using namespace std;

// Реализация класса MFPoint
MFPoint::MFPoint(float x, float y) : KBEntity("", "point", nullptr), x(x), y(y) {}

map<string, string> MFPoint::getAttrs() const {
    map<string, string> attrs = KBEntity::getAttrs();
    attrs["x"] = to_string(x);
    attrs["y"] = to_string(y);
    return attrs;
}

string MFPoint::KRL() const {
    ostringstream oss;
    oss << x << "|" << y;
    return oss.str();
}

Json::Value MFPoint::toJSON() const {
    Json::Value json = KBEntity::toJSON();
    json["x"] = x;
    json["y"] = y;
    return json;
}

MFPoint* MFPoint::fromXML(xmlNodePtr xml) {
    float x = stof((const char*)xmlGetProp(xml, (const xmlChar*)"x"));
    float y = stof((const char*)xmlGetProp(xml, (const xmlChar*)"y"));
    return new MFPoint(x, y);
}

MFPoint* MFPoint::fromJSON(const Json::Value& json) {
    float x = json["x"].asFloat();
    float y = json["y"].asFloat();
    return new MFPoint(x, y);
}

// Реализация класса MembershipFunction
MembershipFunction::MembershipFunction(const string& name, float min, float max, const vector<MFPoint*>& points)
    : KBEntity("", "parameter", nullptr), name(name), min(min), max(max) {
    this->points = {};
    for (auto& point : points) {
        MFPoint * new_point = MFPoint::fromJSON(point->toJSON());
        new_point->owner = this;
        this->points.push_back(new_point);
    }
}

map<string, string> MembershipFunction::getAttrs() const {
    map<string, string> attrs = KBEntity::getAttrs();
    attrs["min-value"] = to_string(min);
    attrs["max-value"] = to_string(max);
    return attrs;
}

vector<xmlNodePtr> MembershipFunction::getInnerXML() const {
    vector<xmlNodePtr> elements;

    xmlNodePtr value = xmlNewNode(nullptr, BAD_CAST "value");
    xmlNodeSetContent(value, BAD_CAST name.c_str());
    elements.push_back(value);

    xmlNodePtr mf = xmlNewNode(nullptr, BAD_CAST "mf");
    for (const auto& point : points) {
        xmlNodePtr pointElem = xmlNewNode(nullptr, BAD_CAST "point");
        xmlNewProp(pointElem, BAD_CAST "x", BAD_CAST to_string(point->x).c_str());
        xmlNewProp(pointElem, BAD_CAST "y", BAD_CAST to_string(point->y).c_str());
        xmlAddChild(mf, pointElem);
    }
    elements.push_back(mf);

    return elements;
}

Json::Value MembershipFunction::toJSON() const {
    Json::Value json = KBEntity::toJSON();
    json["name"] = name;
    json["min"] = min;
    json["max"] = max;
    for (const auto& point : points) {
        json["points"].append(point->toJSON());
    }
    return json;
}

MembershipFunction* MembershipFunction::fromXML(xmlNodePtr xml) {
    float min = stof((const char*)xmlGetProp(xml, (const xmlChar*)"min-value"));
    float max = stof((const char*)xmlGetProp(xml, (const xmlChar*)"max-value"));

    xmlNodePtr valueElem = xmlFirstElementChild(xml);
    string name = (const char*)xmlNodeGetContent(valueElem);

    xmlNodePtr mfElem = xmlNextElementSibling(valueElem);
    vector<MFPoint*> points;
    for (xmlNodePtr pointElem = xmlFirstElementChild(mfElem); pointElem; pointElem = xmlNextElementSibling(pointElem)) {
        points.push_back(MFPoint::fromXML(pointElem));
    }

    return new MembershipFunction(name, min, max, points);
}

MembershipFunction* MembershipFunction::fromJSON(const Json::Value& json) {
    string name = json["name"].asString();
    float min = json["min"].asFloat();
    float max = json["max"].asFloat();
    vector<MFPoint*> points;
    for (const auto& pointJson : json["points"]) {
        points.push_back(MFPoint::fromJSON(pointJson));
    }
    return new MembershipFunction(name, min, max, points);
}

string MembershipFunction::KRL() const {
    ostringstream oss;
    oss << "\"" << name << "\" " << min << " " << max << " " << points.size() << " ={";
    for (size_t i = 0; i < points.size(); ++i) {
        if (i > 0) oss << "; ";
        oss << points[i]->KRL();
    }
    oss << "}";
    return oss.str();
}