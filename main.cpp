//
// Created by hasee on 2021/11/30.
//
#include <iostream>
#include "util/Book.h"
#include <fstream>

using namespace std;

int main(int argc, char **argv) {

    Book book;
    ifstream i;

    switch (argc) {
        case 1:
            cout << "Here is " << argv[0] << endl;
            break;

        case 3:
            if (string(argv[1]) == "--new_resource") {
                book.BuildInit(argv[2]);
            }
            break;

        case 4:
            // argv: 1: resourceDir 2: book_name 3: outputDir
            book = Book::getJson(string(argv[2]) + ".json", string(argv[1]) + "/").get<Book>();
            book.CreateBuildDir(argv[3]);
            book.extract(book.TextRoot() + string(argv[2]) + ".txt", book.dir_path() + "EPUB/");
            book.PackBook();
            break;

        default:
            cout << R"(
--help show this

--new_resource [dir] create a resource folder

[resource] [name] [output]  output EPub dir from name.txt and name.json in resource;
)" << endl;

    }

    return 0;
}
