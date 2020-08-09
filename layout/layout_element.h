#pragma once

using namespace std;
namespace simple_browser_layout {

struct Rect {
    float x;
    float y;
    float width;
    float height;
};

struct Edge {
    float left;
    float right;
    float top;
    float bottom;
};

struct Box {
    struct Rect content;
    struct Edge padding;
    struct Edge border;
    struct Edge margin;
};

enum BoxTypeEnum {
    BLOCK,
    INLINE,
    ANONYMOUS,
};


}