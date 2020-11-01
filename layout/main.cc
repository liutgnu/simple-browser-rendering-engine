#include "../html/html_parser.h"
#include "../css/css_parser.h"
#include "../style/style_node_parser.h"
#include "layout_node.h"
#include <iostream>
#include <fstream>
#include <sstream>

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

    styleParser.print();
    simple_browser_layout::LayoutNode root = simple_browser_layout::combine_style_dom(
        styleParser.parse_style_dom_node(styleParser.domNode, styleParser.rules, nullptr));

    simple_browser_layout::Box canvas;
    canvas.content.x = 0;
    canvas.content.y = 0;
    canvas.content.width = 800;
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
    return 0;
}