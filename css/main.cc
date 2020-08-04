#include "css_parser.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2) {
        cout << "useage: ./a.out <css_file>" << endl;
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

    simple_browser_css::CssParser cssParser(source);
    cssParser.parse_css_rules();

    return 0;
}