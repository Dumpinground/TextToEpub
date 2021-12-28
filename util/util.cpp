//
// Created by hasee on 2021/12/28.
//

#include "util.h"

std::vector<string> split(const string& text, const std::vector<string>& separators) {

    std::vector<string> result;
    auto left = text.find(separators[0]), right = text.find(separators[1]);
    result.push_back(text.substr(0, left));
    int i = 0;
    while (i < separators.size() - 1) {
        result.push_back(text.substr(left + separators[i].length(), right - left - separators[i].length()));
        if (++i + 1 < separators.size())
            left = right, right = text.find(separators[i + 1], right + separators[i + 1].length());
    }
    result.push_back(text.substr(right + separators[i].length(), text.length()));
    return result;
}
