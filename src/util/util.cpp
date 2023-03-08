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

string mediaType(const string& file) {
    auto pos = file.find_last_of('.');
    if (pos == string::npos) {
        return "";
    }
    string extension = file.substr(pos + 1, file.length() - pos - 1);
    string media = "image/";
    if (extension == "jpg")
        media += "jpeg";
    else
        media += extension;
    return media;
}

string join(std::vector<string> &s_vector, const string& separator) {

    if (s_vector.empty()) return "";

    string result = s_vector[0];
    for (int i = 1; i < s_vector.size(); ++i) {
        result += separator + s_vector[i];
    }

    return result;
}

bool ImageFileText::init() {
    auto pos = file.find_last_of('.');
    if (pos == string::npos) {
        return false;
    }
    name = file.substr(0, pos);
    extension = file.substr(pos + 1, file.length() - pos - 1);
    return true;
}

string ImageFileText::mediaType() const {
    string media = "image/";
    if (extension == "jpg")
        media += "jpeg";
    else
        media += extension;
    return media;
}
