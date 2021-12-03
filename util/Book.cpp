//
// Created by hasee on 2021/11/30.
//

#include "Book.h"

#include <utility>
#include <fstream>

using namespace std;

void Book::Create(const string &dir) {
    ifstream i("../template/template.json");
    ofstream o(dir + "/newBook.json");
    o << i.rdbuf();
}

Book::Book(json jsonFile) {
    content = std::move(jsonFile);
}

Book::Book(const string& jsonFilePath) {
    ifstream i(jsonFilePath);
    i >> content;
}

std::ostream &operator<<(std::ostream &out, Book &book) {
    out << setw(4) << book.content << endl;
    return out;
}
