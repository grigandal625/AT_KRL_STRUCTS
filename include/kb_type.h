#ifndef KB_TYPE_H
#define KB_TYPE_H

#include "kb_entity.h"
#include <string>
#include <map>
#include <vector>
#include <libxml/tree.h>
#include <json/value.h>

using namespace std;

class KBNumericType;

class KBType : public KBEntity
{
public:
    KBType(const string id, const char *desc = NULL);

    virtual string getMeta() const { return "abstract"; }
    virtual string getKRLType() const { return "АБСТРАКТНЫЙ"; }
    virtual string KRL() const override;
    virtual map<string, string> getAttrs() const override;
    virtual bool validateValue(const string &value) const { return false; }
    virtual string getXMLOwnerPath() const override;

    static KBType *fromXML(xmlNodePtr node);
    static KBType *fromJSON(const Json::Value &json);
};

class KBNumericType : public KBType
{
private:
    double from;
    double to;

public:
    KBNumericType(const string id, double from, double to, const char *desc = NULL);

    string getMeta() const override { return "number"; }
    string getKRLType() const override { return "ЧИСЛО"; }
    string KRL() const override;
    map<string, string> getAttrs() const override;
    vector<xmlNodePtr> getInnerXML() const override;
    bool validateValue(const string &value) const override;

    static KBNumericType *fromXML(xmlNodePtr node);
    static KBNumericType *fromJSON(const Json::Value &json);
};

#endif // KB_TYPE_H