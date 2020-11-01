#pragma once
#include "style_node.h"
#include <vector>
#include <map>

using namespace std;
namespace simple_browser_style {

class StyleDomNodeParser {

    public:
    DomNode domNode;
    vector<Rule> rules;

    StyleDomNodeParser(const DomNode& domNode, const vector<Rule>& rules):
        domNode(domNode), rules(rules) {}

    // vector<StyleDomNode> parse_style_dom_nodes();
    StyleDomNode parse_style_dom_node(const DomNode& domNode, const vector<Rule>& rules,
        map<string, Value> *extra_property_map);
    void print();
};

}