#include "style_node_parser.h"

using namespace std;
namespace simple_browser_style {

vector<StyleDomNode> StyleDomNodeParser::parse_style_dom_nodes() {
    vector<StyleDomNode> ret;
    for (int i = 0; i < domNodes.size(); ++i) {
        ret.push_back(parse_style_dom_node(domNodes[i], rules));
    }
    return ret;
}

StyleDomNode StyleDomNodeParser::parse_style_dom_node(const DomNode& domNode, const vector<Rule>& rules) {
    StyleDomNode styleDomNode(domNode, rules);
    for (int i = 0; i < styleDomNode.child_list.size(); ++i) {
        styleDomNode.pp_list.push_back(parse_style_dom_node(domNode.child_list[i], rules));
    }
    return styleDomNode;
}

void StyleDomNodeParser::print() {
    vector<StyleDomNode> nodes = parse_style_dom_nodes();

    for (vector<StyleDomNode>::iterator it = nodes.begin();
        it != nodes.end(); ++it) {
        style_dom_node_print(*it, it == nodes.end() - 1);
    }
}

}