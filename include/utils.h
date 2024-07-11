#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <libxml/parser.h>
#include <libxml/tree.h>


inline std::string doubleToString(double value) {
    std::ostringstream oss;
    oss << std::setprecision(std::numeric_limits<double>::max_digits10) << value;
    return oss.str();
}

inline xmlNodePtr parseXmlString(const std::string& xmlString) {
    // Initialize libxml2
    xmlInitParser();

    // Parse the XML string
    xmlDocPtr doc = xmlReadMemory(xmlString.c_str(), xmlString.size(), "noname.xml", NULL, 0);
    if (doc == NULL) {
        throw std::runtime_error("Couldn't parse xml string from " + xmlString);
        return NULL;
    }

    // Get the root element of the document
    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (root == NULL) {
        throw std::runtime_error("Empty document.");
        xmlFreeDoc(doc);
        return NULL;
    }

    // Free the document (caller is responsible for freeing this later)
    // xmlFreeDoc(doc); // Don't free here to allow caller to use the document

    // Cleanup libxml2 (caller is responsible for cleaning up)
    // xmlCleanupParser(); // Don't cleanup here to allow caller to continue using libxml2

    return root;
}

inline xmlNodePtr findNodeByName(xmlNodePtr root, const char* name) {
    for (xmlNodePtr node = root; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE && xmlStrcmp(node->name, (const xmlChar *)name) == 0) {
            return node;
        }
        xmlNodePtr found = findNodeByName(node->children, name);
        if (found) return found;
    }
    return nullptr;
}

#endif // UTILS_H