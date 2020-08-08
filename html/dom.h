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
    
    void print(int32_t depth, bool is_last_child, vector<int32_t>& list);
};

void print_dom_node(DomNode& root, int32_t depth, bool is_last_child, vector<int32_t>& list);
void print_dom_list(vector<DomNode> root_list);
bool in_list(vector<int> *list, int val);
}