#pragma once
#include "../utils/checkers.h"
#include <vector>
#include <string>
#include <tuple>
#include <iostream>

using namespace std;

namespace simple_browser_css {

struct Selector {
    string id;
    string tag;
    vector<string> class_list;

    string to_string();
};

struct Keyword {
    string keyword;
};

struct Length {
    float data;
    string unit;
};

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;    
};

enum ValueType {
    KEYWORD,
    LENGTH,
    COLOR,
};

struct Value {
    ValueType type;    
    union {
        struct {
            string keyword;            
        } Keyword;

        struct {
            float data;
            string unit;
        } Length;

        struct {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;    
        } Color;
    };

    Value(const string& kwd);
    Value(const tuple<float, string>& length);
    Value(const tuple<uint8_t, uint8_t, uint8_t, uint8_t>& color);
    Value(const Value& v);
    Value();
    ~Value();

    string to_string();
};

struct Declaration {
    string name;
    struct Value value;

    Declaration(const string& name, const string& kwd);
    Declaration(const string& name, const tuple<float, string>& length);
    Declaration(const string& name, const tuple<uint8_t, uint8_t, uint8_t, uint8_t>& color);
    Declaration(const Declaration& de);
    string to_string();
};

struct Rule {
    vector<Selector> selectors;
    vector<Declaration> declarations;
    Rule(vector<Selector> selectors, vector<Declaration> declarations):
        selectors(selectors), declarations(declarations) {}

    void print(bool is_last_rule);
};

}