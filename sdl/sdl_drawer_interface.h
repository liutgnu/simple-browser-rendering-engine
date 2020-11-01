#pragma once
#include "../layout/layout_node.h"
#include "shape_drawer.h"
#include "../utils/checkers.h"
#include "color.h"
#include "../utils/font_manager.h"

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
    vector<Html_Rect> rect_list;
    vector<Font_Draw_Info> font_list;
    FontManager& font_manager;
    SDL_Renderer *render;

    SdlDrawerInterface(FontManager& font_manager, SDL_Renderer *render): 
        font_manager(font_manager), render(render){}

    void iterate_layout_tree(const LayoutNode& node) {

        Html_Rect content_rect;
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

        Html_Rect padding_rect;
        padding_rect.edge = convert_from_layout_edge(node.box.padding);
        padding_rect.rect.x = content_rect.rect.x - padding_rect.edge.l;
        padding_rect.rect.y = content_rect.rect.y - padding_rect.edge.t;
        padding_rect.rect.w = content_rect.rect.w + padding_rect.edge.l + padding_rect.edge.r;
        padding_rect.rect.h = content_rect.rect.h + padding_rect.edge.t + padding_rect.edge.b;

        padding_rect.color = convert_from_css_color(*find_in_map_or_default(node.property_map, 
            make_string_vector(1, "background"), &v));

        rect_list.push_back(padding_rect);

        Html_Rect border_rect;
        border_rect.edge = convert_from_layout_edge(node.box.border);
        border_rect.rect.x = padding_rect.rect.x - border_rect.edge.l;
        border_rect.rect.y = padding_rect.rect.y - border_rect.edge.t;
        border_rect.rect.w = padding_rect.rect.w + border_rect.edge.l + border_rect.edge.r;
        border_rect.rect.h = padding_rect.rect.h + border_rect.edge.t + border_rect.edge.b;

        border_rect.color = convert_from_css_color(*find_in_map_or_default(node.property_map, 
            make_string_vector(1, "border-color"), &v));

        rect_list.push_back(border_rect);

        if (node.type == TEXT) {
            Value transparent_color(make_tuple(TRANSPARENT));
            Value black_color(make_tuple(BLACK));

            if (const Value *tmp = find_in_map(node.property_map, string("font-size"))) {
                font_manager.set_font_size(tmp->to_px());
            }

            wstring text = font_manager.convert_to_wstring(node.text);
            font_manager.draw_string(text, 
                Font_Color {
                    .font_color = *(SDL_Color *)(&find_in_map_or_default(node.property_map,
                        make_string_vector(1, "color"), &black_color)->Color),
                    .background_color = *(SDL_Color *)(&find_in_map_or_default(node.property_map,
                        make_string_vector(1, "background"), &transparent_color)->Color),
                }, 
                Font_Position {
                    .x = content_rect.rect.x,
                    .y = content_rect.rect.y + (int)(font_manager.ft_face->size->metrics.ascender >> 6),
                },
                font_list, render);
            font_manager.restore_default_font_size();    
        }

        for (vector<LayoutNode>::const_iterator it = node.child_list.begin();
            it != node.child_list.end(); ++it) {
            iterate_layout_tree(*it);
        }        
    }
};

}