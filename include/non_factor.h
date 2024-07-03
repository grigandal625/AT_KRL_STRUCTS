#ifndef NON_FACTOR_H
#define NON_FACTOR_H

#include "kb_entity.h"
#include <libxml/tree.h>
#include <json/json.h>
#include <string>
#include <map>

using namespace std;

double num(double v);

class NonFactor : public KBEntity {
private:
    double belief;
    double probability;
    double accuracy;
    bool initialized;

public:
    NonFactor(double belief = 50.0, double probability = 100.0, double accuracy = 0.0);
    virtual ~NonFactor() = default;
    NonFactor* copy() const;
    double getBelief() const { return belief; }
    double getProbability() const { return probability; }
    double getAccuracy() const { return accuracy; }

    map<string, string> getAttrs() const override;
    xmlNodePtr toXML() const override;
    Json::Value toJSON() const;

    static NonFactor* fromXML(xmlNodePtr node);
    static NonFactor* fromJSON(const Json::Value& json);

    bool isDefault() const;
    bool isInitialized() const { return this->initialized; };
    string KRL() const override;
    string getXMLOwnerPath() const override;
};

#endif // NON_FACTOR_H
