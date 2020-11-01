#include "../html/html_parser.h"
#include "../css/css_parser.h"
#include "../style/style_node_parser.h"
#include "../layout/layout_node.h"
#include "sdl_drawer.h"
#include "sdl_drawer_interface.h"
#include <iostream>
#include <fstream>
#include <sstream>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3) {
        cout << "useage: ./a.out <html_file> <css_file>" << endl;
        return -1;
    }
    ifstream file_html(argv[1]);
    ifstream file_css(argv[2]);
    if (!file_html || !file_css) {
        cout << "open file error" << endl;
        return -1;
    }
    stringstream ss;
    ss << file_html.rdbuf();
    string html_source = ss.str();
    simple_browser_html::HtmlParser htmlParser(html_source);
    ss.str("");

    ss << file_css.rdbuf();
    string css_source = ss.str();
    simple_browser_css::CssParser cssParser(css_source);
    
    simple_browser_style::StyleDomNodeParser styleParser(
        htmlParser.parse_dom_node(), cssParser.parse_css_rules());

    simple_browser_layout::LayoutNode root = simple_browser_layout::combine_style_dom(
        styleParser.parse_style_dom_node(styleParser.domNode, styleParser.rules, nullptr));

    styleParser.print();

    simple_browser_layout::Box canvas;
    canvas.content.x = 0;
    canvas.content.y = 0;
    canvas.content.width = WINDOW_WIDTH;
    canvas.content.height = 0;

    Fake_Box fake_box {
        .width = 0,
        .height = 0,
        .pen_x = 0,
        .pen_y = 0,
    };

    float line_height = 0;
    simple_browser_layout::layout_block_node(root, canvas, fake_box, line_height);
    simple_browser_layout::layout_node_print(root, true);

    simple_browser_sdldrawer::SdlDrawer sdlDrawer(WINDOW_WIDTH, WINDOW_HEIGHT);
    bool status = sdlDrawer.init_sdldrawer();
    if (!status) {
        cout << "init sdldrawer error!" << endl;
        return -1;
    }
    FontManager fontManager;
    simple_browser_sdldrawer::SdlDrawerInterface drawer_interface(fontManager, sdlDrawer.res.render);
    drawer_interface.iterate_layout_tree(root);

    sdlDrawer.run_sdldrawer(drawer_interface);

    return 0;
}