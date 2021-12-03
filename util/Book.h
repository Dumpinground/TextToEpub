//
// Created by hasee on 2021/11/30.
//

#ifndef TEXTTRANSFORM_BOOK_H
#define TEXTTRANSFORM_BOOK_H

#include <nlohmann/json.hpp>

#include <string>

using string = std::string;
using json = nlohmann::json;

#define VNAME(x) #x
#define VDUMP(x) std::cout << VLIST(x) << std::endl

json newJson();

class Book {
public:
    string title;
    string author;
    string cover;

    explicit Book(json book);

    friend std::ostream &operator<<(std::ostream &out, Book &book);
};

#endif //TEXTTRANSFORM_BOOK_H
