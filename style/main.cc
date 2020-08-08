#include "../html/html_parser.h"
#include "../css/css_parser.h"
#include "style_node_parser.h"
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
        htmlParser.parse_dom_nodes(), cssParser.parse_css_rules());

    styleParser.print();
    return 0;
}