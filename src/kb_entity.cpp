#include "kb_entity.h"
#include <string>
#include <map>
#include <vector>
#include <libxml/tree.h>
#include <json/value.h>
#include <cstring>

using namespace std;

KBEntity::KBEntity(const string tag) : tag(tag) {};


map<string, string> KBEntity::getAttrs() const
{
    map<string, string> result;
    return result;
}

xmlNodePtr KBEntity::toXML() const
{
    map<string, string> attrs = this->getAttrs();
    xmlNodePtr result = xmlNewNode(nullptr, BAD_CAST this->tag.c_str());
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


KBIdentity::KBIdentity(const string id, const string tag, const char* desc) : KBEntity(tag), id(id) {
    if (desc != nullptr)
    {
        this->desc = new char[strlen(desc) + 1];
        strcpy(this->desc, desc);
    }
    else
    {
        this->desc = nullptr;
    }
}

void KBIdentity::setDesc(const char* desc) {
    if (desc != nullptr)
    {
        this->desc = new char[strlen(desc) + 1];
        strcpy(this->desc, desc);
    }
    else
    {
        this->desc = nullptr;
    }
}

map<string, string> KBIdentity::getAttrs() const {
    map<string, string> result = KBEntity::getAttrs();
    result["id"] = this->id;
    if (this->desc!= nullptr) {
        result["desc"] = this->desc;
    }
    return result;
}