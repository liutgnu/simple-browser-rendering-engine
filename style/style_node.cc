#include "style_node.h"

using namespace std;
using namespace simple_browser_html;
using namespace simple_browser_css;

namespace simple_browser_style {

bool StyleDomNode::match_selector(const Selector& selector) {

    // tag of dom node
    if ((selector.tag != "") && (tag_name != selector.tag)) {
        return false;
    }

    vector<char> delimits;
    delimits.push_back(' ');
    delimits.push_back('\t');

    // class of dom node
    if (selector.class_list.size() > 0) {
        if (any_of(selector.class_list.begin(), selector.class_list.end(), [&](string selc)-> bool {
            auto it = attributes.find("class");
            if (it != attributes.end()) {
                return !in_vector(split_string(it->second, delimits), selc, string_equal);
            }
            return true;
        })) {
            return false;
        }
    }

    //id of dom node
    if (selector.id != "" &&
        !exist_in_map<string, string, decltype(string_equal)>
            (attributes, "id", selector.id, string_equal)) {
        return false;
    }
    return true;
}

void StyleDomNode::trans_style(const vector<Rule>& rules) {
    for (int i = 0; i < rules.size(); ++i) {
        for (int j = 0; j < rules[i].selectors.size(); ++j) {
            if (match_selector(rules[i].selectors[j])) {
                for (int k = 0; k < rules[i].declarations.size(); ++k) {
                    property_map.insert(make_pair(rules[i].declarations[k].name, rules[i].declarations[k].value));
                }
            }
        }
    }
}

void StyleDomNode::print_property_map(int32_t depth, bool is_last_child, vector<int32_t>& list) {

    for (map<string, Value>::iterator it = property_map.begin(); it != property_map.end(); ++it) {
        for (int32_t i = 0; i < depth; ++i) {
            if (in_vector(list, i, [](int32_t a, int32_t b) -> bool { return a == b; })) {
                std::cout << "\xE2\x94\x82 ";
            }
            else 
                std::cout << "  ";
        }
        cout << it->first << ": " << it->second.to_string() << endl;
    }
}

string StyleDomNode::display() const {
    const Value* value = find_in_map(property_map, string("display"));
    if (value) {
        if (value->Keyword.keyword == "block" || value->Keyword.keyword == "none") {
            return value->Keyword.keyword;
        }
    }
    return "inline";
}

void style_dom_node_print(StyleDomNode& node, bool is_last_child) {
    static int32_t depth = 0;
    static vector<int32_t> list;

    ++depth;
    node.print(depth, is_last_child, list);
    node.print_property_map(depth, is_last_child, list);

    for (vector<StyleDomNode>::iterator it = node.child_sdn_list.begin();
        it != node.child_sdn_list.end(); ++it) {
        if (it == node.child_sdn_list.begin()) {
            list.push_back(depth);
        }
        if (it == node.child_sdn_list.end() - 1) {
            list.pop_back();
        }
        style_dom_node_print(*it, it == node.child_sdn_list.end() - 1);
    }
    --depth;
    }

}

