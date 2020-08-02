#pragma once
#include <string>
#include <vector>
#include <assert.h>

using namespace std;

namespace simple_browser {

template <typename T>
class BaseParser {
    public:
    string source;
    int32_t position;

    public:
    BaseParser(string& source): source(source), position(0) {}

    // virtual vector<T> parse_dom_nodes();
    // virtual <T> parse_dom_node();

    bool eof() {
        return position >= source.length();
    }

    bool starts_with(string str) {
        return source.substr(position, str.length()) == str;
    }
    
};

auto is_char = [](char c) -> bool {
    return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
};

auto is_num = [](char c) -> bool {
    return (c >= '0') && (c <= '9');
};

auto is_blank = [](char c) -> bool {
    return (c == ' ') || (c == '\t') || (c == '\n');
};

/****************************************************************/
template <typename T, typename F>
void advance_position_loop(BaseParser<T> &parser, const F& f) {
    while (parser.position < parser.source.length() 
        && f(parser.source[parser.position])) {
        ++parser.position;
    }
};

template <typename T, typename F>
string consume_position_loop(BaseParser<T> &parser, const F& f) {
    string ret;
    while (parser.position < parser.source.length()
        && f(parser.source[parser.position])) {
        ret.push_back(parser.source[parser.position]);
        ++parser.position;
    }
    return ret;
}

template <typename T>
void advance_position_string(BaseParser<T> &parser, string str) {
    assert(str == parser.source.substr(parser.position, str.length()));
    parser.position += str.length();
}

/****************************************************************/
template <typename T, typename F>
string skip_blank_and_consume_position(BaseParser<T> &parser, const F& f) {
    advance_position_loop(parser, is_blank);
    return consume_position_loop(parser, f);
}

template <typename T>
void skip_blank_and_advance_position_string(BaseParser<T> &parser, string str) {
    advance_position_loop(parser, is_blank);
    advance_position_string(parser, str);
}

}