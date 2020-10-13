#include "layout_node.h"

using namespace std;
using namespace simple_browser_style;
namespace simple_browser_layout {

LayoutNode::LayoutNode(BoxTypeEnum box_type, const StyleDomNode& node):
    box_type(box_type), StyleDomNode(node) {
        memset(&box, 0, sizeof(box));
    }
LayoutNode::LayoutNode(BoxTypeEnum box_type): box_type(box_type) {
    memset(&box, 0, sizeof(box));
}
LayoutNode::LayoutNode() {
    memset(&box, 0, sizeof(box));
}

void LayoutNode::print_box_property(int32_t depth, bool is_last_child, vector<int32_t>& list) {
    vector<string> box_properties;
    box_properties.push_back(box.get_content());
    box_properties.push_back(box.get_margin());
    box_properties.push_back(box.get_border());
    box_properties.push_back(box.get_padding());

    for (vector<string>::iterator it = box_properties.begin(); it != box_properties.end(); ++it) {
        for (int32_t i = 0; i < depth; ++i) {
            if (in_vector(list, i, [](int32_t a, int32_t b) -> bool { return a == b; })) {
                std::cout << "\xE2\x94\x82 ";
            }
            else 
                std::cout << "  ";
        }
        cout << *it << endl;
    }
}

LayoutNode combine_style_dom(const StyleDomNode& node) {
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
            layoutNode.child_list.push_back(combine_style_dom(*it));
        } else if (it->display() == "none") {
            continue;
        } else if (it->display() == "inline") {
            if (layoutNode.box_type == INLINE) {
                layoutNode.child_list.push_back(combine_style_dom(*it));
            } else {
                if (layoutNode.child_list.size() == 0 || layoutNode.child_list.back().box_type != ANONYMOUS) {
                    layoutNode.child_list.push_back(LayoutNode(ANONYMOUS));
                }
                layoutNode.child_list.back().child_list.push_back(combine_style_dom(*it));
            }
        } else {
            assert(false);
        }
    }
    return layoutNode;
}

vector<string> make_string_vector(int len, ...) {
    vector<string> ret;
    va_list va_p;
    
    va_start(va_p, len);
    for (int i = 0; i < len; ++i) {
        ret.push_back(string(va_arg(va_p, const char *)));
    }
    va_end(va_p);
    return ret;    
}

void calculate_block_node_width(LayoutNode& node, const Box& container_box) {
    Value zero_length(make_tuple(0.0, "px"));
    Value auto_value("auto");
    float width_ma_L, width_ma_R, width_ma_; 

    const Value *margin_left = find_in_map_or_default(node.property_map, 
        make_string_vector(2, "margin-left", "margin"), &zero_length);
    const Value *margin_right = find_in_map_or_default(node.property_map, 
        make_string_vector(2, "margin-right", "margin"), &zero_length);

    const Value *padding_left = find_in_map_or_default(node.property_map, 
        make_string_vector(2, "padding-left", "padding"), &zero_length);
    const Value *padding_right = find_in_map_or_default(node.property_map, 
        make_string_vector(2, "padding-right", "padding"), &zero_length);

    const Value *border_left = find_in_map_or_default(node.property_map, 
        make_string_vector(2, "border-left-width", "border-width"), &zero_length);
    const Value *border_right = find_in_map_or_default(node.property_map, 
        make_string_vector(2, "border-right-width", "border-width"), &zero_length);

    const Value *width = find_in_map_or_default(node.property_map,
        make_string_vector(1, "width"), &auto_value);

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
    float overflow = container_box.content.width - total;

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
    node.box.margin.top = find_in_map_or_default(node.property_map,
        make_string_vector(2, "margin-top", "margin"), &zero_length)->to_px();
    node.box.margin.bottom = find_in_map_or_default(node.property_map,
        make_string_vector(2, "margin-bottom", "margin"), &zero_length)->to_px();
    node.box.border.top = find_in_map_or_default(node.property_map,
        make_string_vector(2, "border-top-width", "border-width"), &zero_length)->to_px();
    node.box.border.bottom = find_in_map_or_default(node.property_map,
        make_string_vector(2, "border-bottom-width", "border-width"), &zero_length)->to_px();
    node.box.padding.top = find_in_map_or_default(node.property_map,
        make_string_vector(2, "padding-top", "padding"), &zero_length)->to_px();
    node.box.padding.bottom = find_in_map_or_default(node.property_map,
        make_string_vector(2, "padding-bottom", "padding"), &zero_length)->to_px();
    node.box.content.x = container_box.content.x + node.box.margin.left + 
        node.box.border.left + node.box.padding.left;
    node.box.content.y = container_box.content.height + container_box.content.y + 
        node.box.margin.top + node.box.border.top + node.box.padding.top;
}

void layout_block_node(LayoutNode& node, const Box& container_box);
void calculate_block_children(LayoutNode& node) {
    for (int i = 0; i < node.child_list.size(); ++i) {
        layout_block_node(node.child_list[i], node.box);
        node.box.content.height += node.child_list[i].box.content.height +
            node.child_list[i].box.margin.top + node.child_list[i].box.margin.bottom + 
            node.child_list[i].box.border.top + node.child_list[i].box.border.bottom + 
            node.child_list[i].box.padding.top + node.child_list[i].box.padding.bottom;
    }
}

void layout_block_node(LayoutNode& node, const Box& container_box) {
    calculate_block_node_width(node, container_box);
    calculate_block_node_position(node, container_box);
    calculate_block_children(node);
    Value def_height(make_tuple(node.box.content.height, "px"));
    node.box.content.height = find_in_map_or_default(node.property_map, 
       make_string_vector(1, "height"), &def_height)->to_px();
}

void layout_node_print(LayoutNode& node, bool is_last_child) {
    static int32_t depth = 0;
    static vector<int32_t> list;

    ++depth;
    node.print(depth, is_last_child, list);
    node.print_box_property(depth, is_last_child, list);

    for (vector<LayoutNode>::iterator it = node.child_list.begin();
        it != node.child_list.end(); ++it) {
        if (it == node.child_list.begin()) {
            list.push_back(depth);
        }
        if (it == node.child_list.end() - 1) {
            list.pop_back();
        }
        layout_node_print(*it, it == node.child_list.end() - 1);
    }
    --depth;
}

}