#ifndef KB_VALUE_H
#define KB_VALUE_H

#include "kb_entity.h"
#include "non_factor.h"
#include <libxml/tree.h>
#include <json/json.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <regex>
#include "utils.h"

using namespace std;

class Evaluatable : public KBEntity {
protected:
    NonFactor* nonFactor;
    bool convertNonFactor;

public:
    Evaluatable(NonFactor* nonFactor = nullptr);

    bool getConvertNonFactor() const { return convertNonFactor; };
    void setConvertNonFactor(bool convertNonFactor) { this->convertNonFactor = convertNonFactor; };
    
    NonFactor* getNonFactor() const { return nonFactor; };
    void setNonFactor(NonFactor* nonFactor) { this->nonFactor = nonFactor->copy(); };
    virtual ~Evaluatable();

    virtual map<string, string> getAttrs() const override;
    virtual xmlNodePtr toXML() const override;

    static Evaluatable* fromXML(xmlNodePtr node);
    static Evaluatable* fromJSON(const Json::Value& json);

    virtual string getInnerKRL() const = 0;
    virtual string KRL() const override;
};

class KBValue : public Evaluatable {
public:
    KBValue(NonFactor* nonFactor = nullptr);

    static KBValue* fromXML(xmlNodePtr node);
    static KBValue* fromJSON(const Json::Value& json);

    virtual KBValue* evaluate() = 0;
    virtual vector<xmlNodePtr> getInnerXML() const override = 0;
    virtual string getContentAsString() const = 0;
    virtual void setContent(const string& value) = 0;
    virtual void setContent(double value) = 0;
    virtual void setContent(bool value) = 0;

    template <typename T>
    T getContent() const;
};

class KBSymbolicValue : public KBValue {
private:
    string content;

public:
    KBSymbolicValue(const string& content, NonFactor* nonFactor = nullptr);

    string getInnerKRL() const override;
    vector<xmlNodePtr> getInnerXML() const override;
    KBValue* evaluate() override;

    string getContentAsString() const override { return content; }
    void setContent(const string& value) override { content = value; }
    void setContent(double value) override { throw invalid_argument("Invalid type for KBSymbolicValue"); }
    void setContent(bool value) override { throw invalid_argument("Invalid type for KBSymbolicValue"); }
};

class KBNumericValue : public KBValue {
private:
    double content;

public:
    KBNumericValue(double content, NonFactor* nonFactor = nullptr);

    string getInnerKRL() const override;
    vector<xmlNodePtr> getInnerXML() const override;
    KBValue* evaluate() override;

    double getContent() const { 
        return content; 
    }
    string getContentAsString() const override { return doubleToString(content); }
    void setContent(const string& value) override { content = stod(value); }
    void setContent(double value) override { content = value; }
    void setContent(bool value) override { throw invalid_argument("Invalid type for KBNumericValue"); }
};

class KBBooleanValue : public KBValue {
private:
    bool content;

public:
    KBBooleanValue(bool content, NonFactor* nonFactor = nullptr);

    string getInnerKRL() const override;
    vector<xmlNodePtr> getInnerXML() const override;
    KBValue* evaluate() override;

    bool getContent() const { return content; }
    string getContentAsString() const override { return content ? "true" : "false"; }
    void setContent(const string& value) override { content = (value == "True" || value == "true"); }
    void setContent(double value) override { throw invalid_argument("Invalid type for KBBooleanValue"); }
    void setContent(bool value) override { content = value; }
};

// Template specializations
template <>
inline string KBValue::getContent<string>() const {
    return getContentAsString();
}

template <>
inline double KBValue::getContent<double>() const {
    const KBNumericValue* derived = dynamic_cast<const KBNumericValue*>(this);
    if (!derived) {
        throw bad_cast();
    }
    return derived->getContent();
}

template <>
inline bool KBValue::getContent<bool>() const {
    const KBBooleanValue* derived = dynamic_cast<const KBBooleanValue*>(this);
    if (!derived) {
        throw bad_cast();
    }
    return derived->getContent();
}

#endif // KB_VALUE_H
