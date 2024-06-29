#ifndef KB_TYPE_H
#define KB_TYPE_H

#include "kb_entity.h"
#include "membership_function.h"
#include <string>
#include <vector>
#include <map>
#include <libxml/tree.h>
#include <json/value.h>

using namespace std;

class KBType : public KBIdentity
{
public:
    KBType(const string id, const char *desc = nullptr);
    virtual string getMeta() const { return "abstract"; }
    virtual string getKRLType() const { return "АБСТРАКТНЫЙ"; }
    virtual string getInnerKRL() const { return ""; }
    virtual map<string, string> getAttrs() const override;
    virtual vector<xmlNodePtr> getInnerXML() const override { return {}; }
    virtual Json::Value toJSON() const override { return KBEntity::toJSON(); };
    virtual string KRL() const override;
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
    KBNumericType(const string id, double from, double to, const char *desc = nullptr);
    string getMeta() const override { return "number"; }
    string getKRLType() const override { return "ЧИСЛО"; }
    string getInnerKRL() const override;
    double getFrom() const { return from; };
    double getTo() const { return to; };
    map<string, string> getAttrs() const override;
    vector<xmlNodePtr> getInnerXML() const override;
    Json::Value toJSON() const override;

    bool validateValue(const string &value) const;

    static KBNumericType *fromXML(xmlNodePtr node);
    static KBNumericType *fromJSON(const Json::Value &json);
};

class KBSymbolicType : public KBType
{
private:
    vector<string> values;

public:
    KBSymbolicType(const string id, const vector<string> &values, const char *desc = nullptr);
    string getMeta() const override { return "string"; }
    string getKRLType() const override { return "СИМВОЛ"; }
    string getInnerKRL() const override;
    vector<string> getValues() const
    {
        vector<string> vs = {};
        for (string s : values)
        {
            vs.push_back(s);
        }
        return vs;
    };
    map<string, string> getAttrs() const override;
    vector<xmlNodePtr> getInnerXML() const override;
    Json::Value toJSON() const override;

    bool validateValue(const string &value) const;

    static KBSymbolicType *fromXML(xmlNodePtr node);
    static KBSymbolicType *fromJSON(const Json::Value &json);
};

class KBFuzzyType : public KBType
{
private:
    vector<MembershipFunction *> membership_functions;

public:
    KBFuzzyType(const string id, const vector<MembershipFunction*> &membership_functions, const char *desc = nullptr);
    string getMeta() const override { return "fuzzy"; }
    string getKRLType() const override { return "НЕЧЕТКИЙ"; }
    string getInnerKRL() const;
    vector<MembershipFunction *> getMembershipFunctions() const
    {
        vector<MembershipFunction *> mfs = {};
        for (MembershipFunction *mf : membership_functions)
        {
            MembershipFunction *new_mf = MembershipFunction::fromJSON(mf->toJSON());
            mfs.push_back(new_mf);
        }
        return mfs;
    };
    vector<xmlNodePtr> getInnerXML() const;
    Json::Value toJSON() const;
    ~KBFuzzyType() override;
};

#endif // KB_TYPE_H