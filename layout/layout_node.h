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


void calculate_block_node_width(LayoutNode& node, const Box& container_box) {
    Value zero_length(make_tuple(0.0, "px"));
    Value auto_value("auto");
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
        [](const Value& v) {
            return v.to_px();
    });
    float total = accumulate(width_list.cbegin(), width_list.cend(), 0, 
        [](float a, float b) {return a + b;});
    float overflow = total - container_box.content.width;

    tuple<bool, bool, bool> result = make_tuple(
        auto_value == *margin_left, auto_value == *width, auto_value == *margin_right);

    width_ma_L = margin_left->to_px();
    width_ma_R = margin_right->to_px();
    width_ma_ = width->to_px();

    if (make_tuple(get<0>(result), get<1>(result)) == make_tuple(false, false)) {  // (false, false, ~)
        width_ma_R = margin_right->to_px() + overflow;
    } else if (result == make_tuple(true, false, false)) {
        width_ma_L = margin_left->to_px() + overflow;
    } else if (result == make_tuple(true, false, true)) {
        width_ma_R = overflow / 2.0;
        width_ma_L = overflow / 2.0;
    } else if (get<1>(result) == true) {  // (~, true, ~)
        width_ma_L = auto_value == *margin_left ? 0 : margin_left->Length.data;
        width_ma_R = auto_value == *margin_right ? 0 : margin_right->Length.data;
        if (overflow >= 0) {
            width_ma_ = overflow;
        } else {
            width_ma_ = 0;
            width_ma_R += overflow;
        }
    } else {  // never reach here
        assert(false);
    }

    node.box.content.width = width_ma_;
    node.box.padding.left = padding_left->to_px();
    node.box.padding.right = padding_right->to_px();
    node.box.border.left = border_left->to_px();
    node.box.border.right = border_right->to_px();
    node.box.margin.left = width_ma_L;
    node.box.margin.right = width_ma_R;
}

void calculate_block_node_position(LayoutNode& node, const Box& container_box) {
    Value zero_length(make_tuple(0.0, "px")); 
    node.box.margin.top = (node.property_map,
        make_type_vector<string>(2, "margin-top", "margin"), &zero_length)->to_px();
    node.box.margin.bottom = (node.property_map,
        make_type_vector<string>(2, "margin-bottom", "margin"), &zero_length)->to_px();
    node.box.border.top = (node.property_map,
        make_type_vector<string>(2, "border-top", "border"), &zero_length)->to_px();
    node.box.border.bottom = (node.property_map,
        make_type_vector<string>(2, "border-bottom", "border"), &zero_length)->to_px();
    node.box.padding.top = (node.property_map,
        make_type_vector<string>(2, "padding-top", "padding"), &zero_length)->to_px();
    node.box.padding.bottom = (node.property_map,
        make_type_vector<string>(2, "padding-bottom", "padding"), &zero_length)->to_px();
    node.box.content.x = container_box.content.x + node.box.margin.left + 
        node.box.border.left + node.box.padding.left;
    node.box.content.y = container_box.content.y + node.box.margin.top + 
        node.box.border.top + node.box.padding.top;
}

void calculate_block_children(LayoutNode& node) {

}

// void calculate_block_node_layout(LayoutNode& node, const Box& container_box) {
//     switch (node.type) {
//         case BLOCK:
//             return calculate_block_node_width(node, container_box);
//         case INLINE:
//         case ANONYMOUS:
//             return;
//         default:
//             assert(false);
//     }
// }

}