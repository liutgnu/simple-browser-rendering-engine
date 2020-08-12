#pragma once
#include "layout_element.h"
#include "../style/style_node.h"
#include <assert.h>
#include <vector>
#include <numeric>

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



void calculate_node_width(const StyleDomNode& node, const Box& container_box) {
    Value zero_length(make_tuple(0.0, "px"));
    Value auto_keyword("auto");
    float width_ma_L, width_ma_R, width_ma_; 

    const Value *margin_left = find_in_map_or_default(node.property_map, 
        make_type_vector<string>(2, "margin-left", "margin"), &zero_length);
    const Value *margin_right = find_in_map_or_default(node.property_map, 
        make_type_vector<string>(2, "margin-right", "margin"), &zero_length);

    const Value *padding_left = find_in_map_or_default(node.property_map, 
        make_type_vector<string>(2, "padding-left", "padding"), &zero_length);
    const Value *padding_right = find_in_map_or_default(node.property_map, 
        make_type_vector<string>(2, "padding-right", "padding"), &zero_length);

    const Value *border_left = find_in_map_or_default(node.property_map, 
        make_type_vector<string>(2, "border-left-width", "border-width"), &zero_length);
    const Value *border_right = find_in_map_or_default(node.property_map, 
        make_type_vector<string>(2, "border-right-width", "border-width"), &zero_length);

    const Value *width = find_in_map_or_default(node.property_map,
        make_type_vector<string>(1, "width"), &zero_length);

    vector<Value> width_list_origin = make_type_vector<Value>(7, 
        *margin_left, *margin_right, *padding_left, 
        *padding_right, *border_left, *border_right, *width);
    
    vector<float> width_list(7);
    transform(width_list_origin.cbegin(), width_list_origin.cend(), width_list.begin(), 
        [](const Value& v) -> float {
            if (v.type == LENGTH && v.Length.unit == "px") {
                return v.Length.data;
            }
            return 0.0;
    });
    float total = accumulate(width_list.cbegin(), width_list.cend(), 0, [](float a, float b) -> float {return a + b;});
    float overflow = total - container_box.content.width;

    tuple<bool, bool, bool> result = make_tuple(
        auto_keyword == *margin_left, auto_keyword == *width, auto_keyword == *margin_right);

    if (result == make_tuple(false, false, false)) {
        width_ma_R = margin_right->to_px() + overflow;
    } else if (result == make_tuple(false, false, true)) {
        width_ma_R = margin_right->to_px() + overflow;
    } else if (result == make_tuple(true, false, false)) {
        width_ma_L = margin_right->to_px() + overflow;
    } else if (result == make_tuple(true, false, true)) {
        
    }
}

void calculate_node_layout(const StyleDomNode& node, const Box& container_box) {

}

}