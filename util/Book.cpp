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
}
