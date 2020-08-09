#include "dom.h"
#include "../utils/checkers.h"

using namespace std;
namespace simple_browser_html {

void DomNode::print(int32_t depth, bool is_last_child, vector<int32_t>& list) {
    for (int32_t i = 0; i < depth; ++i) {
        if (is_last_child && i == depth - 1) {
            std::cout << "\xE2\x94\x94\xE2\x94\x80";
        } else if (!is_last_child && i == depth - 1) {
            std::cout << "\xE2\x94\x9c\xE2\x94\x80";
        }
        else if (in_vector(list, i, [](int32_t a, int32_t b) -> bool{ return a == b; })) {
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