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

    vector<Rule> parse_css_rules();
    Rule parse_css_rule();
    vector<Selector> parse_selectors();
    Selector parse_selector();
    vector<Declaration> parse_declarations();
    Declaration parse_declaration();
    tuple<uint8_t, uint8_t, uint8_t, uint8_t> color_trans(string& hex_color);
};
}