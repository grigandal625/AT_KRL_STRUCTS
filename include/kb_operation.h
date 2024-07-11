#ifndef KB_OPERATION_H
#define KB_OPERATION_H

#include "kb_value.h"
#include "non_factor.h"
#include <libxml/tree.h>
#include <json/json.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

extern const map<string, map<string, string>> TAGS_SIGNS;

class KBOperation : public Evaluatable {
private:
    string id;
    Evaluatable* left;
    Evaluatable* right;
    string op;
    bool convert_non_factor;

public:
    KBOperation(const string& sign, Evaluatable* left, Evaluatable* right = nullptr, NonFactor* non_factor = nullptr);
    ~KBOperation();

    string getId() const { return id; }
    void setId(const string& id) { this->id = id; }

    const Evaluatable* getLeft() const { return left; }
    void setLeft(Evaluatable* left) { this->left = left; }

    const Evaluatable* getRight() const { return right; }
    void setRight(Evaluatable* right) { this->right = right; }

    string getOp() const { return op; }
    void setOp(const string& op) { this->op = op; }

    bool isBinary() const;
    string getSign() const;

    map<string, string> getAttrs() const override;
    vector<xmlNodePtr> getInnerXML() const override;
    Json::Value toJSON() const override;

    static KBOperation* fromXML(xmlNodePtr node);
    static KBOperation* fromJSON(const Json::Value& json);

    string getInnerKRL() const override;
};

#endif // KB_OPERATION_H