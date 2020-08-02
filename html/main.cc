#include "html_parser.h"
#include "../utils/dom.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2) {
        cout << "useage: ./a.out <html_file>" << endl;
        return -1;
    }
    ifstream file_in(argv[1]);
    if (!file_in) {
        cout << "open file error" << endl;
        return -1;
    }
    stringstream ss;
    ss << file_in.rdbuf();
    string source = ss.str();

    simple_browser_html::HtmlParser htmlParser(source);
    print_dom_list(htmlParser.parse_dom_nodes());

    return 0;
}