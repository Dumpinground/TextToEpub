//
// Created by hasee on 2021/11/30.
//

#include "Book.h"

json newJson() {
    return {
            {"title", ""},
            {"author", ""},
            {"cover", ""}
    };
}

Book::Book(json book) {
    title = book["title"];
    author = book["author"];
    cover = book["cover"];
}

std::ostream &operator<<(std::ostream &out, Book &book) {

    out << "{" << std::endl;
    out << "    " << VNAME(book.title) << ": " << book.title << std::endl;
    out << "    " << VNAME(book.author) << ": " << book.author << std::endl;
    out << "    " << VNAME(book.cover) << ": " << book.cover << std::endl;
    out << "}" << std::endl;
    return out;
}
