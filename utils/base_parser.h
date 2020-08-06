#pragma once
#include <string>
#include <vector>
#include <assert.h>
#include "checkers.h"

using namespace std;

namespace simple_browser {

class BaseParser {
    public:
    string source;
    int32_t position;

    public:
    BaseParser(const string& source): source(source), position(0) {}

    bool eof() {
        return position >= source.length();
    }

    bool starts_with_string(string str) {
        return source.substr(position, str.length()) == str;
    }

    template <typename F>
    bool starts_with_char_predicate(const F& f) {
        return f(source[position]);
    }

    /****************************************************************/
    template <typename F>
    void advance_position_loop(const F& f) {
        while (position < source.length() 
            && f(source[position])) {
            ++position;
        }
    };

    template <typename F>
    string consume_position_loop(const F& f) {
        string ret;
        while (position < source.length()
            && f(source[position])) {
            ret.push_back(source[position]);
            ++position;
        }
        return ret;
    }

    void advance_position_string(string str) {
        assert(str == source.substr(position, str.length()));
        position += str.length();
    }

    /****************************************************************/
    template <typename F>
    string skip_blank_and_consume_position(const F& f) {
        advance_position_loop(is_blank);
        return consume_position_loop(f);
    }

    void skip_blank_and_advance_position_string(string str) {
        advance_position_loop(is_blank);
        advance_position_string(str);
    }
};

}