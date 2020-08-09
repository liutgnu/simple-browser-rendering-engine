#pragma once
#include "layout_element.h"
#include "../style/style_node.h"
#include <assert.h>
#include <vector>

using namespace std;
using namespace simple_browser_style;
namespace simple_browser_layout {

class LayoutNode : public StyleDomNode {
    public:
    BoxTypeEnum box_type;
    struct Box box;
    vector<LayoutNode> child_list;

    LayoutNode(BoxTypeEnum box_type, const StyleDomNode& node):
        box_type(box_type), StyleDomNode(node) { }
    LayoutNode(BoxTypeEnum box_type): box_type(box_type){ }
    LayoutNode() {}

};

LayoutNode trans_style_dom(const StyleDomNode& node) {
    LayoutNode layoutNode;

    if (node.display() == "block") {
        layoutNode = LayoutNode(BLOCK, node);
    } else if (node.display() == "inline") {
        layoutNode = LayoutNode(INLINE, node);
    } else {
        assert(false);
    }

    for (auto it = node.child_sdn_list.begin(); it != node.child_sdn_list.end(); ++it) {
        if (it->display() == "block") {
            layoutNode.child_list.push_back(trans_style_dom(*it));
        } else if (it->display() == "none") {
            continue;
        } else if (it->display() == "inline") {
            if (layoutNode.box_type == INLINE) {
                layoutNode.child_list.push_back(trans_style_dom(*it));
            } else {
                if (layoutNode.child_list.size() == 0 || layoutNode.child_list.back().box_type != ANONYMOUS) {
                    layoutNode.child_list.push_back(LayoutNode(ANONYMOUS));
                }
                layoutNode.child_list.back().child_list.push_back(trans_style_dom(*it));
            }
        } else {
            assert(false);
        }
    }
    return layoutNode;
}

}