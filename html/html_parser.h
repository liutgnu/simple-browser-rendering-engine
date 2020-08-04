#pragma once
#include <string>
#include <vector>
#include "../utils/dom.h"
#include "../utils/base_parser.h"

using namespace std;
using namespace simple_browser;

namespace simple_browser_html {

class HtmlParser: public BaseParser {

    public:
    HtmlParser(string& source): BaseParser(source) {}

    vector<DomNode> parse_dom_nodes() {
        vector<DomNode> child_list;
        for(;;) {
            advance_position_loop(is_blank);
            if (eof() || starts_with_string("</")) {
                break;
            }
            child_list.push_back(parse_dom_node());
        }
        return child_list;
    }
    
    DomNode parse_dom_node() {
        if (starts_with_string("<")) {
            return element_node();
        } else {
            return text_node();
        }
    }

    DomNode element_node() {
        advance_position_string("<");
        string tag_name = skip_blank_and_consume_position(is_char);
        map<string, string> attributes_map = parse_attributes();
        advance_position_string(">");
        
        vector<DomNode> child_list = parse_dom_nodes();
        advance_position_string("</" + tag_name + ">");

        return DomNode(tag_name, attributes_map, child_list);
    }

    map<string, string> parse_attributes() {
        map<string, string> ret;
        for (;;) {
            advance_position_loop(is_blank);
            if (starts_with_string(">")) {
                break;
            }
            string attri_name = skip_blank_and_consume_position(is_char);
            skip_blank_and_advance_position_string("=");
            advance_position_loop(is_blank);
            char quote = source[position];
            assert(quote == '\'' || quote == '"');
            skip_blank_and_advance_position_string(string(1, quote));
            string value = consume_position_loop([=](char c) -> bool {
                return (c != quote);
            });
            advance_position_string(string(1, quote));
            ret.insert(make_pair(attri_name, value));
        }
        return ret;
    }

    DomNode text_node() {
        string text = consume_position_loop([](char c) -> bool {
            if (c == '>' || c == '<') {
                return false;
            } else {
                return true;
            }
        });
        return DomNode(text);
    }
};

}