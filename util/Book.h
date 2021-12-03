//
// Created by hasee on 2021/11/30.
//

#ifndef TEXTTRANSFORM_BOOK_H
#define TEXTTRANSFORM_BOOK_H

#include <nlohmann/json.hpp>

#include <string>

using string = std::string;
using json = nlohmann::json;

json newJson();

class Book {
public:
    string title;
    string author;
    string cover;

    Book(json book);
};

#endif //TEXTTRANSFORM_BOOK_H
