#pragma once
#include "../layout/layout_node.h"
#include "shape_drawer.h"
#include "../utils/checkers.h"
#include "color.h"

using namespace simple_browser_layout;
using namespace std;
namespace simple_browser_sdldrawer {

SDL_Rect convert_from_layout_rect(simple_browser_layout::Rect rect)
{
    return SDL_Rect {
        .x = (int)rect.x,
        .y = (int)rect.y,
        .w = (int)rect.width,
        .h = (int)rect.height,
    };
}

SDL_Color convert_from_css_color(simple_browser_css::Value value)
{
    assert(value.type == COLOR);
    return SDL_Color {
        .r = value.Color.r,
        .g = value.Color.g,
        .b = value.Color.b,
        .a = value.Color.a,
    };
}

Rect_Edge convert_from_layout_edge(simple_browser_layout::Edge edge)
{
    return Rect_Edge {
        .t = (int)edge.top,
        .r = (int)edge.right,
        .b = (int)edge.bottom,
        .l = (int)edge.left,
    };
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

class SdlDrawerInterface {

public:
    vector<Html_rect> rect_list;

    SdlDrawerInterface() {}

    void iterate_layout_tree(const LayoutNode& node) {

        Html_rect content_rect;
        content_rect.rect = convert_from_layout_rect(node.box.content);
        content_rect.edge = Rect_Edge {
            .t = content_rect.rect.h,
            .r = 0,
            .b = 0,
            .l = content_rect.rect.w,
        };

        Value v = Value(make_tuple(TRANSPARENT));

        content_rect.color = convert_from_css_color(*find_in_map_or_default(node.property_map, 
            make_string_vector(1, "background"), &v));
        
        rect_list.push_back(content_rect);

        Html_rect padding_rect;
        padding_rect.edge = convert_from_layout_edge(node.box.padding);
        padding_rect.rect.x = content_rect.rect.x - padding_rect.edge.l;
        padding_rect.rect.y = content_rect.rect.y - padding_rect.edge.t;
        padding_rect.rect.w = content_rect.rect.w + padding_rect.edge.l + padding_rect.edge.r;
        padding_rect.rect.h = content_rect.rect.h + padding_rect.edge.t + padding_rect.edge.b;

        padding_rect.color = convert_from_css_color(*find_in_map_or_default(node.property_map, 
            make_string_vector(1, "background"), &v));

        rect_list.push_back(padding_rect);

        Html_rect border_rect;
        border_rect.edge = convert_from_layout_edge(node.box.border);
        border_rect.rect.x = padding_rect.rect.x - border_rect.edge.l;
        border_rect.rect.y = padding_rect.rect.y - border_rect.edge.t;
        border_rect.rect.w = padding_rect.rect.w + border_rect.edge.l + border_rect.edge.r;
        border_rect.rect.h = padding_rect.rect.h + border_rect.edge.t + border_rect.edge.b;

        border_rect.color = convert_from_css_color(*find_in_map_or_default(node.property_map, 
            make_string_vector(1, "border-color"), &v));

        rect_list.push_back(border_rect);

        for (vector<LayoutNode>::const_iterator it = node.child_list.begin();
            it != node.child_list.end(); ++it) {
            iterate_layout_tree(*it);
        }        
    }
};

}