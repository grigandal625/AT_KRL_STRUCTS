#include "kb_type.h"
#include "kb_entity.h"
#include <string>
#include <map>
#include <vector>
#include <libxml/tree.h>
#include <json/value.h>
#include <sstream>

using namespace std;

// KBType implementation

KBType::KBType(const string id, const char *desc)
    : KBEntity(id, "type", desc) {}

string KBType::KRL() const
{
    stringstream ss;
    ss << "ТИП " << getId() << endl;
    ss << getKRLType() << endl;
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
    // Add other type cases here
    return nullptr;
}

KBType *KBType::fromJSON(const Json::Value &json)
{
    string meta = json["meta"].asString();
    if (meta == "numeric" || meta == "number")
        return KBNumericType::fromJSON(json);
    // Add other type cases here
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

string KBNumericType::KRL() const
{
    stringstream ss;
    ss << KBType::KRL();
    ss << "ОТ " << from << endl;
    ss << "ДО " << to << endl;
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