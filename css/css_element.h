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

    Declaration(string name, ValueType type, string kwd): 
        name(name), type(type) {
        Keyword.keyword = kwd;
    }
    Declaration(string name, ValueType type, tuple<float, string> length):
        name(name), type(type) {
        tie(Length.data, Length.unit) = length;
    }
    Declaration(string name, ValueType type, tuple<uint8_t, uint8_t, uint8_t, uint8_t> color):
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
};

struct Rule {
    vector<Selector> selectors;
    vector<Declaration> declarations;
    Rule(vector<Selector> selectors, vector<Declaration> declarations):
        selectors(selectors), declarations(declarations) {}
};

}