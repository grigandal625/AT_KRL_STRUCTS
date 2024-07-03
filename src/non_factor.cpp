#include "non_factor.h"
#include <stdexcept>
#include "utils.h"

using namespace std;

double num(double v) {
    double i = static_cast<double>(static_cast<int>(v));
    double f = v;
    return (i == f) ? i : f;
}

NonFactor::NonFactor(double belief, double probability, double accuracy) 
    : KBEntity("with"), belief(num(belief)), probability(num(probability)), accuracy(num(accuracy)) {
    this->initialized = (belief != 50.0) || (probability != 100.0) || (accuracy != 0.0);
}

NonFactor* NonFactor::copy() const {
    return new NonFactor(belief, probability, accuracy);
}

map<string, string> NonFactor::getAttrs() const {
    map<string, string> attrs = KBEntity::getAttrs();
    attrs["belief"] = to_string(belief);
    attrs["probability"] = to_string(probability);
    attrs["accuracy"] = to_string(accuracy);
    return attrs;
}

xmlNodePtr NonFactor::toXML() const {
    xmlNodePtr node = xmlNewNode(nullptr, BAD_CAST this->getTag().c_str());
    xmlNewProp(node, BAD_CAST "belief", BAD_CAST doubleToString(belief).c_str());
    xmlNewProp(node, BAD_CAST "probability", BAD_CAST doubleToString(probability).c_str());
    xmlNewProp(node, BAD_CAST "accuracy", BAD_CAST doubleToString(accuracy).c_str());
    return node;
}

Json::Value NonFactor::toJSON() const {
    Json::Value json;
    json["belief"] = belief;
    json["probability"] = probability;
    json["accuracy"] = accuracy;
    return json;
}

NonFactor* NonFactor::fromXML(xmlNodePtr node) {
    if (!node) {
        return new NonFactor();
    }

    double belief = 50.0;
    double probability = 100.0;
    double accuracy = 0.0;

    xmlChar* beliefAttr = xmlGetProp(node, BAD_CAST "belief");
    if (beliefAttr) {
        belief = stod(reinterpret_cast<const char*>(beliefAttr));
        xmlFree(beliefAttr);
    }

    xmlChar* probabilityAttr = xmlGetProp(node, BAD_CAST "probability");
    if (probabilityAttr) {
        probability = stod(reinterpret_cast<const char*>(probabilityAttr));
        xmlFree(probabilityAttr);
    }

    xmlChar* accuracyAttr = xmlGetProp(node, BAD_CAST "accuracy");
    if (accuracyAttr) {
        accuracy = stod(reinterpret_cast<const char*>(accuracyAttr));
        xmlFree(accuracyAttr);
    }

    return new NonFactor(belief, probability, accuracy);
}

NonFactor* NonFactor::fromJSON(const Json::Value& json) {
    if (json.isNull()) {
        return new NonFactor();
    }

    double belief = json.get("belief", 50.0).asDouble();
    double probability = json.get("probability", 100.0).asDouble();
    double accuracy = json.get("accuracy", 0.0).asDouble();

    return new NonFactor(belief, probability, accuracy);
}

bool NonFactor::isDefault() const {
    return belief == 50.0 && probability == 100.0 && accuracy == 0.0;
}

string NonFactor::KRL() const {
    return "УВЕРЕННОСТЬ [" + doubleToString(belief) + "; " + doubleToString(probability) + "] ТОЧНОСТЬ " + doubleToString(accuracy);
}

string NonFactor::getXMLOwnerPath() const {
    return this->owner->getXMLOwnerPath() + "/with";
}
