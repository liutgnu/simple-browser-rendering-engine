#pragma once
#include <vector>
#include "../html/dom.h"
#include "../css/css_element.h"
#include <algorithm>
#include <map>

using namespace std;
using namespace simple_browser_html;
using namespace simple_browser_css;

namespace simple_browser_style {

class StyleDomNode: public DomNode {
    
    public:
    map<string, Value> property_map;
    vector<StyleDomNode> child_sdn_list;

    public:
    StyleDomNode(const DomNode& dom_node, const vector<Rule>& rules, 
        map<string, Value> *extra_property_map): DomNode(dom_node) {
        if (extra_property_map) {
            property_map.insert(extra_property_map->begin(), extra_property_map->end());
        }
        trans_style(rules);
    }
    StyleDomNode() {}
    ~StyleDomNode() {}

    bool match_selector(const Selector& selector, int& weight);
    void trans_style(const vector<Rule>& rules);
    void print_property_map(int32_t depth, bool is_last_child, vector<int32_t>& list);
    string display() const;
};

void style_dom_node_print(StyleDomNode& node, bool is_last_child);
}

