#pragma once
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <cstdarg>

using namespace std;
auto is_char = [](char c) -> bool {
    return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
};

auto is_num = [](char c) -> bool {
    return (c >= '0') && (c <= '9');
};

auto is_blank = [](char c) -> bool {
    return (c == ' ') || (c == '\t') || (c == '\n');
};

auto is_identifier = [](char c) -> bool {
    return (is_char(c) || is_num(c) || c == '_' || c == '-');
};

auto is_hex = [](char c) -> bool {
    return (is_num(c) || 
                ((c >= 'A') && (c <= 'F')) ||
                ((c >= 'a') && (c <= 'f')));
};

auto string_equal = [](const string& a, const string& b) -> bool {
    return a == b;
};

template<typename T, typename F>
bool in_vector (const vector<T>& list, const T& elem, const F& f) {
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (f(*it, elem)) {
            return true;
        }
    }
    return false;
}

auto split_string = [](const string& source, const vector<char>& delimits) -> vector<string> {
    vector<int> positions;
    vector<string> ret;
    for (auto it = source.begin(); it != source.end(); ++it) {
        if (in_vector(delimits, *it, [](char a, char b) -> bool{return a == b;})) {
            positions.push_back(it - source.begin());
        }
    }
    int st;
    vector<int>::iterator en;

    if (positions.size() > 0) {
        for (en = positions.begin(), st = 0; en <= positions.end(); st = *en + 1, ++en) {
            ret.push_back(source.substr(st, *en - st));
        }
    } else {
        ret.push_back(source);
    }
    return ret;
};

template<typename T, typename F, typename Q>
bool exist_in_map(map<T, F>& mapping, const T& key, const F& val, const Q& op) {
    auto it = mapping.find(key);
    if (it != mapping.end() && op(it->second, val)) {
        return true;
    }
    return false;
}

template<typename K, typename V>
const V* find_in_map(const map<K, V>& mapping, const K& key) {
    auto it = mapping.find(key);
    if (it == mapping.end()) {
        return nullptr;
    }
    return &it->second;
}

template<typename K, typename V>
const V* find_in_map_or_default(const map<K, V>& mapping, const vector<K>& keys, const V* defalut) {
    const V* ret;
    for (auto pk = keys.cbegin(); pk != keys.cend(); ++pk) {
        if (ret = find_in_map(mapping, *pk)) {
            return ret;
        }
    }
    return defalut;
}

template <typename T>
vector<T> make_type_vector(int len, ...) {
    vector<T> ret;
    va_list va_p;
    
    va_start(va_p, len);
    for (int i = 0; i < len; ++i) {
        ret.push_back(va_arg(va_p, T));
    }
    va_end(va_p);
    return ret;
}