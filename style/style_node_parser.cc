#include "style_node_parser.h"

using namespace std;
namespace simple_browser_style {

StyleDomNode StyleDomNodeParser::parse_style_dom_node(const DomNode& domNode, const vector<Rule>& rules) {
    StyleDomNode styleDomNode(domNode, rules);
    for (int i = 0; i < styleDomNode.child_list.size(); ++i) {
        styleDomNode.child_sdn_list.push_back(parse_style_dom_node(domNode.child_list[i], rules));
    }
    return styleDomNode;
}

void StyleDomNodeParser::print() {
    StyleDomNode node = parse_style_dom_node(domNode, rules);
    style_dom_node_print(node, true);
}

}