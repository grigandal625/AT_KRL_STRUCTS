#ifndef KB_REFERENCE_H
#define KB_REFERENCE_H

#include "kb_value.h"
#include "non_factor.h"
#include "kb_entity.h"
#include <libxml/tree.h>
#include <json/json.h>
#include <string>
#include <vector>

using namespace std;

class KBReference : public Evaluatable {
private:
    string id;
    KBReference* ref;

public:
    KBReference(const string& id, KBReference* ref = nullptr, NonFactor* non_factor = nullptr);
    ~KBReference();

    string getId() const { return id; }
    void setId(const string& id) { this->id = id; }

    const KBReference* getRef() const { return ref; }
    void setRef(KBReference* ref) { this->ref = ref; }

    map<string, string> getAttrs() const override;
    vector<xmlNodePtr> getInnerXML() const override;
    Json::Value toJSON() const override;

    static KBReference* fromXML(xmlNodePtr node);
    static KBReference* fromJSON(const Json::Value& json);

    string getInnerKRL() const override;
};

#endif // KB_REFERENCE_H
