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

struct Declaration {
    string name;
    ValueType type;

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

    Declaration(const string& name, ValueType type, const string& kwd): 
        name(name), type(type) {
        Keyword.keyword = kwd;
    }
    Declaration(const string& name, ValueType type, const tuple<float, string>& length):
        name(name), type(type) {
        tie(Length.data, Length.unit) = length;
    }
    Declaration(const string& name, ValueType type, const tuple<uint8_t, uint8_t, uint8_t, uint8_t>& color):
        name(name), type(type) {
        tie(Color.r, Color.g, Color.b, Color.a) = color;
    }
    Declaration(const Declaration& de) {
        switch (de.type) {
            case KEYWORD:
                new(this) Declaration(de.name, de.type, de.Keyword.keyword);
                return;
            case LENGTH:
                new(this) Declaration(de.name, de.type, make_tuple(de.Length.data, de.Length.unit));
                return;
            case COLOR:
                new(this) Declaration(de.name, de.type, make_tuple(de.Color.r, 
                    de.Color.g, de.Color.b, de.Color.a));
                return;
            default:
                cout << "error declaration type" << endl;
                exit(-1);
        }
    }
    ~Declaration() {}

    string to_string() {
        return name;
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