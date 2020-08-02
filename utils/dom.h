#pragma once
#include <vector>
#include <string>
#include <map>
#include <cstdio>
#include <iostream>

using namespace std;
namespace simple_browser {

enum DomNodeType {
    ELEMENT,
    TEXT
};

class DomNode {

    public:
    DomNodeType type;
    vector<DomNode> child_list;
    string text;
    string tag_name;
    map<string, string> attributes;

    DomNode(){}
    DomNode(string& text): text(text), type(TEXT) {}
    DomNode(string& tag_name, map<string, string>& attributes, vector<DomNode>& child_list):
        tag_name(tag_name), attributes(attributes), type(ELEMENT), child_list(child_list) {}
    
    void print(int32_t depth) {
        for (int32_t i = 0; i < depth; ++i) {
            cout << "  ";
        }

        if (type == TEXT) {
            cout << "<TEXT> " + text << endl;
        } else {
            cout << "<ELEMENT> " + tag_name << endl;
        }
    }
};

void print_dom_node(DomNode& root, int32_t depth) {
    ++depth;
    root.print(depth);
    for (vector<DomNode>::iterator it = root.child_list.begin();
        it != root.child_list.end(); ++it) {
        print_dom_node(*it, depth);
    }
    --depth;
}

void print_dom_list(vector<DomNode> root_list) {
    static int32_t depth = 0;

    for (vector<DomNode>::iterator it = root_list.begin();
        it != root_list.end(); ++it) {
        print_dom_node(*it, depth);
    }
}

}