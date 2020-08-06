#include "css_parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "css_element.h"

using simple_browser_css::Rule;
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
    vector<Rule> rules = cssParser.parse_css_rules();
    for (int i = 0; i < rules.size(); ++i) {
        rules[i].print(i == rules.size() - 1);
    }

    return 0;
}