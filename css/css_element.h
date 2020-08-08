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

    string to_string() {
        string ret;
        if (id.length() > 0) {
            ret += "#" + id + " ";
        }
        if (tag.length() > 0) {
            ret += tag + " ";
        }
        for (int i = 0; i < class_list.size(); ++i) {
            ret += "." + class_list[i] + " ";
        }
        ret.resize(ret.length() - 1);
        return ret;
    }
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

    Value(const string& kwd): type(KEYWORD) {
        new(&this->Keyword.keyword) string(kwd);
    }
    Value(const tuple<float, string>& length): type(LENGTH) {
        Length.data = get<0>(length);
        new(&this->Length.unit) string(get<1>(length));
    }
    Value(const tuple<uint8_t, uint8_t, uint8_t, uint8_t>& color): type(COLOR) {
        tie(Color.r, Color.g, Color.b, Color.a) = color;
    }
    Value(const Value& v) {
        switch (v.type) {
            case KEYWORD:
                new(this) Value(v.Keyword.keyword);
                return;
            case LENGTH:
                new(this) Value(make_tuple(v.Length.data, v.Length.unit));
                return;
            case COLOR:
                new(this) Value(make_tuple(v.Color.r, 
                    v.Color.g, v.Color.b, v.Color.a));
                return;
            default:
                cout << "error value type" << endl;
                exit(-1);
        }
    }
    Value() {}
    ~Value() {}

    string to_string() {
        switch(type) {
            case KEYWORD: return Keyword.keyword;
            case LENGTH: return std::to_string(Length.data) + Length.unit;
            case COLOR: return std::to_string(Color.r) + " " +
                                std::to_string(Color.g) + " " +
                                std::to_string(Color.b) + " " +
                                std::to_string(Color.a);
        }
    }
};

struct Declaration {
    string name;
    struct Value value;

    Declaration(const string& name, const string& kwd): 
        name(name), value(kwd) {}
    Declaration(const string& name, const tuple<float, string>& length):
        name(name), value(length) {}
    Declaration(const string& name, const tuple<uint8_t, uint8_t, uint8_t, uint8_t>& color):
        name(name), value(color) {}
    Declaration(const Declaration& de):name(de.name), value(de.value) {}

    string to_string() {
        return name + ": " + value.to_string();
    }
};

struct Rule {
    vector<Selector> selectors;
    vector<Declaration> declarations;
    Rule(vector<Selector> selectors, vector<Declaration> declarations):
        selectors(selectors), declarations(declarations) {}

    void print(bool is_last_rule) {
        for (int i = 0; i < selectors.size(); ++i) {
            if (is_last_rule && i ==  selectors.size() - 1) {
                cout << "\xE2\x94\x94\xE2\x94\x80" << selectors[i].to_string() << endl;
            } else {
                cout << "\xE2\x94\x9c\xE2\x94\x80" << selectors[i].to_string() << endl;
            }
            for (int i = 0; i < declarations.size(); ++i) {
                if (i != declarations.size() - 1) 
                    cout << (is_last_rule ? " " : "\xE2\x94\x82") << "  \xE2\x94\x9c\xE2\x94\x80" << declarations[i].to_string() << endl;
                else
                    cout << (is_last_rule ? " " : "\xE2\x94\x82") << "  \xE2\x94\x94\xE2\x94\x80" << declarations[i].to_string() << endl;
            }
        }
    }
};

}