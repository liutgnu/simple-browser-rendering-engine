#pragma once
#include <cstring>
#include <string>
using namespace std;
namespace simple_browser_layout {

struct Rect {
    float x;
    float y;
    float width;
    float height;

    string to_string() {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf), "%f %f %f %f", x, y, width, height);
        return string(buf);
    }
};

struct Edge {
    float left;
    float right;
    float top;
    float bottom;

    string to_string() {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf), "%f %f %f %f", left, right, top, bottom);
        return string(buf);
    }
};

struct Box {
    struct Rect content;
    struct Edge padding;
    struct Edge border;
    struct Edge margin;

    string get_content() {
        return "content-xywh: " + content.to_string();
    }
    string get_padding() {
        return "padding-lrtb: " + padding.to_string();
    }
    string get_border() {
        return "border-lrtb: " + border.to_string();
    }
    string get_margin() {
        return "margin-lrtb: " + margin.to_string();
    }
};

enum BoxTypeEnum {
    BLOCK,
    INLINE,
    ANONYMOUS,
};


}