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
    string tag;
    bool validated = false;

public:
    KBEntity(const string tag);
    string getTag() const { return tag; };
    void setTag(const string& tag) { this->tag = tag; };
    virtual string KRL() const { return ""; };
    virtual map<string, string> getAttrs() const;
    virtual xmlNodePtr toXML() const;
    virtual vector<xmlNodePtr> getInnerXML() const;
    virtual Json::Value toJSON() const;
    virtual bool validate(const KnowledgeBase &kb) { return true; };
    virtual ~KBEntity() { };
    virtual string getXMLOwnerPath() const { return ""; };

    bool isValidated() const { return validated; }
    // static KBEntity fromXML(xmlNodePtr node);
    // static KBEntity fromJSON(const Json::Value &json);


    KBEntity *owner = nullptr;
};


class KBIdentity : public KBEntity 
{
private:
    string id;
    char* desc;

public:
    KBIdentity(const string id, const string tag, const char* desc);
    string getId() const { return id; };
    void setId(const string& id) { this->id = id; };

    char* getDesc() const { return desc; };
    void setDesc(const char* desc);
    string getComment() const { return desc != nullptr ? desc : id; };

    virtual ~KBIdentity() {free(desc);}

    virtual map<string, string> getAttrs() const;
};

#endif // KB_ENTITY_H