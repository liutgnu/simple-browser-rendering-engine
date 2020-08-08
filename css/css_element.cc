#include "css_element.h"
using namespace std;

namespace simple_browser_css {

string Selector::to_string() {
    string ret;
    if (id.length() > 0) {
        ret += "#" + id + " ";
    }
    if (tag.length() > 0) {
        ret += tag + " ";
    }
    for (int i = 0; i < class_list.size(); ++i) {
        ret += "." + class_list[i] + " ";
    }
    ret.resize(ret.length() - 1);
    return ret;
}

Value::Value(const string& kwd): type(KEYWORD) {
    new(&this->Keyword.keyword) string(kwd);
}
Value::Value(const tuple<float, string>& length): type(LENGTH) {
    Length.data = get<0>(length);
    new(&this->Length.unit) string(get<1>(length));
}
Value::Value(const tuple<uint8_t, uint8_t, uint8_t, uint8_t>& color): type(COLOR) {
    tie(Color.r, Color.g, Color.b, Color.a) = color;
}
Value::Value(const Value& v) {
    switch (v.type) {
        case KEYWORD:
            new(this) Value(v.Keyword.keyword);
            return;
        case LENGTH:
            new(this) Value(make_tuple(v.Length.data, v.Length.unit));
            return;
        case COLOR:
            new(this) Value(make_tuple(v.Color.r, 
                v.Color.g, v.Color.b, v.Color.a));
            return;
        default:
            cout << "error value type" << endl;
            exit(-1);
    }
}
Value::Value() {}
Value::~Value() {}

string Value::to_string() {
    switch(type) {
        case KEYWORD: return Keyword.keyword;
        case LENGTH: return std::to_string(Length.data) + Length.unit;
        case COLOR: return std::to_string(Color.r) + " " +
                            std::to_string(Color.g) + " " +
                            std::to_string(Color.b) + " " +
                            std::to_string(Color.a);
    }
}


Declaration::Declaration(const string& name, const string& kwd): 
    name(name), value(kwd) {}
Declaration::Declaration(const string& name, const tuple<float, string>& length):
    name(name), value(length) {}
Declaration::Declaration(const string& name, const tuple<uint8_t, uint8_t, uint8_t, uint8_t>& color):
    name(name), value(color) {}
Declaration::Declaration(const Declaration& de):name(de.name), value(de.value) {}

string Declaration::to_string() {
    return name + ": " + value.to_string();
}

void Rule::print(bool is_last_rule) {
    for (int i = 0; i < selectors.size(); ++i) {
        if (is_last_rule && i ==  selectors.size() - 1) {
            cout << "\xE2\x94\x94\xE2\x94\x80" << selectors[i].to_string() << endl;
        } else {
            cout << "\xE2\x94\x9c\xE2\x94\x80" << selectors[i].to_string() << endl;
        }
        for (int i = 0; i < declarations.size(); ++i) {
            if (i != declarations.size() - 1) 
                cout << (is_last_rule ? " " : "\xE2\x94\x82") << "  \xE2\x94\x9c\xE2\x94\x80" << declarations[i].to_string() << endl;
            else
                cout << (is_last_rule ? " " : "\xE2\x94\x82") << "  \xE2\x94\x94\xE2\x94\x80" << declarations[i].to_string() << endl;
        }
    }
}

}