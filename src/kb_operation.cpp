#include "kb_operation.h"
#include <stdexcept>
#include <algorithm>

using namespace std;

const map<string, map<string, string>> TAGS_SIGNS = {
    {"eq", {{"values", "== = eq"}, {"is_binary", "true"}, {"convert_non_factor", "true"}, {"meta", "eq"}}},
    {"gt", {{"values", "> gt"}, {"is_binary", "true"}, {"convert_non_factor", "true"}, {"meta", "eq"}}},
    {"ge", {{"values", ">= ge"}, {"is_binary", "true"}, {"convert_non_factor", "true"}, {"meta", "eq"}}},
    {"lt", {{"values", "< lt"}, {"is_binary", "true"}, {"convert_non_factor", "true"}, {"meta", "eq"}}},
    {"le", {{"values", "<= le"}, {"is_binary", "true"}, {"convert_non_factor", "true"}, {"meta", "eq"}}},
    {"ne", {{"values", "!= <> ne"}, {"is_binary", "true"}, {"convert_non_factor", "true"}, {"meta", "eq"}}},
    {"and", {{"values", "&& & and"}, {"is_binary", "true"}, {"meta", "log"}}},
    {"or", {{"values", "|| | or"}, {"is_binary", "true"}, {"meta", "log"}}},
    {"not", {{"values", "! ~ not"}, {"is_binary", "false"}, {"meta", "log"}}},
    {"xor", {{"values", "xor"}, {"is_binary", "true"}, {"meta", "log"}}},
    {"neg", {{"values", "- neg"}, {"is_binary", "false"}, {"meta", "super_math"}}},
    {"add", {{"values", "+ add"}, {"is_binary", "true"}, {"meta", "math"}}},
    {"sub", {{"values", "- sub"}, {"is_binary", "true"}, {"meta", "math"}}},
    {"mul", {{"values", "* mul"}, {"is_binary", "true"}, {"meta", "math"}}},
    {"div", {{"values", "/ div"}, {"is_binary", "true"}, {"meta", "math"}}},
    {"mod", {{"values", "% mod"}, {"is_binary", "true"}, {"meta", "super_math"}}},
    {"pow", {{"values", "^ ** pow"}, {"is_binary", "true"}, {"meta", "super_math"}}},
};

KBOperation::KBOperation(const string &sign, Evaluatable *left, Evaluatable *right, NonFactor *non_factor)
    : Evaluatable(non_factor), left(left), right(right)
{
    auto is_binary = left != nullptr && right != nullptr;

    for (const auto &[op, properties] : TAGS_SIGNS)
    {
        if (properties.at("values").find(sign) != string::npos)
        {
            if (properties.at("is_binary") == (is_binary ? "true" : "false"))
            {
                this->op = op;
                this->setTag(op);
                break;
            }
        }
    }

    if (this->op.empty())
    {
        throw invalid_argument("Unknown operation: " + sign);
    }

    map<string, string> properties = TAGS_SIGNS.at(this->op);
    if (properties.find("convert+_non_factor") != properties.end())
    {
        this->convert_non_factor = properties.at("convert_non_factor") == "true";
    }
    else
    {
        this->convert_non_factor = false;
    }

    if (this->left)
    {
        this->left->owner = this;
    }
    if (this->isBinary() && this->right)
    {
        this->right->owner = this;
    }
}

KBOperation::~KBOperation()
{
    if (left)
    {
        delete left;
    }
    if (right)
    {
        delete right;
    }
}

bool KBOperation::isBinary() const
{
    return TAGS_SIGNS.at(op).at("is_binary") == "true";
}

string KBOperation::getSign() const
{
    return TAGS_SIGNS.at(op).at("values").substr(0, TAGS_SIGNS.at(op).at("values").find(' '));
}

map<string, string> KBOperation::getAttrs() const
{
    map<string, string> attrs = Evaluatable::getAttrs();
    attrs["op"] = op;
    return attrs;
}

vector<xmlNodePtr> KBOperation::getInnerXML() const
{
    vector<xmlNodePtr> innerXML = {left->toXML()};
    if (isBinary() && right)
    {
        innerXML.push_back(right->toXML());
    }
    return innerXML;
}

Json::Value KBOperation::toJSON() const
{
    Json::Value json;
    json["sign"] = getSign();
    json["left"] = left->toJSON();
    if (isBinary() && right)
    {
        json["right"] = right->toJSON();
    }
    if (getNonFactor())
    {
        json["non_factor"] = getNonFactor()->toJSON();
    }
    return json;
}

KBOperation *KBOperation::fromXML(xmlNodePtr node)
{
    if (!node)
    {
        throw runtime_error("Invalid XML node");
    }

    string sign = (const char *)node->name;
    Evaluatable *left = Evaluatable::fromXML(xmlFirstElementChild(node));
    Evaluatable *right = nullptr;
    if (TAGS_SIGNS.at(sign).at("is_binary") == "true")
    {
        right = Evaluatable::fromXML(xmlNextElementSibling(xmlFirstElementChild(node)));
    }
    NonFactor *non_factor = nullptr;
    xmlNodePtr nonFactorNode = xmlFirstElementChild(node);
    if (nonFactorNode && xmlStrEqual(nonFactorNode->name, BAD_CAST "with"))
    {
        non_factor = NonFactor::fromXML(nonFactorNode);
    }

    return new KBOperation(sign, left, right, non_factor);
}

KBOperation *KBOperation::fromJSON(const Json::Value &json)
{
    if (json.isNull())
    {
        throw runtime_error("Invalid JSON value");
    }

    string sign = json["sign"].asString();
    Evaluatable *left = Evaluatable::fromJSON(json["left"]);
    Evaluatable *right = nullptr;
    if (json.isMember("right"))
    {
        right = Evaluatable::fromJSON(json["right"]);
    }
    NonFactor *non_factor = nullptr;
    if (json.isMember("non_factor"))
    {
        non_factor = NonFactor::fromJSON(json["non_factor"]);
    }

    return new KBOperation(sign, left, right, non_factor);
}

string KBOperation::getInnerKRL() const
{
    if (isBinary())
    {
        return "(" + left->KRL() + ") " + getSign() + " (" + right->KRL() + ")";
    }
    else
    {
        return getSign() + " (" + left->KRL() + ")";
    }
}
