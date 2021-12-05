//
// Created by hasee on 2021/11/30.
//

#include "Book.h"

#include <iostream>
#include <fstream>
#include <iomanip>

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
    auto mkdir = [] (const string &path) {
        string command = "md " + path;
        cout << command << endl;
        system(command.data());
    };

    string dir_name = "[" + contributor.author + "]." + metadata.title + metadata.subtitle + "." + metadata.volume;
    string dir_path = path + dir_name + "/";

    mkdir(dir_path);
    mkdir(dir_path + "META-INF/");
    mkdir(dir_path + "OEBPS/");
    ofstream os(dir_path + "mimetype");
    os << "application/epub+zip";
    os.close();
//    os.open(dir_path + "META-INF/container.xml");

    mkdir(dir_path + "OEBPS/Images");
    mkdir(dir_path + "OEBPS/Styles");
    mkdir(dir_path + "OEBPS/Text");
}
