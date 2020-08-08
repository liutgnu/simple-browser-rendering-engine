#pragma once
#include <vector>
#include <string>
#include <map>
#include <cstdio>
#include <iostream>

using namespace std;
namespace simple_browser_html {

enum DomNodeType {
    ELEMENT,
    TEXT
};

bool in_list(vector<int> *list, int val) {
    for (vector<int>::iterator it = list->begin(); it != list->end(); ++it) {
        if (val == *it)
            return true;
    }
    return false;
}

class DomNode {

    public:
    DomNodeType type;
    vector<DomNode> child_list;
    string text;
    string tag_name;
    map<string, string> attributes;

    DomNode(){}
    DomNode(const string& text): text(text), type(TEXT) {}
    DomNode(const string& tag_name, const map<string, string>& attributes, const vector<DomNode>& child_list):
        tag_name(tag_name), attributes(attributes), type(ELEMENT), child_list(child_list) {}
    DomNode(const DomNode& dn) {
        switch (dn.type) {
            case TEXT:
                new(this) DomNode(dn.text);
                return;
            case ELEMENT:
                new(this) DomNode(dn.tag_name, dn.attributes, dn.child_list);
                return;
        }
    }
    ~DomNode() {}
    
    void print(int32_t depth, bool is_last_child, vector<int32_t>& list) {
        for (int32_t i = 0; i < depth; ++i) {
            if (is_last_child && i == depth - 1) {
                std::cout << "\xE2\x94\x94\xE2\x94\x80";
            } else if (!is_last_child && i == depth - 1) {
                std::cout << "\xE2\x94\x9c\xE2\x94\x80";
            }
            else if (in_list(&list, i)) {
                std::cout << "\xE2\x94\x82 ";
            }
            else 
                std::cout << "  ";
        }

        if (type == TEXT) {
            cout << "<TEXT> " + text << endl;
        } else {
            cout << "<ELEMENT> " + tag_name << endl;
        }
    }
};

void print_dom_node(DomNode& root, int32_t depth, bool is_last_child, vector<int32_t>& list) {
    ++depth;

    root.print(depth, is_last_child, list);
    for (vector<DomNode>::iterator it = root.child_list.begin();
        it != root.child_list.end(); ++it) {
        if (it == root.child_list.begin()) {
            list.push_back(depth);
        }
        if (it == root.child_list.end() - 1) {
            list.pop_back();
        }
        print_dom_node(*it, depth, it == root.child_list.end() - 1, list);
    }
    --depth;
}

void print_dom_list(vector<DomNode> root_list) {
    static int32_t depth = 0;
    static vector<int32_t> list;

    for (vector<DomNode>::iterator it = root_list.begin();
        it != root_list.end(); ++it) {
        print_dom_node(*it, depth, it == root_list.end() - 1, list);
    }
}

}