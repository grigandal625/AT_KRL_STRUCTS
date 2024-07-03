#include "kb_reference.h"
#include <stdexcept>

using namespace std;

KBReference::KBReference(const string &id, KBReference *ref, NonFactor *non_factor)
    : Evaluatable(non_factor), id(id), ref(ref)
{
    if (this->ref)
    {
        this->ref->owner = this;
    }
    this->setTag("ref");
}

KBReference::~KBReference()
{
    delete ref;
}

map<string, string> KBReference::getAttrs() const
{
    map<string, string> attrs = Evaluatable::getAttrs();
    attrs["id"] = id;
    return attrs;
}

vector<xmlNodePtr> KBReference::getInnerXML() const
{
    vector<xmlNodePtr> innerXML = {};
    if (ref)
    {
        innerXML.push_back(ref->toXML());
    }
    return innerXML;
}

Json::Value KBReference::toJSON() const
{
    Json::Value json;
    json["id"] = id;
    if (ref)
    {
        json["ref"] = ref->toJSON();
    }
    if (this->getNonFactor())
    {
        json["non_factor"] = getNonFactor()->toJSON();
    }
    return json;
}

KBReference *KBReference::fromXML(xmlNodePtr node)
{
    if (!node)
    {
        throw runtime_error("Invalid XML node");
    }

    string id = (const char *)xmlGetProp(node, BAD_CAST "id");
    xmlNodePtr refNode = xmlFirstElementChild(node);
    KBReference *ref = nullptr;
    NonFactor *non_factor = nullptr;
    if (refNode)
    {
        string refNodeName = (const char *)refNode->name;
        if (refNodeName == "with")
        {
            non_factor = NonFactor::fromXML(refNode);
        }
        else if (refNodeName == "ref")
        {
            ref = fromXML(refNode);
            if (refNode->next)
            {
                string nextName = (const char *)refNode->next->name;
                if (nextName == "with")
                {
                    non_factor = NonFactor::fromXML(refNode);
                }
            }
        }
    }
    return new KBReference(id, ref, non_factor);
}

KBReference *KBReference::fromJSON(const Json::Value &json)
{
    if (json.isNull())
    {
        throw runtime_error("Invalid JSON value");
    }

    string id = json["id"].asString();
    KBReference *ref = json.isMember("ref") ? fromJSON(json["ref"]) : nullptr;
    NonFactor *non_factor = json.isMember("non_factor") ? NonFactor::fromJSON(json["non_factor"]) : nullptr;

    return new KBReference(id, ref, non_factor);
}

string KBReference::getInnerKRL() const
{
    string result = id;
    const KBReference *currentRef = ref;
    while (currentRef)
    {
        result += "." + currentRef->getId();
        currentRef = currentRef->getRef();
    }
    return result;
}
