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

    bool match_selector(const Selector& selector) {

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
            !find_in_map<string, string, decltype(string_equal)>
                (attributes, "id", selector.id, string_equal)) {
            return false;
        }
        return true;
    }

    void trans_style(const vector<Rule>& rules) {
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

    void print_property_map(int32_t depth, bool is_last_child, vector<int32_t>& list) {

        for (map<string, Value>::iterator it = property_map.begin(); it != property_map.end(); ++it) {
            for (int32_t i = 0; i < depth; ++i) {
                if (in_list(&list, i)) {
                    std::cout << "\xE2\x94\x82 ";
                }
                else 
                    std::cout << "  ";
            }
            cout << it->first << ": " << it->second.to_string() << endl;
        }
    }

};

void style_dom_node_print(StyleDomNode& node, bool is_last_child) {
    static int32_t depth = 0;
    static vector<int32_t> list;

    ++depth;
    node.print(depth, is_last_child, list);
    node.print_property_map(depth, is_last_child, list);

    for (vector<StyleDomNode>::iterator it = node.pp_list.begin();
        it != node.pp_list.end(); ++it) {
        if (it == node.pp_list.begin()) {
            list.push_back(depth);
        }
        if (it == node.pp_list.end() - 1) {
            list.pop_back();
        }
        style_dom_node_print(*it, it == node.pp_list.end() - 1);
    }
    --depth;
    }

}

