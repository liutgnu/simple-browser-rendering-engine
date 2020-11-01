#pragma once
#include "layout_element.h"
#include "../style/style_node.h"
#include <assert.h>
#include <vector>
#include <numeric>
#include <cstring>

typedef struct Fake_Box {
    float width;
    float height;
    float pen_x;
    float pen_y;
} Fake_Box;

using namespace std;
using namespace simple_browser_style;
namespace simple_browser_layout {

class LayoutNode : public StyleDomNode {
    public:
    BoxTypeEnum box_type;
    struct Box box;
    vector<LayoutNode> child_list;

    LayoutNode(BoxTypeEnum box_type, const StyleDomNode& node);
    LayoutNode(BoxTypeEnum box_type);
    LayoutNode();
    void print_box_property(int32_t depth, bool is_last_child, vector<int32_t>& list);

};

void layout_block_node(LayoutNode& node, const Box& container_box,
    Fake_Box& container_fakebox, float& container_current_max_lineheight);
void layout_node_print(LayoutNode& node, bool is_last_child);
LayoutNode combine_style_dom(const StyleDomNode& node);
}