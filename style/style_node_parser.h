#pragma once
#include "style_node.h"
#include <vector>

using namespace std;
namespace simple_browser_style {

class StyleDomNodeParser {

    public:
    vector<DomNode> domNodes;
    vector<Rule> rules;

    StyleDomNodeParser(const vector<DomNode>& domNodes, const vector<Rule>& rules):
        domNodes(domNodes), rules(rules) {}

    vector<StyleDomNode> parse_style_dom_nodes();
    StyleDomNode parse_style_dom_node(const DomNode& domNode, const vector<Rule>& rules);
    void print();
};

}