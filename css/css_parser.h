#pragma once
#include "../utils/base_parser.h"
#include "css_element.h"
#include <iostream>

using namespace simple_browser;
using namespace std;

namespace simple_browser_css {

class CssParser: public BaseParser {

    public:
    CssParser(string& source): BaseParser(source) {}

    vector<Rule> parse_css_rules() {
        vector<Rule> child_list;
        for(;;) {
            advance_position_loop(is_blank);
            if (eof()) {
                break;
            }
            child_list.push_back(parse_css_rule());
        }
        return child_list;
    }

    Rule parse_css_rule() {
        vector<Selector> selectors = parse_selectors();
        skip_blank_and_advance_position_string("{");
        vector<Declaration> declarations = parse_declarations();
        skip_blank_and_advance_position_string("}");
        return Rule(selectors, declarations);
    }

    vector<Selector> parse_selectors() {
        vector<Selector> list;
        for(;;) {
            advance_position_loop(is_blank);
            if (starts_with_string(",")) {
                advance_position_string(",");
            }
            if (eof() || starts_with_string("{"))
                break;
            list.push_back(parse_selector());
        }
        return list;
    }

    Selector parse_selector() {
        Selector ret;
        for (;;) {
            advance_position_loop(is_blank);
            if (source[position] == '.') {
                advance_position_string(".");
                ret.class_list.push_back(consume_position_loop(is_identifier));
            } else if (source[position] == '#') {
                advance_position_string("#");
                ret.id = consume_position_loop(is_identifier);
            } else if (is_char(source[position])) {
                ret.tag = consume_position_loop(is_identifier);
            } else if (source[position] == ',' || source[position] == '{') {
                break;
            } else {
                cout << "unsupport selector " << source[position] << endl;
                exit(-1);
            }
        }
        return ret;
    }

    vector<Declaration> parse_declarations() {
        vector<Declaration> ret;
        for (;;) {
            advance_position_loop(is_blank);
            if (starts_with_string("}")) {
                break;
            }
            ret.push_back(parse_declaration());
            skip_blank_and_advance_position_string(";");
        }
        return ret;
    }

    Declaration parse_declaration() {
        string name = skip_blank_and_consume_position(is_identifier);
        advance_position_string(":");
        advance_position_loop(is_blank);
        if (starts_with_string("#")) { // color
            advance_position_string("#");
            string color = consume_position_loop(is_hex);
            assert(color.length() == 3 || color.length() == 6);
            return Declaration(name, COLOR, color_trans(color));
        } else if (starts_with_string("-") || (starts_with_char_predicate(is_num))) { // length
            float data = stof(consume_position_loop([](char c) -> bool {
                return (is_num(c) || c == '.' || c == '-');
            }));
            return Declaration(name, LENGTH, 
                make_tuple(data, skip_blank_and_consume_position(is_char)));
        } else if (starts_with_char_predicate(is_char)) {
            string keyword = consume_position_loop(is_char);
            return Declaration(name, KEYWORD, keyword);
        } else {
            cout << "unknown declaration " << source[position] << endl;
            exit(-1);
        }
    }

    tuple<uint8_t, uint8_t, uint8_t, uint8_t> color_trans(string& hex_color) {
        string color;
        vector<uint8_t> list;
        if (hex_color.length() == 3 ) {
            for(string::iterator it = hex_color.begin(); it != hex_color.end(); ++it) {
                color.push_back(*it);
                color.push_back(*it);
            }
        } else {
            color = hex_color;
        }
        for (string::iterator it = color.begin(); it != color.end(); ++it) {
            if (!((it - color.begin()) % 2)) {
                uint8_t val = (uint8_t)stoul(color.substr(it - color.begin(), 2), nullptr, 16);
                list.push_back(val);
            }
        }
        return make_tuple(list[0], list[1], list[2], 255);
    } 

};
}