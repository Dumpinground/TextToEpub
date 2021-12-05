//
// Created by hasee on 2021/11/30.
//

#include "Book.h"

#include <iostream>
#include <fstream>

using namespace std;

void Book::Create(const string &dir) {
    ifstream i("../template/template.json");
    ofstream o(dir + "/newBook.json");
    o << i.rdbuf();
}

Book::Book() = default;

std::ostream &operator<<(std::ostream &out, Book &book) {
    out << setw(4) << json(book) << endl;
    return out;
}

void Book::CreateBuild(const string &path) {
    string command;
    sprintf(command.data(), "md %s/", path.data());

    auto mkdir = [&command] (const string &path) {
        cout << command + path << endl;
    };

    string dir_name;
    sprintf(dir_name.data(), "[%s].%s%s.%s",
            contributor.author.data(),
            metadata.title.data(),
            metadata.subtitle.data(),
            metadata.volume.data());
    mkdir(dir_name);

//    system(command + "");
}
