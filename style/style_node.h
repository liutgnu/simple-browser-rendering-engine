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
    vector<StyleDomNode> pp_list;

    public:
    StyleDomNode(const DomNode& dom_node, const vector<Rule>& rules): DomNode(dom_node) {
        trans_style(rules);
    }
    StyleDomNode() {}
    ~StyleDomNode() {}

    bool match_selector(const Selector& selector);
    void trans_style(const vector<Rule>& rules);
    void print_property_map(int32_t depth, bool is_last_child, vector<int32_t>& list);
};

void style_dom_node_print(StyleDomNode& node, bool is_last_child);
}

