#include "kb_entity.h"
#include <string>
#include <map>
#include <vector>
#include <libxml/tree.h>
#include <json/value.h>
#include <cstring>

using namespace std;

KBEntity::KBEntity(const string id, const string tag, const char *desc)
{
    this->id = id;
    this->tag = tag;
    if (desc != NULL)
    {
        this->desc = new char[strlen(desc) + 1];
        strcpy(this->desc, desc);
    }
    else
    {
        this->desc = NULL;
    }
}

map<string, string> KBEntity::getAttrs() const
{
    map<string, string> result;
    result.insert(pair<string, string>("id", this->id));
}

xmlNodePtr KBEntity::toXML() const
{
    map<string, string> attrs = this->getAttrs();
    xmlNodePtr result = xmlNewNode(NULL, BAD_CAST this->tag.c_str());
    for (map<string, string>::iterator it = attrs.begin(); it != attrs.end(); ++it)
    {
        xmlNewProp(result, BAD_CAST it->first.c_str(), BAD_CAST it->second.c_str());
    }
    vector<xmlNodePtr> innerXML = this->getInnerXML();
    for (vector<xmlNodePtr>::iterator it = innerXML.begin(); it != innerXML.end(); ++it)
    {
        xmlAddChild(result, *it);
    }

    return result;
}

vector<xmlNodePtr> KBEntity::getInnerXML() const
{
    return vector<xmlNodePtr>();
}

Json::Value KBEntity::toJSON() const
{
    Json::Value result;
    map<string, string> attrs = this->getAttrs();
    for (map<string, string>::iterator it = attrs.begin(); it != attrs.end(); ++it)
    {
        result[it->first] = it->second;
    }

    result["tag"] = this->getTag();

    return result;
}
