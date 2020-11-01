#include "style_node_parser.h"

using namespace std;
namespace simple_browser_style {

StyleDomNode StyleDomNodeParser::parse_style_dom_node(const DomNode& domNode, const vector<Rule>& rules,
    map<string, Value> *extra_property_map) {
    StyleDomNode styleDomNode(domNode, rules, extra_property_map);
    for (int i = 0; i < styleDomNode.child_list.size(); ++i) {
        // styleDomNode.child_sdn_list.push_back(parse_style_dom_node(domNode.child_list[i], rules));
        if (styleDomNode.child_list[i].type == TEXT) {
            map<string, Value> extra;

            vector<string> font_attribute = {
                "color",
                "font-size",
            };
            for (auto it = font_attribute.cbegin(); it != font_attribute.cend(); ++it) {
                if (const Value *tmp = find_in_map(styleDomNode.property_map, *it)) {
                    extra.insert(make_pair(*it, *tmp));
                }
            }
            styleDomNode.child_sdn_list.push_back(parse_style_dom_node(domNode.child_list[i], rules, &extra));
        } else {
            styleDomNode.child_sdn_list.push_back(parse_style_dom_node(domNode.child_list[i], rules, nullptr));
        }
    }
    return styleDomNode;
}

void StyleDomNodeParser::print() {
    StyleDomNode node = parse_style_dom_node(domNode, rules, nullptr);
    style_dom_node_print(node, true);
}

}