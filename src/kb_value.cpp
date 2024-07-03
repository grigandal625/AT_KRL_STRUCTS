#include "kb_value.h"
#include "non_factor.h"
#include <libxml/parser.h>
#include <json/json.h>
#include <regex>
#include "utils.h"

using namespace std;

Evaluatable::Evaluatable(NonFactor *nonFactor)
    : KBEntity("evaluatable"),
      nonFactor(nonFactor != nullptr ? new NonFactor(
                                           nonFactor->getBelief(),
                                           nonFactor->getProbability(),
                                           nonFactor->getAccuracy())
                                     : new NonFactor()),
      convertNonFactor(nonFactor != nullptr)
{
    this->nonFactor->owner = this;
}

Evaluatable::~Evaluatable()
{
    if (nonFactor)
    {
        delete nonFactor;
    }
}

map<string, string> Evaluatable::getAttrs() const
{
    map<string, string> attrs = KBEntity::getAttrs();
    if (nonFactor && (nonFactor->isInitialized() || convertNonFactor && !nonFactor->isInitialized()))
    {
        attrs["non_factor"] = nonFactor->toJSON().toStyledString();
    }
    return attrs;
}

xmlNodePtr Evaluatable::toXML() const
{
    xmlNodePtr node = KBEntity::toXML();
    if (nonFactor && (nonFactor->isInitialized() || convertNonFactor && !nonFactor->isInitialized()))
    {
        xmlAddChild(node, nonFactor->toXML());
    }
    return node;
}

Evaluatable *Evaluatable::fromXML(xmlNodePtr node)
{
    // Implement the logic to parse XML node and return appropriate subclass instance
    // Raise an error for unknown tags
    // Similar to the Python code provided
    // This part is left unimplemented intentionally
}

Evaluatable *Evaluatable::fromJSON(const Json::Value &json)
{
    // Implement the logic to parse JSON and return appropriate subclass instance
    // Raise an error for unknown tags
    // This part is left unimplemented intentionally
}

string Evaluatable::KRL() const
{
    string result = getInnerKRL();
    if (nonFactor && (nonFactor->isInitialized() || convertNonFactor && !nonFactor->isInitialized()) && !nonFactor->isDefault())
    {
        result += " " + nonFactor->KRL();
    }
    return result;
}

KBValue::KBValue(NonFactor *nonFactor) : Evaluatable(nonFactor)
{
    this->setTag("value");
}

KBValue *KBValue::fromXML(xmlNodePtr node)
{
    xmlChar *content = xmlNodeGetContent(node);
    string contentStr = reinterpret_cast<const char *>(content);
    xmlFree(content);

    // Check if content is boolean
    if (contentStr == "True" || contentStr == "False")
    {
        bool value = (contentStr == "True");
        return new KBBooleanValue(value);
    }

    // Check if content is numeric
    std::regex numeric_regex("^[-+]?[0-9]*\\.?[0-9]+$");
    if (std::regex_match(contentStr, numeric_regex))
    {
        double value = std::stod(contentStr);
        return new KBNumericValue(value);
    }

    // Otherwise, it is symbolic
    return new KBSymbolicValue(contentStr);
}

KBValue *KBValue::fromJSON(const Json::Value &json)
{
    if (!json.isMember("content"))
    {
        throw std::invalid_argument("JSON must contain a 'content' field");
    }

    const Json::Value &content = json["content"];
    NonFactor *nonFactor = nullptr;
    if (json.isMember("non_factor"))
    {
        nonFactor = NonFactor::fromJSON(json["non_factor"]);
    }

    if (content.isString())
    {
        return new KBSymbolicValue(content.asString(), nonFactor);
    }
    else if (content.isBool())
    {
        return new KBBooleanValue(content.asBool(), nonFactor);
    }
    else if (content.isDouble() || content.isInt())
    {
        return new KBNumericValue(content.asDouble(), nonFactor);
    }
    else
    {
        throw std::invalid_argument("Unknown content type in JSON");
    }
}

KBSymbolicValue::KBSymbolicValue(const string &content, NonFactor *nonFactor)
    : KBValue(nonFactor), content(content) {}

string KBSymbolicValue::getInnerKRL() const
{
    return "\"" + content + "\"";
}

vector<xmlNodePtr> KBSymbolicValue::getInnerXML() const
{
    vector<xmlNodePtr> nodes;
    xmlNodePtr textNode = xmlNewText(reinterpret_cast<const xmlChar *>(content.c_str()));
    nodes.push_back(textNode);
    return nodes;
}

KBValue *KBSymbolicValue::evaluate()
{
    return new KBSymbolicValue(content);
}

KBNumericValue::KBNumericValue(double content, NonFactor *nonFactor)
    : KBValue(nonFactor), content(content) {}

string KBNumericValue::getInnerKRL() const
{
    return doubleToString(content);
}

vector<xmlNodePtr> KBNumericValue::getInnerXML() const
{
    vector<xmlNodePtr> nodes;
    string contentStr = doubleToString(content);
    xmlNodePtr textNode = xmlNewText(reinterpret_cast<const xmlChar *>(contentStr.c_str()));
    nodes.push_back(textNode);
    return nodes;
}

KBValue *KBNumericValue::evaluate()
{
    return new KBNumericValue(content);
}

KBBooleanValue::KBBooleanValue(bool content, NonFactor *nonFactor)
    : KBValue(nonFactor), content(content) {}

string KBBooleanValue::getInnerKRL() const
{
    return content ? "true" : "false";
}

vector<xmlNodePtr> KBBooleanValue::getInnerXML() const
{
    vector<xmlNodePtr> nodes;
    string contentStr = content ? "True" : "False";
    xmlNodePtr textNode = xmlNewText(reinterpret_cast<const xmlChar *>(contentStr.c_str()));
    nodes.push_back(textNode);
    return nodes;
}

KBValue *KBBooleanValue::evaluate()
{
    return new KBBooleanValue(content);
}
