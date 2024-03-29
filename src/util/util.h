//
// Created by hasee on 2021/12/16.
//

#ifndef TEXTTOEPUB_UTIL_H
#define TEXTTOEPUB_UTIL_H

#include <nlohmann/json.hpp>

#include <string>
#include <functional>
#include <iostream>
#include <fstream>
#include <queue>
#include <pugixml.hpp>
#include <regex>
//#include <boost/algorithm/string.hpp>
//#include <boost/format.hpp>

#include "../util/uuid.h"

using string = std::string;
using json = nlohmann::json;

#define TemplateRoot string("../template/")

#define VNAME(x) #x
#define VDUMP(x) std::cout << VLIST(x) << std::endl

std::vector<string> split(const string& text, const std::vector<string>& separators);

struct ImageFileText {
    string file, name, extension;

    bool init();
    string mediaType() const;
};

string mediaType(const string& file);

string join(std::vector<string> &s_vector, const string& separator);

#endif //TEXTTOEPUB_UTIL_H
