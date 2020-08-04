#pragma once

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