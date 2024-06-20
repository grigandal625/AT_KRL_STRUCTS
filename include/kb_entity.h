#ifndef KB_ENTITY_H
#define KB_ENTITY_H

#include <string>
#include <map>
#include <vector>
#include <libxml/tree.h>
#include <json/value.h>

using namespace std;

class KnowledgeBase;

class KBEntity
{
private:
    string id;
    char* desc;
    string tag;
    bool validated = false;

public:
    KBEntity(const string id, const string tag, const char* desc);
    string getId() const { return id; };
    string getTag() const { return tag; };
    char* getDesc() const { return desc; };
    virtual string KRL() const;
    virtual map<string, string> getAttrs() const;
    virtual xmlNodePtr toXML() const;
    virtual vector<xmlNodePtr> getInnerXML() const;
    virtual Json::Value toJSON() const;
    virtual bool validate(const KnowledgeBase &kb);
    virtual ~KBEntity();
    virtual string getXMLOwnerPath() const;

    bool isValidated() const { return validated; }
    // static KBEntity fromXML(xmlNodePtr node);
    // static KBEntity fromJSON(const Json::Value &json);


    KBEntity *owner = NULL;
};

#endif // KB_ENTITY_H