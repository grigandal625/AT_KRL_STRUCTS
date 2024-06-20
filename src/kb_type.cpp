#include "kb_type.h"
#include "kb_entity.h"
#include "membership_function.h"
#include <string>
#include <map>
#include <vector>
#include <libxml/tree.h>
#include <json/value.h>
#include <sstream>

using namespace std;

string join(const vector<string> &values, const string &delimiter)
{
    stringstream ss;
    for (size_t i = 0; i < values.size(); ++i)
    {
        if (i != 0)
        {
            ss << delimiter;
        }
        ss << values[i];
    }
    return ss.str();
}

// KBType implementation

KBType::KBType(const string id, const char *desc)
    : KBEntity(id, "type", desc) {}

string KBType::KRL() const
{
    stringstream ss;
    ss << "ТИП " << getId() << endl;
    ss << getKRLType() << endl;

    string innerKRL = getInnerKRL();
    if (!innerKRL.empty()) {
        ss << innerKRL << endl;
    }
    ss << "КОММЕНТАРИЙ " << (getDesc() != NULL ? getDesc() : getId()) << endl;
    return ss.str();
}

map<string, string> KBType::getAttrs() const
{
    map<string, string> attrs = KBEntity::getAttrs();
    attrs["meta"] = getMeta();
    attrs["desc"] = getDesc() != NULL ? string(getDesc()) : getId();
    return attrs;
}

KBType *KBType::fromXML(xmlNodePtr node)
{
    string meta = (const char *)xmlGetProp(node, BAD_CAST "meta");
    if (meta == "numeric" || meta == "number")
        return KBNumericType::fromXML(node);
    else if (meta == "string" || meta == "symbolic")
        return KBSymbolicType::fromXML(node);
    return nullptr;
}

KBType *KBType::fromJSON(const Json::Value &json)
{
    string meta = json["meta"].asString();
    if (meta == "numeric" || meta == "number")
        return KBNumericType::fromJSON(json);
    else if (meta == "string" || meta == "symbolic")
        return KBSymbolicType::fromJSON(json);
    return nullptr;
}

string KBType::getXMLOwnerPath() const
{
    // This should be implemented based on your specific XML structure.
    // Placeholder implementation:
    return "path/to/xml";
}

// KBNumericType implementation

KBNumericType::KBNumericType(const string id, double from, double to, const char *desc)
    : KBType(id, desc), from(from), to(to) {}

string KBNumericType::getInnerKRL() const
{
    stringstream ss;
    ss << "ОТ " << from << endl;
    ss << "ДО " << to;
    return ss.str();
}

map<string, string> KBNumericType::getAttrs() const
{
    map<string, string> attrs = KBType::getAttrs();
    attrs["from"] = to_string(from);
    attrs["to"] = to_string(to);
    return attrs;
}

vector<xmlNodePtr> KBNumericType::getInnerXML() const
{
    vector<xmlNodePtr> innerXML;
    xmlNodePtr fromNode = xmlNewNode(NULL, BAD_CAST "from");
    xmlNodeSetContent(fromNode, BAD_CAST to_string(from).c_str());
    innerXML.push_back(fromNode);

    xmlNodePtr toNode = xmlNewNode(NULL, BAD_CAST "to");
    xmlNodeSetContent(toNode, BAD_CAST to_string(to).c_str());
    innerXML.push_back(toNode);

    return innerXML;
}

Json::Value KBNumericType::toJSON() const 
{ 
    Json::Value json = KBType::toJSON();
    json["from"] = from;
    json["to"] = to;
    return json;
}

bool KBNumericType::validateValue(const string &value) const
{
    // Placeholder implementation
    // Implement proper validation logic
    return true;
}

KBNumericType *KBNumericType::fromXML(xmlNodePtr node)
{
    string id = (const char *)xmlGetProp(node, BAD_CAST "id");
    const char *desc = (const char *)xmlGetProp(node, BAD_CAST "desc");

    xmlNodePtr fromNode = xmlFirstElementChild(node);
    double from = atof((const char *)xmlNodeGetContent(fromNode));

    xmlNodePtr toNode = xmlNextElementSibling(fromNode);
    double to = atof((const char *)xmlNodeGetContent(toNode));

    return new KBNumericType(id, from, to, desc);
}

KBNumericType *KBNumericType::fromJSON(const Json::Value &json)
{
    string id = json["id"].asString();
    const char *desc = json["desc"].isNull() ? NULL : json["desc"].asCString();
    double from = json["from"].asDouble();
    double to = json["to"].asDouble();

    return new KBNumericType(id, from, to, desc);
}

// KBSymbolicType implementation

KBSymbolicType::KBSymbolicType(const string id, const vector<string> &values, const char *desc)
    : KBType(id, desc), values(values) {}

string KBSymbolicType::getInnerKRL() const
{
    stringstream ss;
    ss << "\"" << join(values, "\"\n\"") << "\"";
    return ss.str();
}

map<string, string> KBSymbolicType::getAttrs() const
{
    map<string, string> attrs = KBType::getAttrs();
    string joined_values = join(values, ",");
    attrs["values"] = joined_values;
    return attrs;
}

vector<xmlNodePtr> KBSymbolicType::getInnerXML() const
{
    vector<xmlNodePtr> innerXML;
    for (const string &v : values)
    {
        xmlNodePtr valueNode = xmlNewNode(NULL, BAD_CAST "value");
        xmlNodeSetContent(valueNode, BAD_CAST v.c_str());
        innerXML.push_back(valueNode);
    }
    return innerXML;
}

Json::Value KBSymbolicType::toJSON() const
{
    Json::Value json = KBType::toJSON();
    Json::Value valuesArray(Json::arrayValue);
    for (const auto& str : values) {
        valuesArray.append(str);
    }
    json["values"] = valuesArray;
    return json;
}

bool KBSymbolicType::validateValue(const string &value) const
{
    return true;
}

KBSymbolicType *KBSymbolicType::fromXML(xmlNodePtr node)
{
    string id = (const char *)xmlGetProp(node, BAD_CAST "id");
    const char *desc = (const char *)xmlGetProp(node, BAD_CAST "desc");

    vector<string> values;
    for (xmlNodePtr valueNode = xmlFirstElementChild(node); valueNode; valueNode = xmlNextElementSibling(valueNode))
    {
        values.push_back((const char *)xmlNodeGetContent(valueNode));
    }

    return new KBSymbolicType(id, values, desc);
}

KBSymbolicType *KBSymbolicType::fromJSON(const Json::Value &json)
{
    string id = json["id"].asString();
    const char *desc = json["desc"].isNull() ? NULL : json["desc"].asCString();
    vector<string> values;
    for (const Json::Value &value : json["values"])
    {
        values.push_back(value.asString());
    }

    return new KBSymbolicType(id, values, desc);
}

KBFuzzyType::KBFuzzyType(const string id, const vector<MembershipFunction*> &membership_functions, const char *desc)
    : KBType(id, desc) {
    this->membership_functions = {};
    for (auto mf : membership_functions) {
        MembershipFunction* new_mf = MembershipFunction::fromJSON(mf->toJSON());
        new_mf->owner = this;
        this->membership_functions.push_back(new_mf);
    }
}

string KBFuzzyType::getInnerKRL() const {
    int mf_size = membership_functions.size();
    int i = 0;

    string innerKRL = to_string(mf_size) + "\n";
    for (auto mf : membership_functions) {
        innerKRL += mf->KRL();
        if (mf_size > 0 && i < mf_size - 1) {
            innerKRL += "\n";
        }
        i++;
    }
    return innerKRL;
}

vector<xmlNodePtr> KBFuzzyType::getInnerXML() const {
    vector<xmlNodePtr> innerXML;
    for (auto mf : membership_functions) {
        auto mfNodes = mf->getInnerXML();
        innerXML.insert(innerXML.end(), mfNodes.begin(), mfNodes.end());
    }
    return innerXML;
}

Json::Value KBFuzzyType::toJSON() const {
    Json::Value json = KBType::toJSON();
    json["membership_functions"] = Json::Value(Json::arrayValue);
    for (const auto& mf : membership_functions) {
        json["membership_functions"].append(mf->toJSON());
    }
    return json;
}

KBFuzzyType::~KBFuzzyType() {
    for (auto mf : membership_functions) {
        delete mf;
    }
    KBEntity::~KBEntity();
}

